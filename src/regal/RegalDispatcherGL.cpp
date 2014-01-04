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

DispatcherGL::DispatcherGL()
{
  #if REGAL_TRACE
  if( Config::enableTrace) {
    ::memset(&trace,0,sizeof(DispatchTableGL));
    InitDispatchTableTrace(trace);
    tables.push_back(&trace);
  }
  #endif

  #if REGAL_HTTP
  if( Config::enableHttp ) {
    ::memset(&http,0,sizeof(DispatchTableGL));
    InitDispatchTableHttp(http);
    tables.push_back(&http);
  }
  #endif
  
  
  #if REGAL_DEBUG
  if( Config::enableDebug ) {
    ::memset(&debug,0,sizeof(DispatchTableGL));
    InitDispatchTableDebug(debug);
    tables.push_back(&debug);
  }
  #endif

  #if REGAL_EMULATION
  if( Config::enableEmulation || Config::forceEmulation ) {
    ::memset(&emulation,0,sizeof(DispatchTableGL));
    InitDispatchTableEmu(emulation);               // emulated functions only
    tables.push_back(&emulation);
  }
  #endif

  #if REGAL_ERROR
  if( Config::enableError ) {
    ::memset(&error,0,sizeof(DispatchTableGL));
    InitDispatchTableError(error);
    tables.push_back(&error);
  }
  #endif
  
  #if REGAL_CACHE
  if( true ) {
    ::memset(&cache,0,sizeof(DispatchTableGL));
    InitDispatchTableCache(cache);
    tables.push_back(&cache);
  }
  #endif

  #if REGAL_CODE
  if( Config::enableCode ) {
    ::memset(&code,0,sizeof(DispatchTableGL));
    InitDispatchTableCode(code);
    tables.push_back(&code);
  }
  #endif

  #if REGAL_STATISTICS
  if( Config::enableStatistics ) {
    ::memset(&statistics,0,sizeof(DispatchTableGL));
    InitDispatchTableStatistics(statistics);
    tables.push_back(&statistics);
  }
  #endif

  #if REGAL_LOG
  if( Config::enableLog ) {
    ::memset(&logging,0,sizeof(DispatchTableGL));
    InitDispatchTableLog(logging);
    tables.push_back(&logging);
  }
  #endif

  #if REGAL_DRIVER
  if( Config::enableDriver ) {
    ::memset(&driver,0,sizeof(DispatchTableGL));
  #if REGAL_STATIC_ES2
    InitDispatchTableStaticES2(driver);           // ES 2.0 functions only
  #elif REGAL_SYS_PPAPI
    InitDispatchTablePpapi(driver);               // ES 2.0 functions only
  #else
    Loader::Init(driver);                         // Desktop/ES2.0 lazy loader
  #endif
    tables.push_back(&driver);
  }
  #endif

  #if REGAL_MISSING
  if( Config::enableMissing ) {
    ::memset(&missing,0,sizeof(DispatchTableGL));
    Missing::Init(missing);
    tables.push_back(&missing);
  }
  #endif

  // Optionally move the error checking dispatch to downstream of emulation.
  // This can be helpful for debugging Regal emulation

  #if defined(REGAL_ERROR_POST_EMU)
  if ( Config::error && Config::emulation ) {
    for( size_t i = 0; i < tables.size(); i++ ) {
      if( tables[i] == &error ) {
        tables.erase( tables.begin() + i );
        break;
      }
    }
    for( size_t i = 0; i < tables.size(); i++ ) {
      if( tables[i] == &emulation ) {
        tables.insert( tables.begin() + i, &error );
        break;
      }
    }
  }
  #endif
  
  
  // from the bottom up, replace any null function pointers with
  // non-null ones from the lower level - this means the lazy loader has to be clever
  for( size_t i = tables.size() - 1; i == 0; i-- ) {
    void ** lower = reinterpret_cast<void **>( tables[i] );
    void ** upper = reinterpret_cast<void **>( tables[i-1] );
    for( int j = 0; j < sizeof( DispatchTableGL ) / sizeof( void *); j++ ) {
      if( upper[j] == NULL ) {
        upper[j] = lower[j];
      }
    }
  }
  
  
}

DispatcherGL::~DispatcherGL()
{
}

REGAL_NAMESPACE_END
