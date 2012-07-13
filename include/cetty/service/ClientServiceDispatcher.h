#if !defined(CETTY_SERVICE_CLIENTDISPATCHER_H)
#define CETTY_SERVICE_CLIENTDISPATCHER_H

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

#include <vector>
#include <boost/bind.hpp>
#include <cetty/bootstrap/ClientBootstrap.h>

#include <cetty/channel/ChannelFuture.h>
#include <cetty/channel/ChannelPipeline.h>
#include <cetty/channel/ChannelStateEvent.h>
#include <cetty/channel/SimpleChannelHandler.h>
#include <cetty/channel/ChannelHandlerContext.h>
#include <cetty/channel/socket/asio/AsioServicePool.h>
#include <cetty/channel/socket/asio/AsioClientSocketChannelFactory.h>

#include <cetty/service/Connection.h>
#include <cetty/service/pool/ConnectionPool.h>

namespace cetty {
namespace service {

using namespace cetty::bootstrap;
using namespace cetty::channel;
using namespace cetty::channel::socket::asio;
using namespace cetty::service::pool;

template<typename ReqT, typename RepT>
class ClientServiceDispatcher : public cetty::channel::SimpleChannelHandler {
public:
    typedef OutstandingCall<ReqT, RepT> OutstandingCallType;
    typedef ServiceRequestHandler<ReqT, RepT> ServiceRequestHandlerType;
    typedef ClientServiceDispatcher<ReqT, RepT> ClientServiceDispatcherType;

    typedef boost::intrusive_ptr<OutstandingCallType> OutstandingCallPtr;

public:
    ClientServiceDispatcher(const Connections& connections,
                            const ChannelPipelinePtr& pipeline,
                            const AsioServicePtr& asioService)
        : connections(connections),
          pool(connections),
          defaultPipeline(pipeline),
          asioService(asioService) {
    }

public:
    virtual void channelConnected(ChannelHandlerContext& ctx,
                                  const ChannelStateEvent& e) {
        ChannelPtr ch = e.getChannel();
        ChannelPipelinePtr pipeline = Channels::pipelineFactory(defaultPipeline)->getPipeline();
        pipeline->addLast("requestHandler", new ServiceRequestHandlerType(ch));

        pool.getBootstrap().setPipeline(pipeline);
        pool.getBootstrap().setFactory(new AsioClientSocketChannelFactory(asioService));
    }

    virtual void messageReceived(ChannelHandlerContext& ctx,
                                 const MessageEvent& e) {
        //OutstandingCallPtr call = e.getMessage().smartPointer<OutstandingCallType>();
        ctx.sendUpstream(e);
    }

    virtual void writeRequested(ChannelHandlerContext& ctx,
                                const MessageEvent& e) {
        OutstandingCallPtr call = e.getMessage().smartPointer<OutstandingCallType>();

        ChannelPtr ch = pool.getChannel(ConnectionPool::ConnectedCallback());

        if (ch) {
            ch->write(e.getMessage());
            outStandingCalls.insert(std::make_pair(call->getId(), call));
        }
        else {
            bufferingCalls.push_back(call);
            pool.getChannel(boost::bind(
                                &ClientServiceDispatcherType::connectedCallback, this, _1));
        }
    }

    void connectedCallback(const ChannelPtr& channel) {
        BOOST_ASSERT(channel);

        while (!bufferingCalls.empty()) {
            const OutstandingCallPtr& call = bufferingCalls.front();
            channel->write(ChannelMessage(call));
            outStandingCalls.insert(std::make_pair(call->getId(), call));

            bufferingCalls.pop_front();
        }
    }

    virtual ChannelHandlerPtr clone() {
        return ChannelHandlerPtr(
                   new ClientServiceDispatcherType(connections,
                           defaultPipeline,
                           asioService));
    }

    virtual std::string toString() { return "ClientServiceDispatcherType"; }

private:
    AsioServicePtr asioService;
    ChannelPipelinePtr defaultPipeline;

    Connections connections;
    ConnectionPool pool;

    std::deque<OutstandingCallPtr> bufferingCalls;
    std::map<boost::int64_t, OutstandingCallPtr> outStandingCalls;
};

}
}

#endif //#if !defined(CETTY_SERVICE_CLIENTDISPATCHER_H)

// Local Variables:
// mode: c++
// End:
