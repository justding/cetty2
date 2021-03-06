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

#include <gtest/gtest.h>
#include <boost/date_time.hpp>
#include <cetty/handler/codec/http/HttpHeaderDateFormat.h>

using namespace boost::posix_time;
using namespace cetty::handler::codec::http;

/**
 * This date is set at "06 Nov 1994 08:49:37 GMT" (same used in example in
 * RFC documentation)
 * <p>
 * http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html
 */
static const ptime DATE = from_iso_string("19941106T084937");

TEST(HttpHeaderDateFormatTest, testParse) {
    {
        final Date parsed = format.parse("Sun, 6 Nov 1994 08:49:37 GMT");
        Assert.assertNotNull(parsed);
        Assert.assertEquals(DATE, parsed);
    }
    {
        final Date parsed = format.parse("Sun, 06 Nov 1994 08:49:37 GMT");
        Assert.assertNotNull(parsed);
        Assert.assertEquals(DATE, parsed);
    }
    {
        final Date parsed = format.parse("Sunday, 06-Nov-94 08:49:37 GMT");
        Assert.assertNotNull(parsed);
        Assert.assertEquals(DATE, parsed);
    }
    {
        final Date parsed = format.parse("Sunday, 6-Nov-94 08:49:37 GMT");
        Assert.assertNotNull(parsed);
        Assert.assertEquals(DATE, parsed);
    }
}

TEST(HttpHeaderDateFormatTest, testFormat) {
    std::string formatted = HttpHeaderDateFormat.defaultFormat.format(DATE);
    ASSERT_STREQ("Sun, 06 Nov 1994 08:49:37 GMT", formatted.c_str());
}
