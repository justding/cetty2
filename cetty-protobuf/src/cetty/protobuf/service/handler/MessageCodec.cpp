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

#include <cetty/protobuf/service/handler/MessageCodec.h>

#include <boost/assert.hpp>
#include <google/protobuf/message.h>

#include <cetty/buffer/Unpooled.h>
#include <cetty/buffer/ChannelBuffer.h>
#include <cetty/buffer/ChannelBufferUtil.h>
#include <cetty/protobuf/service/service.pb.h>
#include <cetty/protobuf/service/ProtobufServiceMessage.h>
#include <cetty/protobuf/service/ProtobufServiceRegister.h>
#include <cetty/protobuf/service/handler/ProtobufCodec.h>

namespace cetty {
namespace protobuf {
namespace service {
namespace handler {

enum FieldNumber {
    FIELD_NUM_TYPE              = 1,
    FIELD_NUM_ID                = 2,
    FIELD_NUM_SERVICE           = 3,
    FIELD_NUM_METHOD            = 4,
    FIELD_NUM_ERROR             = 5,
    FIELD_NUM_REQUEST_ENCODING  = 6,
    FIELD_NUM_RESPONSE_ENCODING = 7,
    FIELD_NUM_REQUEST           = 8,
    FIELD_NUM_RESPONSE          = 9
};

int MessageCodec::decodePayload(const ChannelBufferPtr& buffer,
                                const ProtobufServiceMessagePtr& message) {
    const google::protobuf::Message* prototype = NULL;

    const ServiceMessage& serviceMessage = message->serviceMessage();
    const std::string& service = serviceMessage.service();
    const std::string& method = serviceMessage.method();

    if (serviceMessage.type() == MSG_REQUEST) {
        prototype = ProtobufServiceRegister::instance().getRequestPrototype(
                        service,
                        method);
    }
    else if (serviceMessage.type() == MSG_RESPONSE) {
        prototype = ProtobufServiceRegister::instance().getResponsePrototype(
                        service,
                        method);
    }

    if (!prototype) {
        LOG_WARN << "parse the message of failed, unknown service or method.";
        return -1;
    }

    google::protobuf::Message* payload = prototype->New();
    StringPiece arry;
    buffer->readableBytes(&arry);

    if (payload && payload->ParseFromArray(arry.data(), arry.length())) {
        buffer->offsetReaderIndex(arry.length());
        message->setPayload(payload);
        return 0;
    }
    else {
        if (payload) {
            delete payload;
        }

        buffer->offsetReaderIndex(arry.length());

        LOG_WARN << "parse the message of service: " << service
                 << ", method " << method
                 << ", type " << serviceMessage.type()
                 << " failed.";

        return -1;
    }
}

//if successful then return 0,if type is wrong return -1;
int MessageCodec::decodeMessage(const ChannelBufferPtr& buffer,
                                const ProtobufServiceMessagePtr& message) {
    ServiceMessage* serviceMessage = message->mutableServiceMessage();

    // for "RPC0"
    buffer->offsetReaderIndex(4);

    while (buffer->readable()) {
        int wireType = 0;
        int fieldNum = 0;
        int fieldLength = 0;
        int64_t type = 0;
        int64_t id = 0;
        int64_t error = 0;

        if (ProtobufCodec::decodeField(buffer, &wireType, &fieldNum, &fieldLength)) {
            switch (fieldNum) {
                //involved varint
            case FIELD_NUM_TYPE:
                type = ProtobufCodec::decodeVarint(buffer);

                if (type != MSG_REQUEST && type != MSG_RESPONSE && type != MSG_ERROR) {
                    return -1;
                }

                serviceMessage->set_type(static_cast<MessageType>(type));
                break;

            case FIELD_NUM_ID:
                id = ProtobufCodec::decodeFixed64(buffer);
                id = ChannelBufferUtil::swapLong(id);
                serviceMessage->set_id(id);
                break;

            case FIELD_NUM_SERVICE:
                buffer->readBytes(serviceMessage->mutable_service(), fieldLength);
                break;

            case FIELD_NUM_METHOD:
                buffer->readBytes(serviceMessage->mutable_method(), fieldLength);
                break;

            case FIELD_NUM_ERROR:
                error = ProtobufCodec::decodeVarint(buffer);
                serviceMessage->set_error(static_cast<ErrorCode>(error));
                break;

            case FIELD_NUM_REQUEST:
                return decodePayload(buffer, message);

            case FIELD_NUM_RESPONSE:
                return decodePayload(buffer, message);

            default:
                break;
            }
        }
    }

    return 0;
}

ProtobufServiceMessagePtr MessageCodec::decode(ChannelHandlerContext& ctx,
        const ChannelBufferPtr& msg) {

    if (msg) {
        ProtobufServiceMessagePtr message(new ProtobufServiceMessage);

        if (!decodeMessage(msg, message)) {
            return message;
        }
        else {
            LOG_WARN << "decode Message failed.";
        }
    }
    else {
        LOG_WARN << "input channel buffer empty, decode failed.";
    }

    return ProtobufServiceMessagePtr();
}

ChannelBufferPtr MessageCodec::encode(ChannelHandlerContext& ctx,
                                      const ProtobufServiceMessagePtr& msg) {
    if (msg) {
        int msgSize = msg->messageSize();
        ChannelBufferPtr buffer = Unpooled::buffer(msgSize + 32, 4);
        encodeMessage(msg, buffer);
        return buffer;
    }
    else {
        return ChannelBufferPtr();
    }
}

void MessageCodec::encodeMessage(const ProtobufServiceMessagePtr& message,
                                 const ChannelBufferPtr& buffer) {

    const ServiceMessage& serviceMessage = message->serviceMessage();
    BOOST_ASSERT(!serviceMessage.has_request()
                 && !serviceMessage.has_response()
                 && "Can not contain the response or request payload in service message");

    buffer->writeBytes("RPC0");

    encodeProtobuf(serviceMessage, buffer);

    if (serviceMessage.type() == MSG_REQUEST) {
        int payloadSize = message->payload()->GetCachedSize();
        ProtobufCodec::encodeTag(buffer,
                                 FIELD_NUM_REQUEST,
                                 ProtobufCodec::WIRETYPE_LENGTH_DELIMITED);
        ProtobufCodec::encodeVarint(buffer, payloadSize);

        encodeProtobuf(*message->payload(), buffer);
    }
    else if (serviceMessage.type() == MSG_RESPONSE) {
        int payloadSize = message->payload()->GetCachedSize();
        ProtobufCodec::encodeTag(buffer,
                                 FIELD_NUM_RESPONSE,
                                 ProtobufCodec::WIRETYPE_LENGTH_DELIMITED);
        ProtobufCodec::encodeVarint(buffer, payloadSize);

        encodeProtobuf(*message->payload(), buffer);
    }
}

void MessageCodec::encodeProtobuf(const google::protobuf::Message& message,
                                  const ChannelBufferPtr& buffer) {
    int length;
    char* bytes = buffer->writableBytes(&length);
    int messageSize = message.GetCachedSize();

    if (length < messageSize) {
        buffer->ensureWritableBytes(messageSize);
        bytes = buffer->writableBytes(&length);
    }

    message.SerializeWithCachedSizesToArray(
        reinterpret_cast<google::protobuf::uint8*>(bytes));

    buffer->offsetWriterIndex(messageSize);
}

int MessageCodec::decodeMessageHead(const ChannelBufferPtr& buffer,
                                    ServiceMessage* message) {
    BOOST_ASSERT(message);
    // for "RPC0"
    buffer->offsetReaderIndex(4);

    while (buffer->readable()) {
        int wireType = 0;
        int fieldNum = 0;
        int fieldLength = 0;
        int64_t type = 0;
        int64_t id = 0;
        int64_t error = 0;

        if (ProtobufCodec::decodeField(buffer, &wireType, &fieldNum, &fieldLength)) {
            switch (fieldNum) {
                //involved varint
            case FIELD_NUM_TYPE:
                type = ProtobufCodec::decodeVarint(buffer);

                if (type != MSG_REQUEST && type != MSG_RESPONSE && type != MSG_ERROR) {
                    return -1;
                }

                message->set_type(static_cast<MessageType>(type));
                break;

            case FIELD_NUM_ID:
                id = ProtobufCodec::decodeFixed64(buffer);
                id = ChannelBufferUtil::swapLong(id);
                message->set_id(id);
                break;

            case FIELD_NUM_SERVICE:
                buffer->readBytes(message->mutable_service(), fieldLength);
                break;

            case FIELD_NUM_METHOD:
                buffer->readBytes(message->mutable_method(), fieldLength);
                break;

            case FIELD_NUM_ERROR:
                error = ProtobufCodec::decodeVarint(buffer);
                message->set_error(static_cast<ErrorCode>(error));
                break;

            case FIELD_NUM_REQUEST:
            case FIELD_NUM_RESPONSE:
                buffer->clear();
                break;

            default:
                break;
            }
        }
    }

    return 0;
}

}
}
}
}

