#if !defined(CETTY_CHANNEL_CHANNELMESSAGECONTAINER_H)
#define CETTY_CHANNEL_CHANNELMESSAGECONTAINER_H

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

#include <deque>
#include <boost/assert.hpp>
#include <cetty/channel/VoidMessage.h>

#include <cetty/buffer/Unpooled.h>
#include <cetty/buffer/ChannelBuffer.h>

namespace cetty {
namespace channel {

using namespace cetty::buffer;

enum IsStreamMessage {
    MESSAGE_STREAM,
    MESSAGE_BLOCK
};

template<class T, int MessageType = MESSAGE_BLOCK>
class ChannelMessageContainer {
public:
    void addMessage(const T& message);
    void setEventLoop(const EventLoopPtr& eventLoop);
};

template<>
class ChannelMessageContainer<VoidMessage, MESSAGE_STREAM> {
public:
    void addMessage(const VoidMessage& message) {
        BOOST_ASSERT(false && "void type, should not addMessage");
    }

    void setEventLoop(const EventLoopPtr& eventLoop) {
        BOOST_ASSERT(false && "void type, should not setEventLoop");
    }
};

template<>
class ChannelMessageContainer<VoidMessage, MESSAGE_BLOCK> {
public:
    void addMessage(const VoidMessage& message) {
        BOOST_ASSERT(false && "void type, should not addMessage");
    }

    void setEventLoop(const EventLoopPtr& eventLoop) {
        BOOST_ASSERT(false && "void type, should not setEventLoop");
    }
};

template<>
class ChannelMessageContainer<ChannelBufferPtr, MESSAGE_STREAM> {
public:
    typedef ChannelMessageContainer<ChannelBufferPtr, MESSAGE_STREAM> Container;

public:
    ChannelMessageContainer()
        : accumulated_(false) {
    }

    ChannelMessageContainer(bool accumulatedBuffer)
        : accumulated_(accumulatedBuffer) {
        if (accumulated_) {
            buffer_ = Unpooled::buffer(16 * 1024);
        }
    }

    void setEventLoop(const EventLoopPtr& eventLoop) {
        eventLoop_ = eventLoop;
    }

    bool accumulated() const {
        return accumulated_;
    }

    void addMessage(const ChannelBufferPtr& message) {
        if (eventLoop_->inLoopThread()) {
            if (accumulated_) {
                buffer_->writeBytes(message);
            }
            else {
                buffer_ = message;
            }
        }
        else {
            eventLoop_->post(boost::bind(&Container::addMessage,
                                         this,
                                         message->copy()));
        }
    }

    ChannelBufferPtr& getMessages() {
        return buffer_;
    }

    const ChannelBufferPtr& getMessages() const {
        return buffer_;
    }

private:
    bool accumulated_;
    ChannelBufferPtr buffer_;
    EventLoopPtr eventLoop_;
};

template<class T>
class ChannelMessageContainer<T, MESSAGE_BLOCK> {
public:
    typedef std::deque<T> MessageQueue;

public:
    ChannelMessageContainer() {}

    void setEventLoop(const EventLoopPtr& eventLoop) {
        eventLoop_ = eventLoop;
    }

    void addMessage(const T& message) {
        if (eventLoop_->inLoopThread()) {
            messageQueue_.push_back(message);
        }
        else {
            eventLoop_->post(boost::bind(&MessageQueue::push_back,
                                         &messageQueue_,
                                         boost::cref(message)));
        }
    }

    MessageQueue& getMessages() {
        return messageQueue_;
    }

    const MessageQueue& getMessages() const {
        return messageQueue_;
    }

private:
    EventLoopPtr eventLoop_;
    MessageQueue messageQueue_;
};

template<>
void ChannelMessageContainer<ChannelBufferPtr, MESSAGE_BLOCK>::addMessage(const ChannelBufferPtr& message) {
    if (eventLoop_->inLoopThread()) {
        messageQueue_.push_back(message);
    }
    else {
        eventLoop_->post(boost::bind(&ChannelMessageContainer<ChannelBufferPtr, MESSAGE_BLOCK>::addMessage,
                                     this,
                                     message->copy()));
    }
}

typedef ChannelMessageContainer<VoidMessage, MESSAGE_STREAM> VoidBufferContainer;
typedef ChannelMessageContainer<VoidMessage, MESSAGE_BLOCK>  VoidMessageContainer;
typedef ChannelMessageContainer<ChannelBufferPtr, MESSAGE_STREAM>   ChannelBufferContainer;

}
}

#endif //#if !defined(CETTY_CHANNEL_CHANNELMESSAGECONTAINER_H)

// Local Variables:
// mode: c++
// End:
