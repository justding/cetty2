
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

#include <cetty/gearman/protobuf/GearmanProtobufWorkerFilter.h>

#include <string>

#include <cetty/buffer/Unpooled.h>
#include <cetty/gearman/protocol/GearmanMessage.h>
#include <cetty/gearman/protocol/commands/Worker.h>

#include <cetty/protobuf/service/ProtobufServiceMessage.h>
#include <cetty/protobuf/service/handler/MessageCodec.h>

namespace cetty {
namespace gearman {
namespace protobuf {

using namespace cetty::channel;
using namespace cetty::gearman::protocol;
using namespace cetty::protobuf::service;
using namespace cetty::protobuf::service::handler;

GearmanProtobufWorkerFilter::GearmanProtobufWorkerFilter() {
    filter_.setRequestFilter(boost::bind(
        &GearmanProtobufWorkerFilter::filterRequest,
        this,
        _1,
        _2));

    filter_.setResponseFilter(boost::bind(
        &GearmanProtobufWorkerFilter::filterResponse,
        this,
        _1,
        _2,
        _3,
        _4));
}

GearmanProtobufWorkerFilter::~GearmanProtobufWorkerFilter() {
}

ProtobufServiceMessagePtr GearmanProtobufWorkerFilter::filterRequest(
    ChannelHandlerContext& ctx,
    const GearmanMessagePtr& req) {
    ProtobufServiceMessagePtr protoMsg(new ProtobufServiceMessage);
    MessageCodec::decodeMessage(req->data(), protoMsg);
    return protoMsg;
}

GearmanMessagePtr GearmanProtobufWorkerFilter::filterResponse(
    ChannelHandlerContext& ctx,
    const GearmanMessagePtr& req,
    const ProtobufServiceMessagePtr& rep,
    const ChannelFuturePtr& future) {
    std::string jobHandle = req->parameters().front();
    ChannelBufferPtr buffer = Unpooled::buffer(rep->messageSize() + 8);

    //encode the protobufServiceMessage and set it to GearmanMessage
    MessageCodec::encodeMessage(rep, buffer);
    return commands::workCompleteMessage(jobHandle, buffer);
}

}
}
}
