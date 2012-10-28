#if !defined(CETTY_CHANNEL_CHANNELINBOUNDINVOKER_H)
#define CETTY_CHANNEL_CHANNELINBOUNDINVOKER_H

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

#include <boost/any.hpp>

namespace cetty {
namespace channel {

class ChannelException;

class ChannelInboundInvoker {
public:
    virtual ~ChannelInboundInvoker() {}

    /**
     *
     */
    virtual void fireChannelCreated() = 0;

    /**
     *
     */
    virtual void fireChannelActive() = 0;

    /**
     *
     */
    virtual void fireChannelInactive() = 0;

    /**
     *
     */
    virtual void fireExceptionCaught(const ChannelException& cause) = 0;

    /**
     *
     */
    virtual void fireUserEventTriggered(const boost::any& event) = 0;

    /**
     *
     */
    virtual void fireMessageUpdated() = 0;

    /**
     *
     */
    virtual void fireWriteCompleted() = 0;
};

}
}

#endif //#if !defined(CETTY_CHANNEL_CHANNELINBOUNDINVOKER_H)

// Local Variables:
// mode: c++
// End:
