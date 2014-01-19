/**************************************************************************
 *
 * Copyright 2011 Zack Rusin
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/


#include "trace_file.hpp"


#include <assert.h>
#include <string.h>

#include <zlib.h>

// for lseek
#ifdef _WIN32
#include <io.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include "os.hpp"

#include <iostream>


using namespace trace;


class ZLibFile : public File {
public:
    ZLibFile(const std::string &filename = std::string(),
             File::Mode mode = File::Read) {}
    virtual ~ZLibFile() {}


    virtual bool supportsOffsets() const { return false; }
    virtual File::Offset currentOffset() { return File::Offset(0); }
protected:
    virtual bool rawOpen(const std::string &filename, File::Mode mode) { return false; }
    virtual bool rawWrite(const void *buffer, size_t length) { return false; }
    virtual size_t rawRead(void *buffer, size_t length) { return 0; }
    virtual int rawGetc() { return 0; }
    virtual void rawClose() {}
    virtual void rawFlush() {}
    virtual bool rawSkip(size_t length) { return false; }
    virtual int  rawPercentRead() { return 0; }
private:
};

