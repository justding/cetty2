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

#include <cetty/protobuf/service/handler/ProtobufServiceMessageDecoder.h>

#include <cetty/buffer/ChannelBuffer.h>
#include <cetty/protobuf/service/proto/service.pb.h>
#include <cetty/protobuf/service/ProtobufServiceRegister.h>
#include <cetty/protobuf/service/handler/ProtobufServiceMessage.h>

namespace cetty {
namespace protobuf {
namespace service {
namespace handler {

using namespace cetty::buffer;
using namespace cetty::channel;
using namespace cetty::protobuf::service;
using namespace cetty::protobuf::service::proto;

ChannelMessage ProtobufServiceMessageDecoder::decode(ChannelHandlerContext& ctx,
        const ChannelPtr& channel,
        const ChannelMessage& msg) {

    ChannelBufferPtr buffer = msg.smartPointer<ChannelBuffer>();

    if (buffer) {
        ProtobufServiceMessagePtr message(new ProtobufServiceMessage);
        ServiceMessage* rpc = message->mutableServiceMessage();

        if (!decode(buffer, rpc)) {
            const google::protobuf::Message* prototype = NULL;

            if (rpc->type() == REQUEST) {
                prototype = ProtobufServiceRegister::instance().getRequestPrototype(
                                rpc->service(), rpc->method());
            }
            else if (rpc->type() == RESPONSE) {
                prototype = ProtobufServiceRegister::instance().getResponsePrototype(
                                rpc->service(), rpc->method());
            }

            google::protobuf::Message* payload = prototype->New();
            Array arry;
            buffer->readableBytes(&arry);
            payload->ParseFromArray(arry.data(), arry.length());
            message->setPayload(payload);

            return ChannelMessage(message);
        }
        else {
            // error here
            return msg;
        }
    }
    else {
        return msg;
    }
}

int ProtobufServiceMessageDecoder::decode(const ChannelBufferPtr& buffer, ServiceMessage* message) {
    return 0;
}

}
}
}
}