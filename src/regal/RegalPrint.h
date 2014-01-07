/*
 Copyright (c) 2013 NVIDIA Corporation
 Copyright (c) 2013 Cass Everitt
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

#ifndef __REGAL_PRINT_H__
#define __REGAL_PRINT_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#define REGAL_USE_BOOST 0

#if REGAL_USE_BOOST

#include <boost/print/print_string.hpp>
using boost::print::print_string;

#define print_hex( ... ) boost::print::hex( __VA_ARGS__ )
#define print_quote( ... ) boost::print::quote( __VA_ARGS__ )
#define print_array( ... ) boost::print::array( __VA_ARGS__ )
#define print_optional( ... ) boost::print::optional( __VA_ARGS__ )
#define print_raw( ... ) boost::print::raw( __VA_ARGS__ )
#define print_trim( ... ) boost::print::trim( __VA_ARGS__ )
#define print_right( ... ) boost::print::right( __VA_ARGS__ )
#define print_left( ... ) boost::print::left( __VA_ARGS__ )

#define PRINT_STREAM_TYPE int

#else

#define PRINT_STREAM_TYPE std::ostringstream


#include <sstream>

#define print_string( ... ) ( PrintString(), __VA_ARGS__ ).toString()

#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

extern PRINT_STREAM_TYPE printStream;

#if ! REGAL_USE_BOOST

template <typename T>
T print_hex( const T & t ) {
  T r = t;
  return r;
}

template <typename T, typename U>
T print_hex( const T & t, const U & u ) {
  UNUSED_PARAMETER( u );
  T r = t;
  return r;
}

#define print_quote( a, ... ) (a)
#define print_array( ... ) std::string(" implement print_array() ")
#define print_optional( ... ) std::string(" implement print_optional() ")
#define print_raw( ... ) std::string( " implement print_raw() " )
#define print_trim( ... ) std::string( " implement trim() " )
#define print_right( a, ... ) (a)
#define print_left( a, ... ) (a)

struct PrintString {

  PrintString() { printStream.str(""); }

  template <typename T>
  PrintString & operator, ( const T & t ) {
	  printStream << t;
	  return *this;
  }

  std::string toString() { return printStream.str(); }
};

#endif // ! REGAL_USE_BOOST

REGAL_NAMESPACE_END

#if ! REGAL_USE_BOOST
using Regal::PrintString;
#endif

#endif // __REGAL_PRINT_H__
