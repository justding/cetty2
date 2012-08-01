/**
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
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

#include <cetty/handler/codec/LengthFieldPrepender.h>

#include <cetty/channel/Channel.h>
#include <cetty/channel/ChannelConfig.h>
#include <cetty/channel/ChannelHandlerContext.h>
#include <cetty/buffer/ChannelBuffer.h>
#include <cetty/buffer/ChannelBuffers.h>
#include <cetty/buffer/ChannelBufferFactory.h>
#include <cetty/util/StringUtil.h>

namespace cetty {
namespace handler {
namespace codec {

using namespace cetty::channel;
using namespace cetty::buffer;
using namespace cetty::util;

enum {
    HEADER_FIELD_NONE  = 0,
    HEADER_FIELD_FIRST = 1,
    HEADER_FIELD_LAST  = 2
};

ChannelBufferPtr LengthFieldPrepender::encode(ChannelHandlerContext& ctx,
        const ChannelBufferPtr& msg,
        const ChannelBufferPtr& out) {

    BOOST_ASSERT(msg);

    bool hasOutBuffer = !!out;
    int headerFieldLength = (int)header.size();
    int headerLength = lengthFieldLength + headerFieldLength;

    int msgLength = headerLength + msg->readableBytes() + checksumFieldLength;
    int contentLength = msgLength - (lengthFieldOffset + lengthFieldLength - lengthAdjustment);

    boost::uint32_t cs = 0;

    if (checksumFieldLength > 0) {
        Array arry;
        msg->readableBytes(&arry);
        BOOST_ASSERT(!arry.empty());

        if (checksumCalcOffset == headerLength) {
            cs = checksumFunction((const boost::uint8_t*)arry.data(), arry.length());
        }
        else {
            //TODO
        }
    }

    int headerPos = headerFieldLength ?
                    (lengthFieldOffset ? HEADER_FIELD_FIRST : HEADER_FIELD_LAST)
                        : HEADER_FIELD_NONE;

    if (hasOutBuffer) {
        return writeMessage(out, msg, contentLength, headerPos, cs);
    }
    else {
        if (msg->aheadWritableBytes() >= headerLength &&
                (checksumFieldLength ? msg->writableBytes() >= checksumFieldLength : true)) {
            preWriteHeader(msg, contentLength, headerPos);

            if (2 == checksumFieldLength) {
                msg->writeShort(cs);
            }
            else if (4 == checksumFieldLength) {
                msg->writeInt(cs);
            }

            return msg;
        }
        else {
            ChannelBufferPtr buffer = ChannelBuffers::buffer(msgLength);
            return writeMessage(buffer, msg, contentLength, headerPos, cs);
        }
    }
}

void LengthFieldPrepender::validateParameters() {
    if (lengthFieldLength != 1 && lengthFieldLength != 2 &&
            lengthFieldLength != 4 && lengthFieldLength != 8) {
        throw InvalidArgumentException(
            std::string("lengthFieldLength must be either 1, 2, 4, or 8: ") +
            Integer(lengthFieldLength).toString());
    }

    if (checksumFieldLength != 0 && checksumFieldLength != 2 && checksumFieldLength != 4) {
        throw InvalidArgumentException(
            std::string("checksumFieldLength must be either 2 or 4: ") +
            Integer(checksumFieldLength).toString());
    }

    int preHeaderFieldLength = (int)header.size();

    if (lengthFieldOffset != 0 && lengthFieldOffset != preHeaderFieldLength
            && lengthFieldOffset != (preHeaderFieldLength + lengthFieldLength)) {
        throw InvalidArgumentException(
            std::string("lengthFieldOffset must be either 0, preHeaderFieldLength, lengthFieldLength : ") +
            Integer(lengthFieldOffset).toString());
    }

    if (checksumFieldLength > 0) {//enable checksum
        if (!checksumFunction) {
            throw InvalidArgumentException(std::string("you must set the checksum function if you enable checksum."));
        }

        /*
        if (checksumFieldOffset != 0 && checksumFieldOffset != preHeaderFieldLength
            && checksumFieldOffset < (preHeaderFieldLength + lengthFieldLength)) {
                throw InvalidArgumentException(
                    std::string("lengthFieldOffset must be either 0, preHeaderFieldLength, lengthFieldLength : ") +
                    Integer(lengthFieldOffset).toString());
                    */
    }
}

LengthFieldPrepender::LengthFieldPrepender(int lengthFieldLength)
    : lengthFieldLength(lengthFieldLength),
      lengthFieldOffset(0),
      lengthAdjustment(0),
      checksumFieldLength(0),
      checksumCalcOffset(0) {
    validateParameters();
}

LengthFieldPrepender::LengthFieldPrepender(int lengthFieldLength,
        int lengthFieldOffset,
        const std::string& header)
    : lengthFieldLength(lengthFieldLength),
      lengthFieldOffset(lengthFieldOffset),
      lengthAdjustment(lengthFieldOffset ? 0 : header.size()),
      header(header),
      checksumFieldLength(0),
      checksumCalcOffset(0) {
    validateParameters();
}

LengthFieldPrepender::LengthFieldPrepender(int lengthFieldLength,
        const std::string& header)
    : lengthFieldLength(lengthFieldLength),
      lengthFieldOffset(header.size()),
      lengthAdjustment(0),
      header(header),
      checksumFieldLength(0),
      checksumCalcOffset(0) {
    validateParameters();
}

LengthFieldPrepender::LengthFieldPrepender(int lengthFieldLength,
        int lengthFieldOffset,
        int lengthAdjustment,
        const std::string& header)
    : lengthFieldLength(lengthFieldLength),
      lengthFieldOffset(lengthFieldOffset),
      lengthAdjustment(lengthAdjustment),
      header(header),
      checksumFieldLength(0),
      checksumCalcOffset(0) {
    validateParameters();
}

LengthFieldPrepender::LengthFieldPrepender(int lengthFieldLength,
        const std::string& header,
        int checksumFieldLength,
        const ChecksumFunction& checksum)
    : lengthFieldLength(lengthFieldLength),
      lengthFieldOffset(header.size()),
      lengthAdjustment(0),
      header(header),
      checksumFieldLength(checksumFieldLength),
      checksumCalcOffset(0),
      checksumFunction(checksumFunction) {
    validateParameters();
}

LengthFieldPrepender::LengthFieldPrepender(int lengthFieldLength,
        int lengthFieldOffset,
        int lengthAdjustment,
        int checksumFieldLength,
        const std::string& header,
        ChecksumFunction checksumFunction)
    : lengthFieldLength(lengthFieldLength),
      lengthFieldOffset(lengthFieldOffset),
      lengthAdjustment(lengthAdjustment),
      header(header),
      checksumFieldLength(checksumFieldLength),
      checksumCalcOffset(0),
      checksumFunction(checksumFunction) {
    validateParameters();
}

void LengthFieldPrepender::writeHeader(const ChannelBufferPtr& msg, int contentLength, int headerPos) {
    if (HEADER_FIELD_FIRST == headerPos) {
        msg->writeBytes(header);
    }

    switch (lengthFieldLength) {
    case 1:
        if (contentLength >= 256) {
            throw InvalidArgumentException(
                StringUtil::strprintf("length does not fit into a byte: %d",
                                      contentLength));
        }

        msg->writeByte(contentLength);
        break;

    case 2:
        if (contentLength >= 65536) {
            throw InvalidArgumentException(
                StringUtil::strprintf("length does not fit into a short integer: %d",
                                      contentLength));
        }

        msg->writeShort(contentLength);
        break;

    case 4:
        msg->writeInt(contentLength);
        break;

    case 8:
        msg->writeLong(contentLength);
        break;

    default:
        throw RuntimeException("should not reach here");
    }

    if (HEADER_FIELD_LAST == headerPos) {
        msg->writeBytes(header);
    }
}

void LengthFieldPrepender::preWriteHeader(const ChannelBufferPtr& msg, int contentLength, int headerPos) {
    if (HEADER_FIELD_LAST == headerPos) {
        msg->writeBytesAhead(header);
    }

    switch (lengthFieldLength) {
    case 1:
        if (contentLength >= 256) {
            throw InvalidArgumentException(
                StringUtil::strprintf("length does not fit into a byte: %d",
                                      contentLength));
        }

        msg->writeByteAhead(contentLength);
        break;

    case 2:
        if (contentLength >= 65536) {
            throw InvalidArgumentException(
                StringUtil::strprintf("length does not fit into a short integer: %d",
                                      contentLength));
        }

        msg->writeShortAhead(contentLength);
        break;

    case 4:
        msg->writeIntAhead(contentLength);
        break;

    case 8:
        msg->writeLongAhead(contentLength);
        break;

    default:
        throw RuntimeException("should not reach here");
    }

    if (HEADER_FIELD_FIRST == headerPos) {
        msg->writeBytesAhead(header);
    }
}

const ChannelBufferPtr& LengthFieldPrepender::writeMessage(const ChannelBufferPtr& out,
        const ChannelBufferPtr& msg,
        int contentLength,
        int headerPos,
        boost::uint32_t cs) {
    writeHeader(out, contentLength, headerPos);
    msg->readBytes(out);

    if (2 == checksumFieldLength) {
        out->writeShort(cs);
    }
    else if (4 == checksumFieldLength) {
        out->writeInt(cs);
    }

    return out;
}

}
}
}