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

#include <cstring>
#include <sstream>

#define print_string( ... ) ( PrintString(), __VA_ARGS__ ).toString()

#endif

#include <vector>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

extern PRINT_STREAM_TYPE printStream;

#if ! REGAL_USE_BOOST

inline void AppendBytes( const char * s, int len, char * buf, int capacity, int & sz ) {
  int n = std::min( len, capacity - sz - 1 );
  memcpy( buf + sz, s, n );
  sz += n;
  buf[sz] = 0;
}

template <typename T>
inline void AppendString( const T & t, char * buf, int capacity, int & sz ) {
  if( (capacity - sz) <= 1 ) {
    return;
  }
  printStream.str("");
  printStream << t;
  const std::string & s = printStream.str();
  AppendBytes( s.c_str(), int(s.size()), buf, capacity, sz );
}

template <>
inline void AppendString( const std::string & s, char * buf, int capacity, int & sz ) {
  if( (capacity - sz) <= 1 ) {
    return;
  }
  return AppendBytes( s.c_str(), int(s.size()), buf, capacity, sz );
}

template < int N >
inline void AppendString( char const (&s)[N], char * buf, int capacity, int & sz ) {
  if( (capacity - sz) <= 1 ) {
    return;
  }
  return AppendBytes( &s[0], N-1, buf, capacity, sz );
}

template <>
inline void AppendString( const char * const & s, char * buf, int capacity, int & sz ) {
  if( (capacity - sz) <= 1 ) {
    return;
  }
  AppendBytes( s, int(strlen(s)), buf, capacity, sz );
}

template <>
inline void AppendString( const char & s, char * buf, int capacity, int & sz ) {
  if( (capacity - sz) <= 1 ) {
    return;
  }
  AppendBytes( &s, 1, buf, capacity, sz );
}

template <>
inline void AppendString( const int & i, char * buf, int capacity, int & sz ) {
  if( (capacity - sz) <= 1 ) {
    return;
  }
  char nb[80];
	int len = sprintf( nb, "%d", i );
	AppendBytes( nb, len, buf, capacity, sz );
}

template <>
inline void AppendString( const float & f, char * buf, int capacity, int & sz ) {
  if( (capacity - sz) <= 1 ) {
    return;
  }
	char nb[80];
	int len = sprintf( nb, "%f", f );
	AppendBytes( nb, len, buf, capacity, sz );
}


struct PrintString {

  PrintString() : sz(0) { buf[0] = 0; }

  template <typename T>
  PrintString & operator, ( const T & t ) {
    AppendString( t, buf, sizeof( buf ), sz );
	  return *this;
  }
  int sz;
  char buf[1024];
  std::string toString() { return buf; }
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
  UNUSED_PARAMETER(quote);
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

template <typename T>
inline std::string print_trim( const T * t, char delim, unsigned int num, const char *prefix, const char *suffix ) {
  std::string trimmed = prefix;
  while( num ) {
    const T * t2 = t;
    while( *t2 != delim && *t2 != 0 ) {
      t2++;
    }
    if( *t2 != 0 ) { // include delim if t2 is delim
      t2++;
    }
    std::string el( t, size_t(t2 - t) );
    trimmed += el;
    if( *t2 == 0 ) {
      break;
    }
    num--;
    t = t2 + 1;
  }
  if ( num == 0 ) {
    trimmed += delim;
    trimmed += suffix;
  }
  return trimmed;
}

inline std::string print_raw( const void * p, const size_t size, const size_t size_limit ) {
  char h[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
  const char *cp = reinterpret_cast< const char * >( p );
  std::string s;
  s.reserve( size_limit * 2 + size_limit / 4 + 4);
  s += "[ ";
  for( size_t i = 0; i < size_limit; i++ ) {
    s += h[ (cp[i] >> 4) & 0xf ];
    s += h[ (cp[i] >> 0) & 0xf ];
    if( (i & 0x3) == 0x3 ) {
      s += ' ';
    }
  }
  if( ( size_limit & 0x3 ) == 0x3 ) {
    s.erase( s.end()-- );
  }
  if( size_limit < size ) {
    s += " ...";
  }
  s += " ]";
  return s;
}


#define print_optional( ... ) print_string(" implement print_optional() ")
#define print_right( a, ... ) print_string(a)
#define print_left( a, ... ) print_string(a)




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
