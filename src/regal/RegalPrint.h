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

typedef boost::print::string_list<stxring> StringList;

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

#include <vector>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

extern PRINT_STREAM_TYPE printStream;

#if ! REGAL_USE_BOOST

struct PrintString {

  PrintString() { printStream.str(""); }

  template <typename T>
  PrintString & operator, ( const T & t ) {
	  printStream << t;
	  return *this;
  }

  std::string toString() { return printStream.str(); }
};

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

template <typename T, typename U>
std::string print_quote( const T & t, const U u ) {
  return print_string( u, t, u );
}

template <typename T>
std::string print_array( const T * t, int num, const char *quote = "\"", const char *open = "[ ", const char *close = " ]", const char *delim = ", " ) {
  std::string s = open;
  for( int i = 0; i < num - 1; i++ ) {
    s += print_string( t[i], delim );
  }
  if( num > 0 ) {
    s += print_string( t[num-1], close );
  } else {
    s += close;
  }
  return s;
}

// doesn't actually trim yet
template <typename T>
std::string print_trim( const T * t, char delim, int num, const char *prefix, const char *suffix ) {
  std::string s = prefix;
  s += t;
  // can skip the suffix if I'm not actually trimming
  // s += suffix;
  return s;
}



#define print_optional( ... ) std::string(" implement print_optional() ")
#define print_raw( ... ) std::string( " implement print_raw() " )
#define print_right( a, ... ) (a)
#define print_left( a, ... ) (a)




struct StringList {
  
  std::string join( std::string j ) const {
    std::string s;
    for( size_t i = 0; i < v.size() - 1; i++ ) {
      s += v[i];
    }
    s += v.back();
    return s;
  }
  
  void split( std::string s, char j ) {
    size_t p;
    while( ( p = s.find( j ) ) != std::string::npos ) {
      v.push_back( s.substr( 0, p ) );
      s.erase( 0, p + 1 );
    }
    if( s.size() ) {
      v.push_back( s );
    }
  }
  
  template <typename T>
  StringList & operator << ( const T & t ) { push_back( print_string( t ) ); return *this; }
  
  void push_back( std::string s ) { v.push_back( s ); }
  typedef std::vector<std::string>::const_iterator const_iterator;
  const_iterator begin() const { return v.begin(); }
  const_iterator end() const { return v.end(); }
  typedef std::vector<std::string>::iterator iterator;
  iterator begin() { return v.begin(); }
  iterator end() { return v.end(); }
  size_t size() const { return v.size(); }
  std::string str() const { return join( "" ); }
  std::vector<std::string> v;
};


#endif // ! REGAL_USE_BOOST

REGAL_NAMESPACE_END

#if ! REGAL_USE_BOOST
using Regal::PrintString;
using Regal::print_hex;
using Regal::print_quote;
using Regal::print_array;
using Regal::print_trim;
#endif

#endif // __REGAL_PRINT_H__
