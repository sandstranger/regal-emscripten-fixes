/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2012 Google Inc.
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

#ifndef __REGAL_CONFIG_H__
#define __REGAL_CONFIG_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <string>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Json { struct Output; }

namespace Config
{
  void Init();

  extern void writeJSON(Json::Output &jo);

  //

  extern ::std::string configFile;

  extern ::std::string loadGL;       // REGAL_LOAD_GL
  extern ::std::string loadES2;      // REGAL_LOAD_ES2
//extern ::std::string loadGLX;      // REGAL_LOAD_GLX
//extern ::std::string loadWGL;      // REGAL_LOAD_WGL
  extern ::std::string loadEGL;      // REGAL_LOAD_EGL

  //

  extern bool forceES1Profile;
  extern bool forceES2Profile;
  extern bool forceCoreProfile;

  //

  extern bool sysES1;
  extern bool sysES2;
  extern bool sysGL;

  // Use GLX or EGL, but not both

  extern bool sysGLX;
  extern bool sysEGL;

  // Thread locking

  extern bool          enableThreadLocking;
};

REGAL_NAMESPACE_END

#endif
