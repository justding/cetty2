/*
 * Copyright 2010 Red Hat, Inc.
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

#include "cetty/channel/Channels.h"
#include "cetty/channel/ChannelPipelineFactory.h"
#include "cetty/handler/codec/http/HttpRequestDecoder.h"
#include "cetty/handler/codec/http/HttpResponseEncoder.h"
#include "cetty/handler/codec/http/HttpChunkAggregator.h"

#include "WebSocketServerHandler.h"

using namespace cetty::channel;
using namespace cetty::handler::codec::http;

/**
 * @author <a href="http://www.jboss.org/netty/">The Netty Project</a>
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 *
 * @version $Rev: 2080 $, $Date: 2010-01-26 18:04:19 +0900 (Tue, 26 Jan 2010) $
 */
class WebSocketServerPipelineFactory : public ChannelPipelineFactory {
public:
    WebSocketServerPipelineFactory() {}
	virtual ~WebSocketServerPipelineFactory() {}

    virtual ChannelPipeline* getPipeline() {
        // Create a default pipeline implementation.
		ChannelPipeline* pipeline = Channels::pipeline();

        pipeline->addLast("decoder", ChannelHandlerPtr(new HttpRequestDecoder()));
        pipeline->addLast("aggregator", ChannelHandlerPtr(new HttpChunkAggregator(65536)));
        pipeline->addLast("encoder", ChannelHandlerPtr(new HttpResponseEncoder()));
        pipeline->addLast("handler", ChannelHandlerPtr(new WebSocketServerHandler()));
        return pipeline;
    }
};
