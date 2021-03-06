#if !defined(CETTY_CHANNEL_CHANNELPTR_H)
#define CETTY_CHANNEL_CHANNELPTR_H

/**
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

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace cetty { namespace channel { 

class Channel;
typedef boost::shared_ptr<Channel> ChannelPtr;
typedef boost::shared_ptr<Channel const> ConstChannelPtr;

typedef boost::weak_ptr<Channel> ChannelWeakPtr;
typedef boost::weak_ptr<Channel const> ConstChannelWeakPtr;

}}

#endif //#if !defined(CETTY_CHANNEL_CHANNELPTR_H)

// Local Variables:
// mode: c++
// End:
