/*
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

#include <cetty/handler/codec/http/HttpRequestDecoder.h>

#include <cetty/util/StringPiece.h>

namespace cetty {
namespace handler {
namespace codec {
namespace http {

void HttpRequestDecoder::registerTo(Context& ctx) {
    decoder_.registerTo(ctx);
    ctx.setExceptionCallback(boost::bind(
                                 &HttpRequestDecoder::exceptionCaught,
                                 this,
                                 _1,
                                 _2));
}

void HttpRequestDecoder::init() {
    decoder_.setInitialState(requestDecoder_.initialState());
    decoder_.setDecoder(boost::bind(&HttpPackageDecoder::decode,
                                    &requestDecoder_,
                                    _1,
                                    _2,
                                    _3));

    requestDecoder_.setCheckPointInvoker(decoder_.checkPointInvoker());
    requestDecoder_.setHttpPackageCreator(boost::bind(
            &HttpRequestCreator::create,
            &requestCreator_,
            _1,
            _2,
            _3));
}

void HttpRequestDecoder::exceptionCaught(ChannelHandlerContext& ctx,
        const ChannelException& cause) {

}

}
}
}
}
