#if !defined(CETTY_GEARMAN_PROTOBUF_GEARMANPROTOBUFWORKERFILTER_H)
#define CETTY_GEARMAN_PROTOBUF_GEARMANPROTOBUFWORKERFILTER_H

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
#include <cetty/service/ServerFilter.h>
#include <cetty/protobuf/service/ProtobufServiceMessagePtr.h>
#include <cetty/gearman/GearmanMessagePtr.h>

namespace cetty {
namespace gearman {
namespace protobuf {

using namespace cetty::channel;
using namespace cetty::protobuf::service;
using namespace cetty::gearman;

class GearmanProtobufWorkerFilter
    : public cetty::service::ServiceFilter<
    GearmanMessagePtr,
    GearmanMessagePtr,
    ProtobufServiceMessagePtr,
        ProtobufServiceMessagePtr > {

public:
    typedef boost::intrusive_ptr<GearmanProtobufWorkerFilter> GearmanProtobufWorkerFilterPtr;

public:
    GearmanProtobufWorkerFilter();
    ~GearmanProtobufWorkerFilter();

    virtual ChannelHandlerPtr clone();

    virtual std::string toString() const;

protected:
    virtual ProtobufServiceMessagePtr filterReq(const GearmanMessagePtr& req);

    virtual GearmanMessagePtr filterRep(const GearmanMessagePtr& req,
                                        const ProtobufServiceMessagePtr& rep);
};

}
}
}

#endif //#if !defined(CETTY_GEARMAN_PROTOBUF_GEARMANPROTOBUFWORKERFILTER_H)

// Local Variables:
// mode: c++
// End: