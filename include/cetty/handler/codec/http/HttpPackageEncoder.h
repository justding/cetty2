#if !defined(CETTY_HANDLER_CODEC_HTTP_HTTPMESSAGEENCODER_H)
#define CETTY_HANDLER_CODEC_HTTP_HTTPMESSAGEENCODER_H

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

#include <string>
#include <boost/function.hpp>
#include <cetty/buffer/ChannelBuffer.h>
#include <cetty/handler/codec/http/HttpHeaders.h>
#include <cetty/handler/codec/http/HttpPackage.h>
#include <cetty/handler/codec/http/HttpTransferEncoding.h>

namespace cetty {
namespace channel {
class ChannelHandlerContext;
}
}

namespace cetty {
namespace handler {
namespace codec {
namespace http {

using namespace cetty::channel;
using namespace cetty::buffer;
using namespace cetty::handler::codec;

class HttpHeader;

/**
 * Encodes an {@link HttpMessage} or an {@link HttpChunk} into
 * a {@link ChannelBuffer}.
 *
 * <h3>Extensibility</h3>
 *
 * Please note that this encoder is designed to be extended to implement
 * a protocol derived from HTTP, such as
 * <a href="http://en.wikipedia.org/wiki/Real_Time_Streaming_Protocol">RTSP</a> and
 * <a href="http://en.wikipedia.org/wiki/Internet_Content_Adaptation_Protocol">ICAP</a>.
 * To implement the encoder of such a derived protocol, extend this class and
 * implement all abstract methods properly.
 *
 *
 * @author Andy Taylor (andy.taylor@jboss.org)
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 * @author <a href="mailto:frankee.zhou@gmail.com">Frankee Zhou</a>
 *
 * @apiviz.landmark
 */

class HttpPackageEncoder : private boost::noncopyable {
public:
    typedef boost::function<void (HttpPackage const&, ChannelBufferPtr&)> InitialLineEncoder;

public:
    /**
     * Creates a new instance.
     */
    HttpPackageEncoder();
    ~HttpPackageEncoder();

    ChannelBufferPtr encode(ChannelHandlerContext& ctx,
                            const HttpPackage& msg,
                            const ChannelBufferPtr& out);

    void setInitialLineEncoder(const InitialLineEncoder& encoder) {
        initialLineEncoder_ = encoder;
    }

    static bool encodeResponseInitialLine(const HttpPackage& package,
                                          const ChannelBufferPtr& buf);

    static bool encodeRequestInitialLine(const HttpPackage& package,
                                         const ChannelBufferPtr& buf);

private:
    void encodeHeaders(ChannelBuffer& buf,
                       const HttpHeaders::ConstHeaderIterator& begin,
                       const HttpHeaders::ConstHeaderIterator& end);

    // header and value has been validated when added to the HttpHeader.
    void encodeHeader(ChannelBuffer& buf,
                      const std::string& header,
                      const std::string& value);

private:
    HttpTransferEncoding lastTE_;
    InitialLineEncoder initialLineEncoder_;
};


}
}
}
}

#endif //#if !defined(CETTY_HANDLER_CODEC_HTTP_HTTPMESSAGEENCODER_H)

// Local Variables:
// mode: c++
// End:
