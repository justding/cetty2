#if !defined(CETTY_CHANNEL_CHANNELOPTION_H)
#define CETTY_CHANNEL_CHANNELOPTION_H
/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
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

#include <map>
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include <cetty/util/Enum.h>

namespace cetty {
namespace channel {

template<typename T>
class ValueChecker : public boost::static_visitor<bool> {
public:
    bool operator()(const T& value) const {
        return true;
    }

    template<typename U>
    bool operator()(const U& value) const {
        return false;
    }
};

class ChannelOption : public cetty::util::Enum<ChannelOption> {
public:
    class Null {
    public:
        Null() {}
    };

    typedef boost::variant<Null, int, bool, std::string, std::vector<int> > Variant;
    typedef std::map<ChannelOption, Variant> Options;

public:
    /**
     *
     */
    static const Variant EMPTY_VALUE;

public:
    static const ChannelOption CO_CONNECT_TIMEOUT_MILLIS;
    static const ChannelOption CO_REUSE_CHILD;
    static const ChannelOption CO_RESERVED_CHILD_COUNT;
    static const ChannelOption CO_AUTO_READ;

    static const ChannelOption CO_SO_BROADCAST;
    static const ChannelOption CO_SO_KEEPALIVE;
    static const ChannelOption CO_SO_SNDBUF;
    static const ChannelOption CO_SO_RCVBUF;
    static const ChannelOption CO_SO_REUSEADDR;
    static const ChannelOption CO_SO_LINGER;
    static const ChannelOption CO_SO_BACKLOG;
    static const ChannelOption CO_SO_SNDLOWAT;
    static const ChannelOption CO_SO_RCVLOWAT;

    static const ChannelOption CO_SNDHIGHWAT;
    static const ChannelOption CO_RCVHIGHWAT;

    static const ChannelOption CO_IP_TOS;
    static const ChannelOption CO_IP_MULTICAST_ADDR;
    static const ChannelOption CO_IP_MULTICAST_IF;
    static const ChannelOption CO_IP_MULTICAST_TTL;
    static const ChannelOption CO_IP_MULTICAST_LOOP_DISABLED;

    static const ChannelOption CO_UDP_RECEIVE_PACKET_SIZE;

    static const ChannelOption CO_TCP_NODELAY;

    static const ChannelOption CO_SCTP_DISABLE_FRAGMENTS;
    static const ChannelOption CO_SCTP_EXPLICIT_COMPLETE;
    static const ChannelOption CO_SCTP_FRAGMENT_INTERLEAVE;
    static const ChannelOption CO_SCTP_INIT_MAXSTREAMS;
    static const ChannelOption CO_SCTP_NODELAY;
    static const ChannelOption CO_SCTP_PRIMARY_ADDR;
    static const ChannelOption CO_SCTP_SET_PEER_PRIMARY_ADDR;

    static const ValueChecker<int> INT_VALUE_CHECKER;
    static const ValueChecker<bool> BOOL_VALUE_CHECKER;
    static const ValueChecker<std::string> STRING_VALUE_CHECKER;
    static const ValueChecker<std::vector<int> > INT_VECTOR_VALUE_CHECKER;

public:
    ChannelOption(const ChannelOption& option);
    ChannelOption& operator=(const ChannelOption& option);

    bool validate(const Variant& value) const;

    /**
     * test the {@link ChannelOption::Variant} is empty.
     */
    static bool empty(const ChannelOption::Variant& option);

private:
    ChannelOption(int id,
                  const char* name,
                  const boost::static_visitor<bool>* checker);

private:
    const boost::static_visitor<bool>* checker_;
};

inline
ChannelOption& ChannelOption::operator=(const ChannelOption& option) {
    cetty::util::Enum<ChannelOption>::operator=(option);
    checker_ = option.checker_;
    return *this;
}

inline
ChannelOption::ChannelOption(const ChannelOption& option)
    : cetty::util::Enum<ChannelOption>(option),
      checker_(option.checker_) {
}

inline
bool ChannelOption::empty(const ChannelOption::Variant& option) {
    return option.which() == 0;
}

}
}

#endif //#if !defined(CETTY_CHANNEL_CHANNELOPTION_H)

// Local Variables:
// mode: c++
// End:
