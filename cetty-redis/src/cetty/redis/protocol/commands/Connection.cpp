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

#include <cetty/redis/protocol/commands/Connection.h>

namespace cetty {
namespace redis {
namespace protocol {
namespace commands {

static const std::string AUTH   = "AUTH";
static const std::string ECHO   = "ECHO";
static const std::string PING   = "PING";
static const std::string SELECT = "SELECT";
static const std::string QUIT   = "QUIT";

}
}
}
}
