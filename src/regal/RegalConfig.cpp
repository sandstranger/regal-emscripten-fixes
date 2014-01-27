/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <limits>

#include "RegalLog.h"
#include "RegalConfig.h"
#include "RegalSystem.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Config {

  ::std::string configFile("");  // Don't read/write configuration by default

  ::std::string loadGL  ("");    // Default to auto-detect
  ::std::string loadES2 ("");    // Default to auto-detect
//::std::string loadGLX ("");    // Default to auto-detect
//::std::string loadWGL ("");    // Default to auto-detect
  ::std::string loadEGL ("");    // Default to auto-detect

  bool forceES1Profile     = REGAL_FORCE_ES1_PROFILE;
  bool forceES2Profile     = REGAL_FORCE_ES2_PROFILE;
  bool forceCoreProfile    = REGAL_FORCE_CORE_PROFILE;
  bool sysES1              = REGAL_SYS_ES1;
  bool sysES2              = REGAL_SYS_ES2;
  bool sysGL               = REGAL_SYS_GL;
  bool sysGLX              = REGAL_SYS_GLX;
  bool sysEGL              = REGAL_SYS_EGL && !REGAL_SYS_GLX;

  bool          enableThreadLocking = REGAL_THREAD_LOCKING;

  void Init()
  {
    Internal("Config::Init","()");

#ifndef REGAL_NO_GETENV
    getEnv( "REGAL_LOAD_GL",  loadGL);
    getEnv( "REGAL_LOAD_ES2", loadES2);
//  getEnv( "REGAL_LOAD_GLX", loadGLX);
//  getEnv( "REGAL_LOAD_WGL", loadWGL);
    getEnv( "REGAL_LOAD_EGL", loadEGL);

    getEnv( "REGAL_SYS_ES2", sysES2, REGAL_SYS_ES2);
    getEnv( "REGAL_SYS_GL",  sysGL,  REGAL_SYS_GL);

    getEnv( "REGAL_FORCE_ES1_PROFILE",  forceES1Profile,  !REGAL_FORCE_ES1_PROFILE  );
    getEnv( "REGAL_FORCE_ES2_PROFILE",  forceES2Profile,  !REGAL_FORCE_ES2_PROFILE  );
    getEnv( "REGAL_FORCE_CORE_PROFILE", forceCoreProfile, !REGAL_FORCE_CORE_PROFILE );

    // With REGAL_SYS_GLX && REGAL_SYS_EGL
    // we infer each from other, if specified,
    // to behave as a toggle.

    const char *tmp;

#if REGAL_SYS_GLX
    tmp = getEnv( "REGAL_SYS_GLX" );
    if (tmp)
    {
      sysGLX = atoi(tmp)!=0;
#if REGAL_SYS_EGL
      sysEGL = !sysGLX;
#endif
    }
#endif

#if REGAL_SYS_EGL
    tmp = getEnv( "REGAL_SYS_EGL" );
    if (tmp)
    {
      sysEGL = atoi(tmp)!=0;
#if REGAL_SYS_GLX
      sysGLX = !sysEGL;
#endif
     }
#endif

    // Default to GLX, if necessary
    //
    // This situation can arise if REGAL_SYS_GLX and REGAL_SYS_EGL environment variables
    // are unset, or via JSON settings.

#if REGAL_SYS_GLX && REGAL_SYS_EGL
    if (sysGLX && sysEGL)
      sysEGL = false;
#endif

    // Default to GL, if necessary
    //
    // This situation can arise if REGAL_SYS_GL and REGAL_SYS_ES2 environment variables
    // are unset, or via JSON settings.

#if REGAL_SYS_GL && REGAL_SYS_ES2
    if (sysGL && sysES2)
      sysES2 = false;
#endif

#if REGAL_THREAD_LOCKING
    getEnv( "REGAL_THREAD_LOCKING", enableThreadLocking );
#else
    enableThreadLocking = false;
#endif

#endif

    // REGAL_NO_EMULATION is deprecated, use REGAL_EMULATION=0 instead.

#if REGAL_EMULATION && defined(REGAL_NO_EMULATION) && REGAL_NO_EMULATION
    enableEmulation = false;
#endif

#if REGAL_SYS_ES1
    Info("REGAL_FORCE_ES1_PROFILE   ", forceES1Profile     ? "enabled" : "disabled");
#endif

#if REGAL_SYS_ES2
    Info("REGAL_FORCE_ES2_PROFILE   ", forceES2Profile     ? "enabled" : "disabled");
#endif

    Info("REGAL_FORCE_CORE_PROFILE  ", forceCoreProfile    ? "enabled" : "disabled");

#if REGAL_SYS_ES1
    Info("REGAL_SYS_ES1             ", sysES1              ? "enabled" : "disabled");
#endif

#if REGAL_SYS_ES2
    Info("REGAL_SYS_ES2             ", sysES2              ? "enabled" : "disabled");
#endif

#if REGAL_SYS_GL
    Info("REGAL_SYS_GL              ", sysGL               ? "enabled" : "disabled");
#endif

#if REGAL_SYS_GLX
    Info("REGAL_SYS_GLX             ", sysGLX              ? "enabled" : "disabled");
#endif

#if REGAL_SYS_EGL
    Info("REGAL_SYS_EGL             ", sysEGL              ? "enabled" : "disabled");
#endif

    Info("REGAL_THREAD_LOCKING      ", enableThreadLocking ? "enabled" : "disabled");
  }

}

REGAL_NAMESPACE_END
