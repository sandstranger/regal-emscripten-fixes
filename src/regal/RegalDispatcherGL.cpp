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
#include "RegalDispatcherGL.h"

using namespace ::std;

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

void InitDispatchGL() {
/*
  // start building the dispatch up from the bottom (driver)
  // each dispatch layer needs to:
  //     1. grab all the function pointers it needs to be able to call
  //     2. replace all the function pointers it wants to override

  RegalContext *ctx = REGAL_GET_CONTEXT();
  RegalAssert( ctx );

  if( ctx == NULL ) {
    return; // can't initialize without a context
  }
  
  Loader::Init( ctx );                    // Desktop/ES2.0 loader
  
#if 0  // Need to add back support for restricted driver subsets
#if REGAL_STATIC_ES2
  InitDispatchTableStaticES2(ctx);           // ES 2.0 functions only
#elif REGAL_SYS_PPAPI
  InitDispatchTablePpapi(ctx);               // ES 2.0 functions only
#endif
#endif
  
  
#if REGAL_LOG
  if( Config::enableLog ) {
    InitDispatchTableLog( ctx );
  }
#endif
  
#if REGAL_STATISTICS
  if( Config::enableStatistics ) {
    InitDispatchTableStatistics( ctx );
  }
#endif
  
#if REGAL_CODE
  if( Config::enableCode ) {
    InitDispatchTableCode( ctx );
  }
#endif
  
#if REGAL_CACHE
  if( true ) {
    InitDispatchTableCache( ctx );
  }
#endif

  // Optionally move the error checking dispatch to downstream of emulation.
  // This can be helpful for debugging Regal emulation
#if REGAL_ERROR_POST_EMU
#if REGAL_ERROR
  if( Config::enableError ) {
    InitDispatchTableError( ctx );
  }
#endif
#endif
  
#if REGAL_EMULATION
  if( Config::enableEmulation || Config::forceEmulation ) {
    InitDispatchTableEmu( ctx );               // emulated functions only
  }
#endif
  
#if ! REGAL_ERROR_POST_EMU
#if REGAL_DEBUG
  if( Config::enableDebug ) {
    InitDispatchTableDebug( ctx );
  }
#endif
#endif
  
#if REGAL_HTTP
  if( Config::enableHttp ) {
    InitDispatchTableHttp( ctx );
  }
#endif
  
#if REGAL_TRACE
  if( Config::enableTrace) {
    InitDispatchTableTrace( ctx );
  }
#endif
  
  
  */
  
  
  


  
  
  
}

REGAL_NAMESPACE_END
