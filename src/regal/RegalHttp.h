/*
  Copyright (c) 2011 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "pch.h" /* For MS precompiled header support */

#ifndef __REGAL_HTTP_H__
#define __REGAL_HTTP_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalSystem.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

#ifndef REGAL_HTTP
#  if REGAL_WRANGLER
#    define REGAL_HTTP 0
#  else
#    define REGAL_HTTP 1
#  endif
#endif

// Http is disabled for NaCL and Emscripten, for now

#if REGAL_SYS_PPAPI || REGAL_SYS_EMSCRIPTEN
#undef REGAL_HTTP
#define REGAL_HTTP 0
#endif

#ifndef REGAL_HTTP_PORT
#define REGAL_HTTP_PORT 8080
#endif

namespace Http
{
  void Init();
  void Cleanup();

  void Start();
  void Stop();

  extern bool enabled;
  extern int  port;

}

REGAL_NAMESPACE_END

#endif

