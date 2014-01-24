/* NOTE: Do not edit this file, it is generated by a script:
   Export.py --api gl 4.4 --api wgl 4.4 --api glx 4.4 --api cgl 1.4 --api egl 1.0 --outdir .
*/

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

/*
  Intended formatting conventions:
  $ astyle --style=allman --indent=spaces=2 --indent-switches
*/

#ifndef __REGAL_CONTEXT_H__
#define __REGAL_CONTEXT_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalThread.h"
#include "RegalPrivate.h"
#include "RegalContextInfo.h"
#include "RegalLayer.h"
#include "RegalScopedPtr.h"
#include "RegalSharedList.h"

#if REGAL_SYS_PPAPI
#define __gl2_h_  // HACK - revisit
#include <ppapi/c/pp_resource.h>
#include <ppapi/c/ppb_opengles2.h>
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct EmuInfo;
struct DebugInfo;

struct None;
#if REGAL_EMULATION
struct Obj;
struct Hint;
struct Ppa;
struct Ppca;
struct Bin;
struct Xfer;
struct TexSto;
struct BaseVertex;
struct Rect;
struct Iff;
struct Quads;
struct So;
struct Dsa;
struct Vao;
struct TexC;
struct Filt;
#endif

struct RegalContext
{
  RegalContext();
  ~RegalContext();

  void Init();
  void Cleanup();

  // If profile is forced at build-time, no need to check runtime flag

  inline bool isES1()    const { RegalAssert(info); return REGAL_SYS_ES1 && ( REGAL_FORCE_ES1_PROFILE  || info->es1  ); }
  inline bool isES2()    const { RegalAssert(info); return REGAL_SYS_ES2 && ( REGAL_FORCE_ES2_PROFILE  || info->es2  ); }
  inline bool isCore()   const { RegalAssert(info); return REGAL_SYS_GL  && ( REGAL_FORCE_CORE_PROFILE || info->core ); }
  inline bool isCompat() const { RegalAssert(info); return REGAL_SYS_GL  &&                               info->compat; }

  bool                    initialized;
  Dispatch::GL            dispatchGL;
  scoped_ptr<DebugInfo>   dbg;
  scoped_ptr<ContextInfo> info;
  scoped_ptr<EmuInfo>     emuInfo;

  std::vector<Layer *>    layer;

  #if REGAL_SYS_PPAPI
  PPB_OpenGLES2      *ppapiES2;
  PP_Resource         ppapiResource;
  #endif

  RegalSystemContext  sysCtx;
  Thread::Thread      thread;

  #if REGAL_SYS_X11
  Display            *x11Display;
  #endif

  #if REGAL_SYS_GLX
  GLXDrawable         x11Drawable;
  #endif

  #if REGAL_SYS_WGL
  HDC                 hdc;
  HGLRC               hglrc;
  #endif

  GLLOGPROCREGAL      logCallback;

  //
  // Regal context sharing
  //

  shared_list<RegalContext *> shareGroup;

  // Query that any of the contexts in the share
  // group are already initialized

  bool groupInitialized() const;

  // The http and perhaps other threads need to be able brief, temporary access the context.
  // parkContext() makes the calling thread release the context
  // unparkContext() makes it current to the calling thread

  struct ParkProcs {
    template <typename T>
    void init( T & dt ) {
      #if REGAL_SYS_OSX
        CGLSetCurrentContext       = dispatchGlobal.CGLSetCurrentContext;
        CGLSetCurrentContext_layer = dispatchGlobal.CGLSetCurrentContext_layer;
      #elif REGAL_SYS_EGL
        eglMakeCurrent       = dispatchGlobal.eglMakeCurrent;
        eglMakeCurrent_layer = dispatchGlobal.eglMakeCurrent_layer;
      #elif REGAL_SYS_GLX
        glXMakeCurrent       = dispatchGlobal.glXMakeCurrent;
        glXMakeCurrent_layer = dispatchGlobal.glXMakeCurrent_layer;
      #elif REGAL_SYS_WGL
        wglMakeCurrent       = dispatchGlobal.wglMakeCurrent;
        wglMakeCurrent_layer = dispatchGlobal.wglMakeCurrent_layer;
      #else
        # error "Implement me!"
      #endif
    }
    #if REGAL_SYS_OSX
      REGALCGLSETCURRENTCONTEXTPROC CGLSetCurrentContext;
      Layer * CGLSetCurrentContext_layer;
    #elif REGAL_SYS_EGL
      REGALEGLMAKECURRENTPROC eglMakeCurrent;
      Layer * eglMakeCurrent_layer;
    #elif REGAL_SYS_GLX
      REGALGLXMAKECURRENTPROC glXMakeCurrent;
      Layer * glXMakeCurrent_layer;
    #elif REGAL_SYS_WGL
      REGALWGLMAKECURRENTPROC wglMakeCurrent;
       wglMakeCurrent;
    #else
      # error "Implement me!"
    #endif
  };
  void parkContext( ParkProcs & pp );
  void unparkContext( ParkProcs & pp );

  // State tracked via EmuContextState.py / Regal.cpp

  size_t              depthBeginEnd;   // Normally zero or one
  size_t              depthPushMatrix; //
  size_t              depthPushAttrib; //
  size_t              depthNewList;    //
};

REGAL_NAMESPACE_END

#endif // __REGAL_CONTEXT_H__
