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

#include <cetty/gearman/GearmanClientHandler.h>

#include <string>
#include <cetty/buffer/Unpooled.h>
#include <cetty/buffer/ChannelBufferUtil.h>
#include <cetty/logging/LoggerHelper.h>
#include <cetty/channel/ChannelHandlerContext.h>

#include <cetty/gearman/GearmanWorker.h>
#include <cetty/gearman/protocol/GearmanMessage.h>

namespace cetty {
namespace gearman {

using namespace cetty::channel;
using namespace cetty::buffer;
using namespace cetty::gearman::protocol;

GearmanClientHandler::GearmanClientHandler()
    : inboundTransfer_(NULL),
      inboundContainer_(NULL),
      outboundTransfer_(NULL),
      outboundContainer_(NULL) {
}

void GearmanClientHandler::registerTo(Context& ctx) {
    inboundTransfer_ = ctx.inboundTransfer();
    inboundContainer_ = ctx.inboundContainer();
    outboundTransfer_ = ctx.outboundTransfer();
    outboundContainer_ = ctx.outboundContainer();

    ctx.setChannelMessageUpdatedCallback(boost::bind(&GearmanClientHandler::messageReceived,
                                         this,
                                         _1));

    ctx.setFlushFunctor(boost::bind(&GearmanClientHandler::flush,
                                    this,
                                    _1,
                                    _2));
}

void GearmanClientHandler::submitJob(ChannelHandlerContext& ctx,
                                     const GearmanMessagePtr& msg) {
    outboundTransfer_->write(msg, ctx.newVoidFuture());
}

void GearmanClientHandler::messageReceived(ChannelHandlerContext& ctx) {
    std::string  data;

    InboundQueue& queue = inboundContainer_->getMessages();

    while (!queue.empty()) {
        GearmanMessagePtr& msg = queue.front();

        const std::vector<std::string>& params = msg->parameters();

        switch (msg->type()) {
        case GearmanMessage::JOB_CREATED:
            DLOG_DEBUG << "JOB_SUBMIT, job-handler:" << params.front();
            break;

        case GearmanMessage::WORK_STATUS:
            DLOG_DEBUG << "WORK_STATUS, "
                       << "job-handler:" << params[0] << ", "
                       << "complete numerator:" << params[1] << ", "
                       << "complete denominator:" << params[2];
            break;

            //use job handler to identify a job  define a map<��job��vector<��job handler> > for split
        case GearmanMessage::WORK_COMPLETE:
            DLOG_DEBUG << "WORK_COMPLETE, "
                       << "job-handler:" << params[0] << ", "
                       << "work dump data:" << ChannelBufferUtil::hexDump(msg->data());

            if (inboundTransfer_->unfoldAndAdd(msg)) {
                ctx.fireMessageUpdated();
            }

            break;

        case GearmanMessage::WORK_WARNING:
            DLOG_DEBUG << "WORK_WARNING, "
                       << "job-handler:" << params[0] << ", "
                       << "work dump data:" << ChannelBufferUtil::hexDump(msg->data());

            break;

        case GearmanMessage::WORK_FAIL:
            DLOG_DEBUG << "WORK_FAIL, " << "job-handler:" << params[0];
            break;

        case GearmanMessage::WORK_EXCEPTION:
            DLOG_DEBUG << "WORK_EXCEPTION, " << "job-handler:" << params[0];
            break;

        case GearmanMessage::WORK_DATA:
            DLOG_DEBUG << "WORK_DATA, "
                       << "job-handler:" << params[0] << ", "
                       << "work dump data:" << ChannelBufferUtil::hexDump(msg->data());

            break;

        case GearmanMessage::STATUS_RES:
            DLOG_DEBUG << "STATUS_RES, "
                       << "job-handler:" << params[0] << ", "
                       << "known status" << params[1] << ", "
                       << "running status" << params[2] << ", "
                       << "complete numerator:" << params[3] << ", "
                       << "complete denominator:" << params[4];
            break;

        case GearmanMessage::OPTION_RES:
            DLOG_DEBUG << "OPTION_RES, option:" << params.front();
            break;

        default:
            break;
        }

        queue.pop_front();
    }
}

void GearmanClientHandler::flush(ChannelHandlerContext& ctx,
                                 const ChannelFuturePtr& future) {
    OutboundQueue& queue = outboundContainer_->getMessages();

    while (!queue.empty()) {
        GearmanMessagePtr& msg = queue.front();

        if (msg) {
            outboundTransfer_->unfoldAndAdd(msg);
        }

        queue.pop_front();
    }

    ctx.flush();
}

}
}
