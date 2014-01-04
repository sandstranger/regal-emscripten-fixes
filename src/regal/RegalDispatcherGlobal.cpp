/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
  Copyright (c) 2012-2013 Google Inc.
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

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <algorithm>

#include "RegalConfig.h"
#include "RegalDispatcherGlobal.h"

using namespace ::std;

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

Dispatch::Global dispatchGlobal;

void InitDispatchGlobal() {
  
  // initialize in reverse order, starting with the driver

  Loader::Init( dispatchGlobal );     // CGL/GLX/WGL/EGL loader

#if 0 && REGAL_SYS_EGL && REGAL_STATIC_EGL
  InitDispatchTableStaticEGL( dispatchGlobal );
#endif

#if REGAL_LOG
  InitDispatchTableGlobalLog( dispatchGlobal );
#endif

#if REGAL_SYS_GLX && !REGAL_SYS_X11
  InitDispatchTableGlobalGLX( dispatchGlobal );
#endif
  

#if REGAL_HTTP
  InitDispatchTableGlobalHttp( dispatchGlobal );
#endif
  
  
#if REGAL_TRACE
  {
    // have to check this early for the global dispatches, otherwise we'd use Config
    getEnv( "REGAL_TRACE", Config::enableTrace);
    InitDispatchTableGlobalTrace( dispatchGlobal );
  }
#endif


}


REGAL_NAMESPACE_END
