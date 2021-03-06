#if !defined(CETTY_GEARMAN_PROTOBUF_GEARMANPROTOBUFCLIENTFILTER_H)
#define CETTY_GEARMAN_PROTOBUF_GEARMANPROTOBUFCLIENTFILTER_H

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
#include <cetty/service/ClientServiceFilter.h>
#include <cetty/protobuf/service/ProtobufServiceMessagePtr.h>
#include <cetty/gearman/protocol/GearmanMessagePtr.h>

namespace cetty {
namespace gearman {
namespace protobuf {

using namespace cetty::service;
using namespace cetty::channel;
using namespace cetty::protobuf::service;
using namespace cetty::gearman::protocol;

class GearmanProtobufClientFilter : private boost::noncopyable {
public:
    typedef ClientServiceFilter<GearmanProtobufClientFilter,
            ProtobufServiceMessagePtr,
            GearmanMessagePtr,
            GearmanMessagePtr,
            ProtobufServiceMessagePtr> Filter;

    typedef Filter::Context Context;
    typedef Filter::Handler Handler;
    typedef Filter::HandlerPtr HandlerPtr;

public:
    GearmanProtobufClientFilter();
    ~GearmanProtobufClientFilter();

    void registerTo(Context& ctx) {
        filter_.registerTo(ctx);
    }

    bool background() const {
        return background_;
    }

    void setBackground(bool background) {
        background_ = background;
    }

    const std::string& uniqueKey() const {
        return uniqueKey_;
    }

    // uniqueId = key + id
    // if the key is empty, will not use the unique id
    void setUniqueKey(const std::string& key) {
        uniqueKey_ = key;
    }

private:
    GearmanMessagePtr filterRequest(ChannelHandlerContext& ctx,
                                    const ProtobufServiceMessagePtr& req);

    ProtobufServiceMessagePtr filterResponse(ChannelHandlerContext& ctx,
            const ProtobufServiceMessagePtr& req,
            const GearmanMessagePtr& rep);

    const std::string& getUniqueId(int64_t id);

private:
    bool background_;
    std::string uniqueKey_;
    std::string uniqueId_;

    Filter filter_;
};

}
}
}

#endif //#if !defined(CETTY_GEARMAN_PROTOBUF_GEARMANPROTOBUFCLIENTFILTER_H)

// Local Variables:
// mode: c++
// End:
