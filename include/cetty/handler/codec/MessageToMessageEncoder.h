#if !defined(CETTY_HANDLER_CODEC_MESSAGETOMESSAGEENCODER_H)
#define CETTY_HANDLER_CODEC_MESSAGETOMESSAGEENCODER_H

/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (c) 2010-2012 frankee zhou (frankee.zhou at gmail dot com)
 *
 * Distributed under under the Apache License, version 2.0 (the "License").
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <cetty/channel/ChannelHandlerWrapper.h>
#include <cetty/channel/ChannelMessageHandlerContext.h>
#include <cetty/handler/codec/EncoderException.h>

namespace cetty {
namespace handler {
namespace codec {

using namespace cetty::channel;

template<typename H,
         typename OutboundIn,
         typename OutboundOut,
         typename C = ChannelMessageHandlerContext<H,
         VoidMessage,
         VoidMessage,
         OutboundIn,
         OutboundOut,
         VoidMessageContainer,
         VoidMessageContainer,
         ChannelMessageContainer<OutboundIn, MESSAGE_BLOCK>,
         ChannelMessageContainer<OutboundOut, MESSAGE_BLOCK> > >
class MessageToMessageEncoder : private boost::noncopyable {
public:
    typedef C Context;

    typedef MessageToMessageEncoder<H, OutboundIn, OutboundOut, Context> Self;

    typedef typename ChannelHandlerWrapper<H>::Handler Handler;
    typedef typename ChannelHandlerWrapper<H>::HandlerPtr HandlerPtr;

    typedef ChannelMessageContainer<OutboundIn, MESSAGE_BLOCK> OutboundContainer;
    typedef typename OutboundContainer::MessageQueue OutboundQueue;

    typedef ChannelMessageTransfer<OutboundOut,
            ChannelMessageContainer<OutboundOut, MESSAGE_BLOCK>,
            TRANSFER_OUTBOUND> OutboundTransfer;

    typedef boost::function<OutboundOut(ChannelHandlerContext&,
                                        OutboundIn const&)> Encoder;

    typedef boost::function<bool (OutboundIn const&)> EncodableChecker;

public:
    MessageToMessageEncoder() {
    }

    MessageToMessageEncoder(const Encoder& encoder)
        : encoder_(encoder),
          transfer_(),
          container_() {
    }

    MessageToMessageEncoder(const Encoder& encoder,
                            const EncodableChecker& checker)
        : encoder_(encoder),
          checker_(checker),
          transfer_(),
          container_() {
    }

    ~MessageToMessageEncoder() {}

    void registerTo(Context& ctx) {
        transfer_ = ctx.outboundTransfer();
        container_ = ctx.outboundContainer();

        ctx.setFlushFunctor(boost::bind(
                                &Self::flush,
                                this,
                                _1,
                                _2));
    }

    void setEncoder(const Encoder& encoder) {
        encoder_ = encoder;
    }

    void setEncodableChecker(const EncodableChecker& checker) {
        checker_ = checker;
    }

private:
    void flush(ChannelHandlerContext& ctx,
               const ChannelFuturePtr& future) {

        OutboundQueue& outboundQueue = container_->getMessages();

        while (!outboundQueue.empty()) {
            try {
                OutboundIn& msg = outboundQueue.front();

                if (!msg) {
                    break;
                }

                if (checker_ && !checker_(msg)) {
                    //ctx.nextOutboundMessageBuffer().add(msg);
                    outboundQueue.pop_front();
                    continue;
                }

                OutboundOut omsg = encoder_(ctx, msg);

                if (!omsg) {
                    // encode() might be waiting for more inbound messages to generate
                    // an aggregated message - keep polling.
                    outboundQueue.pop_front();
                    continue;
                }

                transfer_->unfoldAndAdd(omsg);
                outboundQueue.pop_front();
            }
            catch (const CodecException& e) {
                ctx.fireExceptionCaught(e);
            }
            catch (const std::exception& e) {
                ctx.fireExceptionCaught(EncoderException(e.what()));
            }
        }

        ctx.flush(future);
    }

    /**
     * Returns {@code true} if and only if the specified message can be encoded by this encoder.
     *
     * @param msg the message
     */

private:
    Encoder encoder_;
    EncodableChecker checker_;

    OutboundTransfer* transfer_;
    OutboundContainer* container_;
};

}
}
}

#endif //#if !defined(CETTY_HANDLER_CODEC_MESSAGETOMESSAGEENCODER_H)

// Local Variables:
// mode: c++
// End:
