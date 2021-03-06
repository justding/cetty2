
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

#include <cetty/channel/EventLoop.h>
#include <cetty/channel/EventLoopPool.h>

namespace cetty {
namespace channel {

EventLoop::EventLoop(const EventLoopPoolPtr& pool)
    : pool_(pool) {
}

EventLoop::~EventLoop() {
}

void EventLoop::stop() {
    pool_.reset();
}

}
}
