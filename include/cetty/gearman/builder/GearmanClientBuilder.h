#if !defined(CETTY_GEARMAN_BUILDER_GEARMANCLIENTBUILDER_H)
#define CETTY_GEARMAN_BUILDER_GEARMANCLIENTBUILDER_H

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

#include <cetty/service/builder/ClientBuilder.h>
#include <cetty/gearman/GearmanClientPtr.h>
#include <cetty/gearman/protocol/GearmanMessagePtr.h>

namespace cetty {
namespace gearman {
namespace builder {

using namespace cetty::service;
using namespace cetty::service::builder;
using namespace cetty::gearman::protocol;

class GearmanClientBuilder : private boost::noncopyable {
public:
    typedef ClientBuilder<GearmanMessagePtr, GearmanMessagePtr> Builder;

public:
    GearmanClientBuilder();
    GearmanClientBuilder(int threadCnt);
    GearmanClientBuilder(const EventLoopPtr& eventLoop);
    GearmanClientBuilder(const EventLoopPoolPtr& eventLoopPool);

    GearmanClientBuilder& addConnection(const std::string& host, int port);

    GearmanClientPtr build();

private:
    void init();

private:
    Builder builder_;
};

}
}
}

#endif //#if !defined(CETTY_GEARMAN_BUILDER_GEARMANCLIENTBUILDER_H)

// Local Variables:
// mode: c++
// End:
