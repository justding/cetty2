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

#include <cetty/handler/codec/http/QueryStringEncoder.h>

namespace cetty {
namespace handler {
namespace codec {
namespace http {
QueryStringEncoder::QueryStringEncoder(const std::string& uri) : uri(uri) {

}

std::string QueryStringEncoder::toString() const {
    if (params.empty()) {
        return uri;
    }
    else {
        std::string tmpStr;
        std::string str(uri);
        str.append("?");

        int i = 0;
        int size = params.size();
        NameValueCollection::ConstIterator itr = params.begin();
        NameValueCollection::ConstIterator end = params.end();
        for (; itr != end; ++itr) {
            encodeComponent(itr->first, tmpStr, str);
            str.append("=");
            encodeComponent(itr->second, tmpStr, str);

            if (i + 1 != size) {
                str.append("&");
            }

            ++i;
        }

        return str;
    }
}

void QueryStringEncoder::encodeComponent(const std::string& s,
        std::string& tmp,
        std::string& output) const {
    static std::string RESERVED;

    tmp.clear();
    URI::encode(s, RESERVED, tmp);
    output.append(tmp);
}

}
}
}
}
