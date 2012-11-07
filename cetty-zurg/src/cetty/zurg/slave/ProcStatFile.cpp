/*
 * ProcStatFile.cpp
 *
 *  Created on: 2012-11-1
 *      Author: chenhl
 */

#include <cetty/zurg/slave/ProcStatFile.h>
#include <cetty/util/SmallFile.h>
#include <cetty/util/StringUtil.h>

namespace cetty {
namespace zurg {
namespace slave {

using namespace cetty::util;

ProcStatFile::ProcStatFile(int pid)
    :valid_(false),
     error_(0),
     ppid_(0),
     startTime_(0) {

    char filename[64];
    ::snprintf(filename, sizeof filename, "/proc/%d/stat", pid);
    SmallFile file(filename);

    if ((error_ = file.readToBuffer(NULL)) == 0) {
        valid_ = true;
        parse(file.buffer());
    }
}

void ProcStatFile::parse(const char* buffer) {
    // do_task_stat() in fs/proc/array.c
    // pid (cmd) S ppid pgid sid
    const char* p = buffer;
    p = strchr(p, ')');

    for (int i = 0; i < 20 && (p); ++i) {
        p = strchr(p, ' ');
        if (p) {
            if (i == 1) ppid_ = atoi(p);
            ++p;
        }
    }
    if (p) startTime_ = StringUtil::atoi(p);
}

}
}
}


