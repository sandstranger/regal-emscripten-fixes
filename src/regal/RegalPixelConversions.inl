/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2012 Google Inc
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

 Regal Pixel Conversion Functions
 Lloyd Pique

 */

// ===========================================================================
// Static leading zero count.
// StaticLZC<12>::value == 3, StaticLZC<256>::value == 8, etc.
// ===========================================================================

template <GLuint V> struct StaticLZC { enum Value { value = ( V & 1 ) == 0 ? 1 + StaticLZC<V/2>::value : 0 }; };
template <> struct StaticLZC<0> { enum Value { value = 0 }; };

// ===========================================================================
// Static is-power-of-two.
// StaticIsPow2<4>::value == 1, StaticIsPow2<5>::value == 0, etc.
// ===========================================================================

template <GLuint V> struct StaticIsPow2 { enum Value { value = ( V & ( V - 1 ) ) == 0 }; };

// ===========================================================================
// 24 bit RGB is a bit special, but is otherwise treated like any other
// standard integer type. This is just a type tag.
// ===========================================================================

struct uint24_t;

// ===========================================================================
// Single component packing and unpacking.
//
// Each component is packed/unpacked based on a simple bitmask (assumed to be
// a contiguous set of one bits). For example Component<0x03c0> is useful to
// pack and unpack the Green component of RGB565 encoded data.
//
// p8/u8 pack/unpack each component into an eight bit integer intermediate
// form, though represented as a 32 bit value for efficiency.
// ===========================================================================

template <GLuint CM_ > struct Component
{
  enum Value
  {
    COMPONENT_MASK      = CM_,
    LEADING_BIT_COUNT   = StaticLZC<CM_>::value,
    COMPONENT_BIT_COUNT = StaticLZC<~( CM_ >> LEADING_BIT_COUNT )>::value
  };

  // This function ensures that unpacked integers are nicely expanded to fill
  // the full range of the unpacked integer size. For example from a four bit
  // packed form to an eight bit form, the value 0x0 is expanded to 0x00, 0x5
  // becomes 0x55, and 0xf becomes 0xff.

  template <size_t OUTPUT_BITS>
  static inline GLuint expand_( GLuint v )
  {
    v |= v >> ( ( ( COMPONENT_BIT_COUNT *  1 ) < OUTPUT_BITS ) ? ( COMPONENT_BIT_COUNT *  1 ) : 0 );
    v |= v >> ( ( ( COMPONENT_BIT_COUNT *  2 ) < OUTPUT_BITS ) ? ( COMPONENT_BIT_COUNT *  2 ) : 0 );
    v |= v >> ( ( ( COMPONENT_BIT_COUNT *  4 ) < OUTPUT_BITS ) ? ( COMPONENT_BIT_COUNT *  4 ) : 0 );
    v |= v >> ( ( ( COMPONENT_BIT_COUNT *  8 ) < OUTPUT_BITS ) ? ( COMPONENT_BIT_COUNT *  8 ) : 0 );
    v |= v >> ( ( ( COMPONENT_BIT_COUNT * 16 ) < OUTPUT_BITS ) ? ( COMPONENT_BIT_COUNT * 16 ) : 0 );
    v |= v >> ( ( ( COMPONENT_BIT_COUNT * 32 ) < OUTPUT_BITS ) ? ( COMPONENT_BIT_COUNT * 32 ) : 0 );
    return v;
  }

  static inline GLuint u8( GLuint v )
  {
    v = ( v & COMPONENT_MASK ) >> LEADING_BIT_COUNT;
    v <<= 8 - COMPONENT_BIT_COUNT;
    return expand_<8>( v );
  }

  static inline GLuint p8( GLuint v )
  {
    return ( v >> ( 8 - COMPONENT_BIT_COUNT ) ) << LEADING_BIT_COUNT;
  }
};

// ===========================================================================
// Single component packing and unpacking. Special overload for zero width
// (not present) components.
// ===========================================================================

template <> struct Component<0> {
  static GLuint u8( GLuint v ) { UNUSED_PARAMETER(v); return 0; }
  static GLuint p8( GLuint v ) { UNUSED_PARAMETER(v); return 0; }
};

// ===========================================================================
// Helper functions to read and write various integer sizes from byte arrays.
// These functions are used if the data is not aligned.
// ===========================================================================

template <typename P_> GLuint Read ( const GLubyte* src );
template <typename P_> void     Write( GLubyte* dst, GLuint v );

template <> GLuint Read<GLuint>( const GLubyte* src ) {
  GLuint v = 0;
  v |= src[ 0 ] << 0;
  v |= src[ 1 ] << 8;
  v |= src[ 2 ] << 16;
  v |= src[ 3 ] << 24;
  return v;
}

template <> GLuint Read<uint24_t>( const GLubyte* src ) {
  GLuint v = 0;
  v |= src[ 0 ] << 0;
  v |= src[ 1 ] << 8;
  v |= src[ 2 ] << 16;
  return v;
}

template <> GLuint Read<GLushort>( const GLubyte* src ) {
  GLuint v = 0;
  v |= src[ 0 ] << 0;
  v |= src[ 1 ] << 8;
  return v;
}

template <> GLuint Read<GLubyte>( const GLubyte* src ) {
  return src[ 0 ] << 0;
}

template <> void Write<GLuint>( GLubyte* dst, GLuint v ) {
  dst[ 0 ] = static_cast<GLubyte>(( v >>  0 ) & 255);
  dst[ 1 ] = static_cast<GLubyte>(( v >>  8 ) & 255);
  dst[ 2 ] = static_cast<GLubyte>(( v >> 16 ) & 255);
  dst[ 3 ] = static_cast<GLubyte>(( v >> 24 ) & 255);
}

template <> void Write<uint24_t>( GLubyte* dst, GLuint v ) {
  dst[ 0 ] = static_cast<GLubyte>(( v >>  0 ) & 255);
  dst[ 1 ] = static_cast<GLubyte>(( v >>  8 ) & 255);
  dst[ 2 ] = static_cast<GLubyte>(( v >> 16 ) & 255);
}

template <> void Write<GLushort>( GLubyte* dst, GLuint v ) {
  dst[ 0 ] = static_cast<GLubyte>(( v >> 0 ) & 255);
  dst[ 1 ] = static_cast<GLubyte>(( v >> 8 ) & 255);
}

template <> void Write<GLubyte>( GLubyte* dst, GLuint v ) {
  dst[ 0 ] = static_cast<GLubyte>(( v >> 0 ) & 255);
}

// ===========================================================================
// Pixel packing and unpacking, all components, and for any number of
// continuous pixel data in memory.
// ===========================================================================

template <typename PT_, GLuint PBS_, GLuint RM_, GLuint GM_, GLuint BM_, GLuint AM_>
struct PixelAny
{
  typedef PixelAny<PT_, PBS_, RM_, GM_, BM_, AM_> SelfType;
  typedef PT_ PixelType;

  enum {
    PACKED_BYTES = PBS_,
    RED_MASK = RM_,
    GRN_MASK = GM_,
    BLU_MASK = BM_,
    ALP_MASK = AM_,
    COMPONENT_COUNT = (!!RM_ + !!GM_ + !!BM_ + !!AM_),
    ALIGNMENT = StaticIsPow2<PBS_>::value ? PBS_ : 1,
  };

  // Pack/unpack for each component.

  static GLuint ur8( GLuint v ) { return Component<RED_MASK>::u8( v ); }
  static GLuint ug8( GLuint v ) { return Component<GRN_MASK>::u8( v ); }
  static GLuint ub8( GLuint v ) { return Component<BLU_MASK>::u8( v ); }
  static GLuint ua8( GLuint v ) { return Component<ALP_MASK>::u8( v ); }

  static GLuint pr8( GLuint v ) { return Component<RED_MASK>::p8( v ); }
  static GLuint pg8( GLuint v ) { return Component<GRN_MASK>::p8( v ); }
  static GLuint pb8( GLuint v ) { return Component<BLU_MASK>::p8( v ); }
  static GLuint pa8( GLuint v ) { return Component<ALP_MASK>::p8( v ); }

  // Pack/unpack for a whole pixel.

  static GLuint urgba32( GLuint v ) {
    return ( ua8( v ) << 24 ) | ( ub8( v ) << 16 ) | ( ug8( v ) <<  8 ) | ( ur8( v ) <<  0 );
  }

  static GLuint prgba32( GLuint v ) {
    return pa8( ( v & 0xff000000 ) >> 24 ) | pb8( ( v & 0x00ff0000 ) >> 16 ) | pg8( ( v & 0x0000ff00 ) >> 8 ) | pr8( ( v & 0x000000ff ) >> 0 );
  }

  // Pack/unpack for a sequence of pixels.
  // The "slow" functions read data a byte at a time from memory, and are meant
  // for unaligned reads and writes.

  static void Unpack32( const GLubyte* src, GLuint* dst, size_t cnt )  {
    while ( cnt-- ) {
      *dst++ = urgba32( Read<PixelType>( src ) );
      src += PACKED_BYTES;
    }
  }

  static void Pack32( const GLuint* src, GLubyte* dst, size_t cnt )  {
    while ( cnt-- ) {
      Write<PixelType>( dst, prgba32 ( *src++ ) );
      dst += PACKED_BYTES;
    }
  }

  static void Unpack32( const void* src, GLuint* dst, size_t cnt ) {
    Unpack32( static_cast<const GLubyte*>( src ), dst, cnt );
  }

  static void Pack32( const GLuint* src, void* dst, size_t cnt ) {
    Pack32( src, static_cast<GLubyte*>( dst ), cnt );
  }
};

// Template parameters:
//  - PT_   pixel type for stepping through input pixel data
//  - PBS_
//  - RM_   red bit mask
//  - GM_   red bit mask
//  - BM_   blue bit mask
//  - AM_   alpha bit mask

template <typename PT_, GLuint PBS_, GLuint RM_, GLuint GM_, GLuint BM_, GLuint AM_>
struct Pixel : public PixelAny<PT_, PBS_, RM_, GM_, BM_, AM_>
{
  typedef PixelAny<PT_, PBS_, RM_, GM_, BM_, AM_> BaseType;
  typedef Pixel   <PT_, PBS_, RM_, GM_, BM_, AM_> SelfType;

  typedef typename BaseType::PixelType PixelType;
  using BaseType::PACKED_BYTES;
  using BaseType::prgba32;
  using BaseType::urgba32;

  static SelfType singleton;

  static void Unpack32( const PixelType* src, GLuint* dst, size_t cnt )  {
    while ( cnt-- ) {
      *dst++ = urgba32( static_cast<GLuint>( *src++ ) );
    }
  }

  static void Pack32( const GLuint* src, PixelType* dst, size_t cnt )  {
    while ( cnt-- ) {
      *dst++ = static_cast<PixelType> ( prgba32 ( *src++ ) );
    }
  }

  static void Unpack32( const void* src, GLuint* dst, size_t cnt ) {
    bool aligned = ( reinterpret_cast<intptr_t>( src ) & ( PACKED_BYTES - 1 ) ) == 0;
    if ( aligned ) {
      Unpack32( static_cast<const PixelType*>( src ), dst, cnt );
    } else {
      BaseType::Unpack32( static_cast<const GLubyte*>( src ), dst, cnt );
    }
  }

  static void Pack32( const GLuint* src, void* dst, size_t cnt ) {
    bool aligned = ( reinterpret_cast<intptr_t>( dst ) & ( PACKED_BYTES - 1 ) ) == 0;
    if ( aligned ) {
      Pack32( src, static_cast<PixelType*>( dst ), cnt );
    } else {
      BaseType::Pack32( src, static_cast<GLubyte*>( dst ), cnt );
    }
  }
};
