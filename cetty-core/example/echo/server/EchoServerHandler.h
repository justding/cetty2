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
/**
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
 * Distributed under under the Apache License, version 2.0 (the "License").
 */

#include <cetty/channel/Channel.h>
#include <cetty/channel/ChannelConfig.h>
#include <cetty/channel/VoidChannelFuture.h>
#include <cetty/channel/ChannelInboundBufferHandler.h>
#include <cetty/buffer/ChannelBuffer.h>

using namespace cetty::channel;
using namespace cetty::buffer;

/**
 * Handler implementation for the echo server.
 *
 * 
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 *
 * @author <a href="mailto:frankee.zhou@gmail.com">Frankee Zhou</a>
 */
class EchoServerHandler : private boost::noncopyable {
public:
    typedef ChannelInboudBufferHandler<EchoServerHandler>::Context Context;

    typedef ChannelInboudBufferHandler<EchoServerHandler>::OutboundTransfer OutboundTransfer;
    typedef ChannelInboudBufferHandler<EchoServerHandler>::InboundContainer InboundContainer;

public:
    EchoServerHandler()
        : transferredBytes_(0),
          transfer_(),
          container_() {
    }

    ~EchoServerHandler() {}

    void registerTo(Context& ctx) {
        transfer_ = ctx.outboundTransfer();
        container_ = ctx.inboundContainer();

        ctx.setChannelMessageUpdatedCallback(
            boost::bind(&EchoServerHandler::messageUpdated,
            this,
            _1));

        ctx.setChannelActiveCallback(
            boost::bind(&EchoServerHandler::channelActive,
            this,
            _1));
    }

    int getTransferredBytes() const {
        return transferredBytes_;
    }

    void channelActive(ChannelHandlerContext& ctx) {
        voidFuture = new VoidChannelFuture(ctx.channel());
    }

    void messageUpdated(ChannelHandlerContext& ctx) {
        const ChannelBufferPtr& buffer = container_->getMessages();

        if (buffer) {
            transfer_->write(buffer->readBytes(), voidFuture);
        }
    }

private:
    int transferredBytes_;

    OutboundTransfer* transfer_;
    InboundContainer* container_;

    ChannelFuturePtr voidFuture;
};
