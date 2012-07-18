#if !defined(CETTY_CHANNEL_CHANNELPIPELINE_H)
#define CETTY_CHANNEL_CHANNELPIPELINE_H

/*
 * Copyright 2009 Red Hat, Inc.
 *
 * Red Hat licenses this file to you under the Apache License, version 2.0
 * (the "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
 * Distributed under under the Apache License, version 2.0 (the "License").
 */

#include <map>
#include <string>
#include <boost/thread/recursive_mutex.hpp>

#include <cetty/channel/Channel.h>
#include <cetty/channel/ChannelSink.h>
#include <cetty/channel/ChannelInboundInvoker.h>
#include <cetty/channel/ChannelOutboundInvoker.h>
#include <cetty/util/ReferenceCounter.h>

namespace cetty {
namespace util {
class Exception;
}
}

namespace cetty {
namespace logging {
class InternalLogger;
}
}

namespace cetty {
namespace channel {

using namespace cetty::util;
using namespace cetty::logging;

class Channel;
class ChannelEvent;
class ChannelHandlerContext;
class ChannelPipelineException;

/**
 * A list of {@link ChannelHandler}s which handles or intercepts
 * {@link ChannelEvent}s of a {@link Channel}.  {@link ChannelPipeline}
 * implements an advanced form of the
 * <a href="http://java.sun.com/blueprints/corej2eepatterns/Patterns/InterceptingFilter.html">Intercepting
 * Filter</a> pattern to give a user full control over how an event is handled
 * and how the {@link ChannelHandler}s in the pipeline interact with each other.
 *
 * <h3>Creation of a pipeline</h3>
 *
 * For each new channel, a new pipeline must be created and attached to the
 * channel.  Once attached, the coupling between the channel and the pipeline
 * is permanent; the channel cannot attach another pipeline to it nor detach
 * the current pipeline from it.
 * <p>
 * The recommended way to create a new pipeline is to use the helper methods in
 * {@link Channels} rather than calling an individual implementation's
 * constructor:
 * <pre>
 * import static org.jboss.netty.channel.{@link Channels}.*;
 * {@link ChannelPipeline} pipeline = pipeline(); // same with Channels.pipeline()
 * </pre>
 *
 * <h3>How an event flows in a pipeline</h3>
 *
 * The following diagram describes how {@link ChannelEvent}s are processed by
 * {@link ChannelHandler}s in a {@link ChannelPipeline} typically.
 * A {@link ChannelEvent} can be handled by either a {@link ChannelUpstreamHandler}
 * or a {@link ChannelDownstreamHandler} and be forwarded to the closest
 * handler by calling {@link ChannelHandlerContext#sendUpstream(ChannelEvent)}
 * or {@link ChannelHandlerContext#sendDownstream(ChannelEvent)}.  The meaning
 * of the event is interpreted somewhat differently depending on whether it is
 * going upstream or going downstream. Please refer to {@link ChannelEvent} for
 * more information.
 * <pre>
 *                                       I/O Request
 *                                     via {@link Channel} or
 *                                 {@link ChannelHandlerContext}
 *                                           |
 *  +----------------------------------------+---------------+
 *  |                  ChannelPipeline       |               |
 *  |                                       \|/              |
 *  |  +----------------------+  +-----------+------------+  |
 *  |  | Upstream Handler  N  |  | Downstream Handler  1  |  |
 *  |  +----------+-----------+  +-----------+------------+  |
 *  |            /|\                         |               |
 *  |             |                         \|/              |
 *  |  +----------+-----------+  +-----------+------------+  |
 *  |  | Upstream Handler N-1 |  | Downstream Handler  2  |  |
 *  |  +----------+-----------+  +-----------+------------+  |
 *  |            /|\                         .               |
 *  |             .                          .               |
 *  |     [ sendUpstream() ]        [ sendDownstream() ]     |
 *  |     [ + INBOUND data ]        [ + OUTBOUND data  ]     |
 *  |             .                          .               |
 *  |             .                         \|/              |
 *  |  +----------+-----------+  +-----------+------------+  |
 *  |  | Upstream Handler  2  |  | Downstream Handler M-1 |  |
 *  |  +----------+-----------+  +-----------+------------+  |
 *  |            /|\                         |               |
 *  |             |                         \|/              |
 *  |  +----------+-----------+  +-----------+------------+  |
 *  |  | Upstream Handler  1  |  | Downstream Handler  M  |  |
 *  |  +----------+-----------+  +-----------+------------+  |
 *  |            /|\                         |               |
 *  +-------------+--------------------------+---------------+
 *                |                         \|/
 *  +-------------+--------------------------+---------------+
 *  |             |                          |               |
 *  |     [ Socket.read() ]          [ Socket.write() ]      |
 *  |                                                        |
 *  |  Cetty Internal I/O Threads (Transport Implementation) |
 *  +--------------------------------------------------------+
 * </pre>
 * An upstream event is handled by the upstream handlers in the bottom-up
 * direction as shown on the left side of the diagram.  An upstream handler
 * usually handles the inbound data generated by the I/O thread on the bottom
 * of the diagram.  The inbound data is often read from a remote peer via the
 * actual input operation such as {@link InputStream#read(byte[])}.
 * If an upstream event goes beyond the top upstream handler, it is discarded
 * silently.
 * <p>
 * A downstream event is handled by the downstream handler in the top-down
 * direction as shown on the right side of the diagram.  A downstream handler
 * usually generates or transforms the outbound traffic such as write requests.
 * If a downstream event goes beyond the bottom downstream handler, it is
 * handled by an I/O thread associated with the {@link Channel}. The I/O thread
 * often performs the actual output operation such as {@link OutputStream#write(byte[])}.
 * <p>
 * For example, let us assume that we created the following pipeline:
 * <pre>
 * {@link ChannelPipeline} p = {@link Channels}.pipeline();
 * p.addLast("1", new UpstreamHandlerA());
 * p.addLast("2", new UpstreamHandlerB());
 * p.addLast("3", new DownstreamHandlerA());
 * p.addLast("4", new DownstreamHandlerB());
 * p.addLast("5", new UpstreamHandlerX());
 * </pre>
 * In the example above, the class whose name starts with <tt>Upstream</tt> means
 * it is an upstream handler.  The class whose name starts with
 * <tt>Downstream</tt> means it is a downstream handler.
 * <p>
 * In the given example configuration, the handler evaluation order is 1, 2, 3,
 * 4, 5 when an event goes upstream.  When an event goes downstream, the order
 * is 5, 4, 3, 2, 1.  On top of this principle, {@link ChannelPipeline} skips
 * the evaluation of certain handlers to shorten the stack depth:
 * <ul>
 * <li>3 and 4 don't implement {@link ChannelUpstreamHandler}, and therefore the
 *     actual evaluation order of an upstream event will be: 1, 2, and 5.</li>
 * <li>1, 2, and 5 don't implement {@link ChannelDownstreamHandler}, and
 *     therefore the actual evaluation order of a downstream event will be:
 *     4 and 3.</li>
 * <li>If 5 extended {@link SimpleChannelHandler} which implements both
 *     {@link ChannelUpstreamHandler} and {@link ChannelDownstreamHandler}, the
 *     evaluation order of an upstream and a downstream event could be 125 and
 *     543 respectively.</li>
 * </ul>
 *
 * <h3>Building a pipeline</h3>
 * <p>
 * A user is supposed to have one or more {@link ChannelHandler}s in a
 * pipeline to receive I/O events (e.g. read) and to request I/O operations
 * (e.g. write and close).  For example, a typical server will have the following
 * handlers in each channel's pipeline, but your mileage may vary depending on
 * the complexity and characteristics of the protocol and business logic:
 *
 * <ol>
 * <li>Protocol Decoder - translates binary data (e.g. {@link ChannelBuffer})
 *                        into a Java object.</li>
 * <li>Protocol Encoder - translates a Java object into binary data.</li>
 * <li>{@link ExecutionHandler} - applies a thread model.</li>
 * <li>Business Logic Handler - performs the actual business logic
 *                              (e.g. database access).</li>
 * </ol>
 *
 * and it could be represented as shown in the following example:
 *
 * <pre>
 * {@link ChannelPipeline} pipeline = {@link Channels#pipeline() Channels.pipeline()};
 * pipeline.addLast("decoder", new MyProtocolDecoder());
 * pipeline.addLast("encoder", new MyProtocolEncoder());
 * pipeline.addLast("executor", new {@link ExecutionHandler}(new {@link OrderedMemoryAwareThreadPoolExecutor}(16, 1048576, 1048576)));
 * pipeline.addLast("handler", new MyBusinessLogicHandler());
 * </pre>
 *
 * <h3>Thread safety</h3>
 * <p>
 * A {@link ChannelHandler} can be added or removed at any time because a
 * {@link ChannelPipeline} is thread safe.  For example, you can insert a
 * {@link SslHandler} when sensitive information is about to be exchanged,
 * and remove it after the exchange.
 *
 * <h3>Pitfall</h3>
 * <p>
 * Due to the internal implementation detail of the current default
 * {@link ChannelPipeline}, the following code does not work as expected if
 * <tt>FirstHandler</tt> is the last handler in the pipeline:
 * <pre>
 * public class FirstHandler extends {@link SimpleChannelUpstreamHandler} {
 *
 *     public void messageReceived({@link ChannelHandlerContext} ctx, {@link MessageEvent} e) {
 *         // Remove this handler from the pipeline,
 *         ctx.getPipeline().remove(this);
 *         // And let SecondHandler handle the current event.
 *         ctx.getPipeline().addLast("2nd", new SecondHandler());
 *         ctx.sendUpstream(e);
 *     }
 * }
 * </pre>
 * To implement the expected behavior, you have to add <tt>SecondHandler</tt>
 * before the removal or make sure there is at least one more handler between
 * <tt>FirstHandler</tt> and <tt>SecondHandler</tt>.
 *
 *
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 *
 * @author <a href="mailto:frankee.zhou@gmail.com">Frankee Zhou</a>
 *
 * @apiviz.landmark
 * @apiviz.composedOf org.jboss.netty.channel.ChannelHandlerContext
 * @apiviz.owns       org.jboss.netty.channel.ChannelHandler
 * @apiviz.uses       org.jboss.netty.channel.ChannelSink - - sends events downstream
 */
class ChannelPipeline
    : public ChannelInboundInvoker,
      public ChannelOutboundInvoker,
      public cetty::util::ReferenceCounter<ChannelPipeline> {

public:
    typedef std::vector<std::pair<std::string, ChannelHandlerPtr> > ChannelHandlers;

public:
    /**
     * Creates a new empty pipeline.
     */
    ChannelPipeline();

    virtual ~ChannelPipeline() {}

    virtual const ChannelPtr& getChannel() const;
    virtual const ChannelSinkPtr& getSink() const;

    virtual void attach(const ChannelPtr& channel, const ChannelSinkPtr& sink);
    virtual void detach();

    virtual bool isAttached() const;

    /**
     * Inserts a {@link ChannelHandler} at the first position of this pipeline.
     *
     * @param name     the name of the handler to insert first
     * @param handler  the handler to insert first
     *
     * @throws InvalidArgumentException
     *         if there's an entry with the same name already in the pipeline
     * @throws NullPointerException
     *         if the specified name or handler is <tt>NULL</tt>
     */
    virtual void addFirst(const std::string& name, const ChannelHandlerPtr& handler);

    /**
     * Appends a {@link ChannelHandler} at the last position of this pipeline.
     *
     * @param name     the name of the handler to append
     * @param handler  the handler to append
     *
     * @throws InvalidArgumentException
     *         if there's an entry with the same name already in the pipeline
     * @throws NullPointerException
     *         if the specified name or handler is <tt>NULL</tt>
     */
    virtual void addLast(const std::string& name, const ChannelHandlerPtr& handler);

    /**
     * Inserts a {@link ChannelHandler} before an existing handler of this
     * pipeline.
     *
     * @param baseName  the name of the existing handler
     * @param name      the name of the handler to insert before
     * @param handler   the handler to insert before
     *
     * @throws NoSuchElementException
     *         if there's no such entry with the specified <tt>baseName</tt>
     * @throws InvalidArgumentException
     *         if there's an entry with the same name already in the pipeline
     * @throws NullPointerException
     *         if the specified baseName, name, or handler is <tt>NULL</tt>
     */
    virtual void addBefore(const std::string& baseName, const std::string& name, const ChannelHandlerPtr& handler);

    /**
     * Inserts a {@link ChannelHandler} after an existing handler of this
     * pipeline.
     *
     * @param baseName  the name of the existing handler
     * @param name      the name of the handler to insert after
     * @param handler   the handler to insert after
     *
     * @throws NoSuchElementException
     *         if there's no such entry with the specified <tt>baseName</tt>
     * @throws InvalidArgumentException
     *         if there's an entry with the same name already in the pipeline
     * @throws NullPointerException
     *         if the specified baseName, name, or handler is <tt>NULL</tt>
     */
    virtual void addAfter(const std::string& baseName,
        const std::string& name,
        const ChannelHandlerPtr& handler);

    /**
     * Removes the specified {@link ChannelHandler} from this pipeline.
     *
     * @throws NoSuchElementException
     *         if there's no such handler in this pipeline
     * @throws NullPointerException
     *         if the specified handler is <tt>NULL</tt>
     */
    virtual void remove(const ChannelHandlerPtr& handler);

    /**
     * Removes the {@link ChannelHandler} with the specified name from this
     * pipeline.
     *
     * @return the removed handler
     *
     * @throws NoSuchElementException
     *         if there's no such handler with the specified name in this pipeline
     * @throws NullPointerException
     *         if the specified name is <tt>NULL</tt>
     */
    virtual ChannelHandlerPtr remove(const std::string& name);

    /**
     * Removes the first {@link ChannelHandler} in this pipeline.
     *
     * @return the removed handler
     *
     * @throws NoSuchElementException
     *         if this pipeline is empty
     */
    virtual ChannelHandlerPtr removeFirst();

    /**
     * Removes the last {@link ChannelHandler} in this pipeline.
     *
     * @return the removed handler
     *
     * @throws NoSuchElementException
     *         if this pipeline is empty
     */
    virtual ChannelHandlerPtr removeLast();

    /**
     * Replaces the specified {@link ChannelHandler} with a new handler in
     * this pipeline.
     *
     * @throws NoSuchElementException
     *         if the specified old handler does not exist in this pipeline
     * @throws InvalidArgumentException
     *         if a handler with the specified new name already exists in this
     *         pipeline, except for the handler to be replaced
     * @throws NullPointerException
     *         if the specified old handler, new name, or new handler is
     *         <tt>NULL</tt>
     */
    virtual void replace(const ChannelHandlerPtr& oldHandler,
        const std::string& newName,
        const ChannelHandlerPtr& newHandler);

    /**
     * Replaces the {@link ChannelHandler} of the specified name with a new
     * handler in this pipeline.
     *
     * @return the removed handler
     *
     * @throws NoSuchElementException
     *         if the handler with the specified old name does not exist in this pipeline
     * @throws InvalidArgumentException
     *         if a handler with the specified new name already exists in this
     *         pipeline, except for the handler to be replaced
     * @throws NullPointerException
     *         if the specified old handler, new name, or new handler is
     *         <tt>NULL</tt>
     */
    virtual ChannelHandlerPtr replace(const std::string& oldName,
        const std::string& newName,
        const ChannelHandlerPtr& newHandler);

    /**
     * Returns the first {@link ChannelHandler} in this pipeline.
     *
     * @return the first handler.  <tt>NULL</tt> if this pipeline is empty.
     */
    virtual ChannelHandlerPtr getFirst() const;

    /**
     * Returns the last {@link ChannelHandler} in this pipeline.
     *
     * @return the last handler.  <tt>NULL</tt> if this pipeline is empty.
     */
    virtual ChannelHandlerPtr getLast() const;

    /**
     * Returns the {@link ChannelHandler} with the specified name in this
     * pipeline.
     *
     * @return the handler with the specified name.
     *         <tt>NULL</tt> if there's no such handler in this pipeline.
     */
    virtual ChannelHandlerPtr get(const std::string& name) const;

    /**
     * Returns the context object of the specified {@link ChannelHandler} in
     * this pipeline.
     *
     * @return the context object of the specified handler.
     *         <tt>NULL</tt> if there's no such handler in this pipeline.
     */
    virtual ChannelHandlerContext* getContext(const ChannelHandlerPtr& handler) const;

    /**
     * Returns the context object of the {@link ChannelHandler} with the
     * specified name in this pipeline.
     *
     * @return the context object of the handler with the specified name.
     *         <tt>NULL</tt> if there's no such handler in this pipeline.
     */
    virtual ChannelHandlerContext* getContext(const std::string& name) const;

    /**
     * Converts this pipeline into an ordered {@link Map} whose keys are
     * handler names and whose values are handlers.
     */
    virtual ChannelHandlers getChannelHandles() const;

    /**
     * Returns the {@link std::string} representation of this pipeline.
     */
    std::string toString() const;

    virtual void fireChannelCreated();
    virtual void fireChannelActive();
    virtual void fireChannelInactive();
    virtual void fireExceptionCaught(const ChannelException& cause);
    virtual void fireEventTriggered(const ChannelEvent& event);
    virtual void fireMessageUpdated();

    virtual ChannelFuturePtr bind(const SocketAddress& localAddress);
    virtual ChannelFuturePtr connect(const SocketAddress& remoteAddress);
    virtual ChannelFuturePtr connect(const SocketAddress& remoteAddress,
        const SocketAddress& localAddress);
    virtual ChannelFuturePtr disconnect();
    virtual ChannelFuturePtr close();
    virtual ChannelFuturePtr flush();
    virtual ChannelFuturePtr write(const ChannelMessage& message);

    virtual const ChannelFuturePtr& bind(const SocketAddress& localAddress, const ChannelFuturePtr& future);

    const ChannelFuturePtr& bind(
        final DefaultChannelHandlerContext ctx, final SocketAddress localAddress, final const ChannelFuturePtr& future) {
            if (localAddress == null) {
                throw new NullPointerException("localAddress");
            }
            validateFuture(future);

            EventExecutor executor = ctx.executor();
            if (executor.inEventLoop()) {
                try {
                    ((ChannelOperationHandler) ctx.handler()).bind(ctx, localAddress, future);
                } catch (Throwable t) {
                    notifyHandlerException(t);
                }
            } else {
                executor.execute(new Runnable() {
                    @Override
                        public void run() {
                            bind(ctx, localAddress, future);
                    }
                });
            }
            return future;
    }

    const ChannelFuturePtr& connect(const SocketAddress& remoteAddress, const ChannelFuturePtr& future);

    const ChannelFuturePtr& connect(const SocketAddress& remoteAddress, const SocketAddress& localAddress, const ChannelFuturePtr& future);

    const ChannelFuturePtr& connect(
        final DefaultChannelHandlerContext ctx, final SocketAddress remoteAddress,
        final SocketAddress localAddress, final const ChannelFuturePtr& future) {
            if (remoteAddress == null) {
                throw new NullPointerException("remoteAddress");
            }
            validateFuture(future);

            EventExecutor executor = ctx.executor();
            if (executor.inEventLoop()) {
                try {
                    ((ChannelOperationHandler) ctx.handler()).connect(ctx, remoteAddress, localAddress, future);
                } catch (Throwable t) {
                    notifyHandlerException(t);
                }
            }
            else {
                executor.execute(new Runnable() {
                    @Override
                        public void run() {
                            connect(ctx, remoteAddress, localAddress, future);
                    }
                });
            }

            return future;
    }

    const ChannelFuturePtr& disconnect(const ChannelFuturePtr& future);

    const ChannelFuturePtr& disconnect(final DefaultChannelHandlerContext ctx, final const ChannelFuturePtr& future) {
        // Translate disconnect to close if the channel has no notion of disconnect-reconnect.
        // So far, UDP/IP is the only transport that has such behavior.
        if (!ctx.channel().metadata().hasDisconnect()) {
            return close(ctx, future);
        }

        validateFuture(future);
        EventExecutor executor = ctx.executor();
        if (executor.inEventLoop()) {
            try {
                ((ChannelOperationHandler) ctx.handler()).disconnect(ctx, future);
            } catch (Throwable t) {
                notifyHandlerException(t);
            }
        } else {
            executor.execute(new Runnable() {
                @Override
                    public void run() {
                        disconnect(ctx, future);
                }
            });
        }

        return future;
    }

    const ChannelFuturePtr& close(const ChannelFuturePtr& future);

    const ChannelFuturePtr& close(final DefaultChannelHandlerContext ctx, final const ChannelFuturePtr& future) {
        validateFuture(future);
        EventExecutor executor = ctx.executor();
        if (executor.inEventLoop()) {
            try {
                ((ChannelOperationHandler) ctx.handler()).close(ctx, future);
            } catch (Throwable t) {
                notifyHandlerException(t);
            }
        } else {
            executor.execute(new Runnable() {
                @Override
                    public void run() {
                        close(ctx, future);
                }
            });
        }

        return future;
    }
    
    const ChannelFuturePtr& flush(const const ChannelFuturePtr&& future);

    const ChannelFuturePtr& flush(final DefaultChannelHandlerContext ctx, final const ChannelFuturePtr& future) {
        validateFuture(future);
        EventExecutor executor = ctx.executor();
        if (executor.inEventLoop()) {
            flush0(ctx, future);
        } else {
            executor.execute(new Runnable() {
                @Override
                    public void run() {
                        flush(ctx, future);
                }
            });
        }

        return future;
    }

    private void flush0(final DefaultChannelHandlerContext ctx, const ChannelFuturePtr& future) {
        try {
            ctx.flushBridge();
            ((ChannelOperationHandler) ctx.handler()).flush(ctx, future);
        } catch (Throwable t) {
            notifyHandlerException(t);
        } finally {
            if (ctx.outByteBuf != null) {
                ByteBuf buf = ctx.outByteBuf;
                if (!buf.readable()) {
                    buf.discardReadBytes();
                }
            }
        }
    }

    const ChannelFuturePtr& write(Object message, const ChannelFuturePtr& future);

    const ChannelFuturePtr& write(DefaultChannelHandlerContext ctx, final Object message, final const ChannelFuturePtr& future) {
        if (message == null) {
            throw new NullPointerException("message");
        }
        validateFuture(future);

        EventExecutor executor;
        boolean msgBuf = false;
        for (;;) {
            if (ctx == null) {
                throw new NoSuchBufferException();
            }

            if (ctx.hasOutboundMessageBuffer()) {
                msgBuf = true;
                executor = ctx.executor();
                break;
            }

            if (message instanceof ByteBuf && ctx.hasOutboundByteBuffer()) {
                executor = ctx.executor();
                break;
            }

            ctx = ctx.prev;
        }

        if (executor.inEventLoop()) {
            if (msgBuf) {
                ctx.outMsgBuf.add(message);
            } else {
                ByteBuf buf = (ByteBuf) message;
                ctx.outByteBuf.writeBytes(buf, buf.readerIndex(), buf.readableBytes());
            }
            flush0(ctx, future);
            return future;
        } else {
            final DefaultChannelHandlerContext ctx0 = ctx;
            executor.execute(new Runnable() {
                @Override
                    public void run() {
                        write(ctx0, message, future);
                }
            });
        }

        return future;
    }

    /**
     * Retrieves a boost::any which is 
     * {@link #setAttachment(const std::string&, const boost::any&) attached} to
     * this context.
     *
     * @param name the attachment's name, if the name is empty, then return the
     *        first attachment in the attachments if attachments is not empty.
     *
     * @return Empty boost::any if no such attachment was attached.
     *
     */
    virtual boost::any getAttachment(const std::string& name) const;

    /**
     * Attaches an object to this pipeline to store a some information
     * specific to the {@link ChannelPipeline} which will not take care of
     * the life cycle of the attachment.
     */
    virtual void setAttachment(const std::string& name, const boost::any& attachment);

    virtual void notifyHandlerException(const ChannelEvent& evt, const Exception& e);

public:
    static InternalLogger* getLogger() { return logger; }

protected:
    void callBeforeAdd(ChannelHandlerContext* ctx);
    void callAfterAdd(ChannelHandlerContext* ctx);
    void callBeforeRemove(ChannelHandlerContext* ctx);
    void callAfterRemove(ChannelHandlerContext* ctx);

private:
    void init(const std::string& name, const ChannelHandlerPtr& handler);

    void checkDuplicateName(const std::string& name);

    DefaultChannelHandlerContext* getContextNoLock(const std::string& name) const;
    DefaultChannelHandlerContext* getContextNoLock(const ChannelHandlerPtr& handler) const;

    DefaultChannelHandlerContext* getContextOrDie(const std::string& name);
    DefaultChannelHandlerContext* getContextOrDie(const ChannelHandlerPtr& handler);

    ChannelHandlerPtr remove(DefaultChannelHandlerContext* ctx);
    ChannelHandlerPtr replace(DefaultChannelHandlerContext* ctx, const std::string& newName, const ChannelHandlerPtr& newHandler);

    // upstream & downstream list operators.
    void updateUpstreamList();
    void updateDownstreamList();

private:
    static InternalLogger* logger;
    static ChannelSinkPtr discardingSink;

private:
    typedef std::map<std::string, DefaultChannelHandlerContext*> ContextMap;
    typedef std::map<std::string, boost::any> AttachmentMap;

    ContextMap name2ctx;

    ChannelPtr channel;
    ChannelSinkPtr sink;

    mutable boost::recursive_mutex mutex;

    ChannelHandlerContext* head;
    ChannelHandlerContext* tail;

    ChannelHandlerContext* inboundHead; //< upstream single list.
    ChannelHandlerContext* outboundHead; //< downstream single list.
    
    AttachmentMap attachments;
};

}
}

#endif //#if !defined(CETTY_CHANNEL_CHANNELPIPELINE_H)

// Local Variables:
// mode: c++
// End:

