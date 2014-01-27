/*
 Copyright (c) 2011-2013 NVIDIA Corporation
 Copyright (c) 2011-2012 Cass Everitt
 Copyright (c) 2012 Scott Nations
 Copyright (c) 2012 Mathias Schott
 Copyright (c) 2012 Nigel Stewart
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
 
 Regal DSA implementation
 Cass Everitt
 
 */

#ifndef __REGAL_DSA_H__
#define __REGAL_DSA_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalEmu.h"
#include "DsaProcs.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

#define REGAL_DSA_NUM_ASM_PROG_TYPES 5
#define REGAL_DSA_NUM_BUFFER_TYPES 2
#define REGAL_DSA_INVALID 0xffffffff

namespace Emu {
  
  template<typename P, typename T> inline void DsaGetv( P & orig, GLenum pname, T * params ) {
    UNUSED_PARAMETER(orig); UNUSED_PARAMETER(ctx); UNUSED_PARAMETER(pname); UNUSED_PARAMETER(params);
  }
  template<typename P> inline void DsaGetv( P & orig, GLenum pname, GLfloat * params ) {
    RglGetFloatv( orig, pname, params );
  }
  template<typename P> inline void DsaGetv( P & orig, GLenum pname, GLdouble * params ) {
    RglGetDoublev( orig, pname, params );
  }
  
  
  struct Dsa : public Layer
  {
    Dsa( RegalContext * ctx ) : Layer(ctx) {}

    virtual std::string GetName() const { return "dsa"; }

    virtual void ResetInterception() {
      RegalContext * ctx = GetContext();
      orig.Initialize( ctx->dispatchGL );
      DsaIntercept( this, ctx->dispatchGL );
    }
    
    
    int callDepth;
    struct Cache {
      GLenum matrixMode;
      GLenum activeTexture;
      GLenum clientActiveTexture;
      GLenum textureTarget;
      GLuint texture;
      GLuint asmProgram[ REGAL_DSA_NUM_ASM_PROG_TYPES ];
      GLuint vao;
      GLuint buffer;
      GLuint glslProgram;
      GLuint framebuffer;
      GLuint framebufferTarget;
      GLuint renderbuffer;
      GLuint renderbufferTarget;
    };
    Cache drv;
    Cache dsa;
    DsaOriginate orig;
    
    bool Initialize( const std::string & instanceInfo )
    {
      RegalContext *ctx = GetContext();
      orig.Initialize( ctx->dispatchGL );
      // determine if we can initialize
      bool canInit = true;
      if( canInit == false ) {
        return false;
      }
      DsaIntercept( this, ctx->dispatchGL );
      
      callDepth = 0;
      drv.matrixMode = GL_MODELVIEW;
      drv.activeTexture = GL_TEXTURE0;
      drv.clientActiveTexture = GL_TEXTURE0;
      drv.textureTarget = GL_TEXTURE_2D;
      drv.texture = 0;
      for( int i = 0; i < REGAL_DSA_NUM_ASM_PROG_TYPES; i++ ) {
        drv.asmProgram[i] = 0;
      }
      drv.vao = 0;
      drv.buffer = 0;
      drv.glslProgram = 0;
      drv.framebuffer = 0;
      drv.framebufferTarget = GL_FRAMEBUFFER;
      drv.renderbuffer = 0;
      drv.renderbufferTarget = GL_RENDERBUFFER;
      
      dsa.matrixMode = REGAL_DSA_INVALID;
      dsa.activeTexture = REGAL_DSA_INVALID;
      dsa.clientActiveTexture = REGAL_DSA_INVALID;
      dsa.textureTarget = REGAL_DSA_INVALID;
      dsa.texture = REGAL_DSA_INVALID;
      for( int i = 0; i < REGAL_DSA_NUM_ASM_PROG_TYPES; i++ ) {
        dsa.asmProgram[i] = REGAL_DSA_INVALID;
      }
      dsa.vao = REGAL_DSA_INVALID;
      dsa.buffer = REGAL_DSA_INVALID;
      dsa.glslProgram = REGAL_DSA_INVALID;
      dsa.framebuffer = REGAL_DSA_INVALID;
      dsa.framebufferTarget = REGAL_DSA_INVALID;
      dsa.renderbuffer = REGAL_DSA_INVALID;
      dsa.renderbufferTarget = REGAL_DSA_INVALID;

      return true;
    }
    
    void Restore() {
      RestoreMatrixMode();
      RestoreGlslProgram();
      RestoreFramebuffer();
      RestoreAsmProgram( GL_VERTEX_PROGRAM_ARB );
      RestoreAsmProgram( GL_FRAGMENT_PROGRAM_ARB );
      RestoreAsmProgram( GL_GEOMETRY_PROGRAM_NV );
      RestoreAsmProgram( GL_TESS_CONTROL_PROGRAM_NV );
      RestoreAsmProgram( GL_TESS_EVALUATION_PROGRAM_NV );
      RestoreVao();
      RestoreBuffer();
      RestoreTexture();
      RestoreActiveTexture();  // flushTexture() may dirty this
      RestoreClientActiveTexture();
    }
    
    
    // state shadow/dirty/restore methods
    
    ////////////////////////////////////////////////////////////////////////
    bool NotMatrixMode( GLenum mode ) const {
      return mode != ( dsa.matrixMode != REGAL_DSA_INVALID ? dsa.matrixMode : drv.matrixMode );
    }
    bool ShadowMatrixMode( GLenum realMatrixMode ) {
      drv.matrixMode = realMatrixMode;
      return dsa.matrixMode != REGAL_DSA_INVALID;
    }
    void DsaMatrixMode( GLenum mode ) {
      if( NotMatrixMode( mode ) ) {
        dsa.matrixMode = mode;
        switch( mode ) {
            //<>  dsn: wtf?  Why are these accepted?  And why only up to GL_TEXTURE3?
          case GL_TEXTURE0: case GL_TEXTURE1: case GL_TEXTURE2: case GL_TEXTURE3:
            DsaActiveTexture( mode );
            dsa.matrixMode = GL_TEXTURE;
            break;
        }
        RglMatrixMode( orig, dsa.matrixMode );
      }
    }
    void RestoreMatrixMode() {
      if( dsa.matrixMode != REGAL_DSA_INVALID ) {
        RglMatrixMode( orig, drv.matrixMode );
        RestoreActiveTexture();
        dsa.matrixMode = REGAL_DSA_INVALID;
      }
    }
    
    ////////////////////////////////////////////////////////////////////////
    bool NotActiveTexture( GLenum tex ) const {
      return tex != ( dsa.activeTexture != REGAL_DSA_INVALID ? dsa.activeTexture : drv.activeTexture );
    }
    bool ShadowActiveTexture( GLenum realActiveTexture ) {
      if ( !validTextureEnum(realActiveTexture) )
        return false;
      drv.activeTexture = realActiveTexture;
      bool usingDsa = dsa.activeTexture != REGAL_DSA_INVALID;
      return usingDsa;
    }
    void DsaActiveTexture( GLenum tex) {
      if ( !validTextureEnum(tex) )
        return;
      if( NotActiveTexture( tex ) ) {
        dsa.activeTexture = tex;
        RglActiveTexture( orig, dsa.activeTexture );
      }
    }
    void RestoreActiveTexture() {
      if( dsa.activeTexture != REGAL_DSA_INVALID ) {
        RglActiveTexture( orig, drv.activeTexture );
        dsa.activeTexture = REGAL_DSA_INVALID;
      }
    }
    
    ////////////////////////////////////////////////////////////////////////
    bool NotClientActiveTexture( GLenum tex ) const {
      return tex != ( dsa.clientActiveTexture != REGAL_DSA_INVALID ? dsa.clientActiveTexture : drv.clientActiveTexture );
    }
    bool ShadowClientActiveTexture( GLenum realClientActiveTexture ) {
      drv.clientActiveTexture = realClientActiveTexture;
      return dsa.clientActiveTexture != REGAL_DSA_INVALID;
    }
    void DsaClientActiveTexture( GLenum tex ) {
      if( NotClientActiveTexture( tex ) ) {
        dsa.clientActiveTexture = tex;
        RglClientActiveTexture( orig, dsa.clientActiveTexture );
      }
    }
    void RestoreClientActiveTexture() {
      if( dsa.clientActiveTexture != REGAL_DSA_INVALID ) {
        RglClientActiveTexture( orig, drv.clientActiveTexture );
        dsa.clientActiveTexture = REGAL_DSA_INVALID;
      }
    }
    
    ////////////////////////////////////////////////////////////////////////
    bool NotGlslProgram( GLuint program ) const {
      return program != ( dsa.glslProgram != REGAL_DSA_INVALID ? dsa.glslProgram : drv.glslProgram );
    }
    bool ShadowGlslProgram( GLuint realGlslProgram ) {
      drv.glslProgram = realGlslProgram;
      return dsa.glslProgram != REGAL_DSA_INVALID;
    }
    void DsaGlslProgram( GLuint program ) {
      if( NotGlslProgram( program ) ) {
        dsa.glslProgram = program;
        RglUseProgram( orig, dsa.glslProgram );
      }
    }
    void RestoreGlslProgram() {
      if( dsa.glslProgram != REGAL_DSA_INVALID ) {
        RglUseProgram( orig, drv.glslProgram );
        dsa.glslProgram = REGAL_DSA_INVALID;
      }
    }
    void DeleteGlslProgram( GLuint program ) {
      if( drv.glslProgram == program ) {
        drv.glslProgram = 0;
      }
      RestoreGlslProgram();
    }
    
    ////////////////////////////////////////////////////////////////////////
    bool NotFramebuffer( GLenum target, GLuint framebuffer ) const {
      return dsa.framebuffer != REGAL_DSA_INVALID ?
      ( ( target != dsa.framebufferTarget ) || ( framebuffer != dsa.framebuffer ) ) :
      ( ( target != drv.framebufferTarget ) || ( framebuffer != drv.framebuffer ) ) ;
      
    }
    bool ShadowFramebuffer( GLenum realFramebufferTarget, GLuint realFramebuffer ) {
      drv.framebufferTarget = realFramebufferTarget;
      drv.framebuffer = realFramebuffer;
      return dsa.framebuffer != REGAL_DSA_INVALID;
    }
    void DsaFramebuffer( GLenum target, GLuint framebuffer ) {
      if( NotFramebuffer( target, framebuffer ) ) {
        dsa.framebufferTarget = target;
        dsa.framebuffer = framebuffer;
        RglBindFramebuffer( orig, dsa.framebufferTarget, dsa.framebuffer );
      }
    }
    void RestoreFramebuffer() {
      if( dsa.framebuffer != REGAL_DSA_INVALID ) {
        RglBindFramebuffer( orig, drv.framebufferTarget, drv.framebuffer );
        dsa.framebufferTarget = REGAL_DSA_INVALID;
        dsa.framebuffer = REGAL_DSA_INVALID;
      }
    }
    void DeleteFramebuffers( GLsizei n, const GLuint * framebuffers ) {
      for( GLsizei i = 0; i < n; i++ ) {
        if( drv.framebuffer == framebuffers[i] ) {
          drv.framebuffer = 0;
        }
      }
      RestoreFramebuffer();
    }
    ////////////////////////////////////////////////////////////////////////
    bool NotRenderbuffer( GLenum target, GLuint renderbuffer ) const {
      return dsa.renderbuffer != REGAL_DSA_INVALID ?
      ( ( target != dsa.renderbufferTarget ) || ( renderbuffer != dsa.renderbuffer ) ) :
      ( ( target != drv.renderbufferTarget ) || ( renderbuffer != drv.renderbuffer ) ) ;
      
    }
    bool ShadowRenderbuffer( GLenum realRenderbufferTarget, GLuint realRenderbuffer ) {
      drv.renderbufferTarget = realRenderbufferTarget;
      drv.renderbuffer = realRenderbuffer;
      return dsa.renderbuffer != REGAL_DSA_INVALID;
    }
    void DsaRenderbuffer( GLenum target, GLuint renderbuffer ) {
      if( NotRenderbuffer( target, renderbuffer ) ) {
        dsa.renderbufferTarget = target;
        dsa.renderbuffer = renderbuffer;
        RglBindRenderbuffer( orig, dsa.renderbufferTarget, dsa.renderbuffer );
      }
    }
    void RestoreRenderbuffer() {
      if( dsa.renderbuffer != REGAL_DSA_INVALID ) {
        RglBindRenderbuffer( orig, drv.renderbufferTarget, drv.renderbuffer );
        dsa.renderbufferTarget = REGAL_DSA_INVALID;
        dsa.renderbuffer = REGAL_DSA_INVALID;
      }
    }
    void DeleteRenderbuffers( GLsizei n, const GLuint *renderbuffers ) {
      for( int i  = 0; i < n; i++ ) {
        if( renderbuffers[i] == drv.renderbuffer ) {
          drv.renderbuffer = 0;
        }
      }
      RestoreRenderbuffer();
    }
    
#define REGAL_DSA_NUM_ASM_TARGET_INDEXES 5
    static GLenum IndexToAsmTarget( int i ) {
      const GLenum array[] = {
        GL_VERTEX_PROGRAM_ARB,
        GL_FRAGMENT_PROGRAM_ARB,
        GL_GEOMETRY_PROGRAM_NV,
        GL_TESS_CONTROL_PROGRAM_NV,
        GL_TESS_EVALUATION_PROGRAM_NV
      };
      return array[i];
    }
    static int AsmTargetIndex( GLenum target ) {
      switch (target) {
        case GL_VERTEX_PROGRAM_ARB: return 0;
        case GL_FRAGMENT_PROGRAM_ARB: return 1;
        case GL_GEOMETRY_PROGRAM_NV: return 2;
        case GL_TESS_CONTROL_PROGRAM_NV: return 3;
        case GL_TESS_EVALUATION_PROGRAM_NV: return 4;
        default:
          break;
      }
      // freak out
      return 0;
    }
    
    ////////////////////////////////////////////////////////////////////////
    bool NotAsmProgram( GLenum target, GLuint prog ) const {
      int idx = AsmTargetIndex( target );
      return prog != ( dsa.asmProgram[idx] != REGAL_DSA_INVALID ? dsa.asmProgram[idx] : drv.asmProgram[idx] );
    }
    bool ShadowAsmProgram( GLenum target, GLuint prog ) {
      int idx = AsmTargetIndex( target );
      drv.asmProgram[idx] = prog;
      return dsa.asmProgram[idx] != REGAL_DSA_INVALID;
    }
    void DsaAsmProgram( GLenum target, GLuint prog ) {
      int idx = AsmTargetIndex( target );
      if( NotAsmProgram( target, prog ) ) {
        dsa.asmProgram[idx] = prog;
        RglBindProgramARB( orig, target, dsa.asmProgram[idx] );
      }
    }
    void RestoreAsmProgram( GLenum target ) {
      int idx = AsmTargetIndex( target );
      if( dsa.asmProgram[idx] != REGAL_DSA_INVALID ) {
        RglBindProgramARB( orig, target, drv.asmProgram[idx] );
        dsa.asmProgram[idx] = REGAL_DSA_INVALID;
      }
    }
    void DeleteAsmPrograms( GLsizei n, const GLuint *progs ) {
      for( GLsizei i  = 0; i < n; i++ ) {
        for( int j = 0; j < REGAL_DSA_NUM_ASM_TARGET_INDEXES; j++ ) {
          if( progs[i] == drv.asmProgram[ j ] ) {
            drv.asmProgram[ j ] = 0;
            RestoreAsmProgram( IndexToAsmTarget( j ) );
          }
        }
      }
    }
    
    
    ////////////////////////////////////////////////////////////////////////
    bool NotVao( GLuint vao ) const {
      return vao != ( dsa.vao != REGAL_DSA_INVALID ? dsa.vao : drv.vao );
    }
    bool ShadowVao( GLuint vao ) {
      drv.vao = vao;
      return dsa.vao != REGAL_DSA_INVALID;
    }
    void DsaVao( GLuint vao ) {
      if( NotVao( vao ) ) {
        dsa.vao = vao;
        RglBindVertexArray( orig, dsa.vao );
      }
    }
    void RestoreVao() {
      if( dsa.vao != REGAL_DSA_INVALID ) {
        RglBindVertexArray( orig, drv.vao );
        dsa.vao = REGAL_DSA_INVALID;
      }
    }
    void DeleteVaos( GLsizei n, const GLuint * arrays ) {
      for( GLsizei i = 0; i < n; i++ ) {
        if( drv.vao == arrays[i] ) {
          drv.vao = 0;
        }
      }
      RestoreVao();
    }
    
    
    
    ////////////////////////////////////////////////////////////////////////
    bool NotBuffer( GLuint buf ) const {
      return buf != ( dsa.buffer != REGAL_DSA_INVALID ? dsa.buffer : drv.buffer );
    }
    bool ShadowBuffer( GLenum target, GLuint buf ) {
      if( target == GL_ARRAY_BUFFER ) {
        drv.buffer = buf;
        return dsa.buffer != REGAL_DSA_INVALID;
      }
      return false;
    }
    void DsaBuffer( GLuint buf ) {
      if( NotBuffer( buf ) ) {
        dsa.buffer = buf;
        RglBindBuffer( orig, GL_ARRAY_BUFFER, dsa.buffer );
      }
    }
    void RestoreBuffer( ) {
      if( dsa.buffer != REGAL_DSA_INVALID ) {
        RglBindBuffer( orig, GL_ARRAY_BUFFER, drv.buffer );
        dsa.buffer = REGAL_DSA_INVALID;
      }
    }
    void DeleteBuffers( GLsizei n, const GLuint * buffers ) {
      for( GLsizei i = 0; i < n; i++ ) {
        if( drv.buffer == buffers[i] ) {
          drv.buffer = 0;
        }
      }
      RestoreBuffer();
    }
    
    ////////////////////////////////////////////////////////////////////////
    bool NotTexture( GLenum target, GLuint texture ) const {
      return dsa.texture != REGAL_DSA_INVALID ?
      ( target != dsa.textureTarget || texture != dsa.texture ) :
      ( target != drv.textureTarget || texture != drv.texture ) ;
    }
    
    bool ShadowTexture( GLenum target, GLuint texture ) {
      if( drv.activeTexture == GL_TEXTURE0 ) {
        drv.textureTarget = target;
        drv.texture = texture;
        return dsa.texture != REGAL_DSA_INVALID;
      }
      return false;
    }
    void ShadowDsaTexture( GLenum target, GLuint texture ) {
      if( NotActiveTexture( GL_TEXTURE0 ) == false ) {
        drv.textureTarget = target;
        drv.texture = texture;
      }
    }
    void DsaTexture( GLenum target, GLuint texture ) {
      switch( target ) {
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
          target = GL_TEXTURE_CUBE_MAP;
        default:
          break;
      }
      if( NotTexture( target, texture ) ) {
        dsa.textureTarget = target;
        dsa.texture = texture;
        DsaActiveTexture( GL_TEXTURE0 );
        RglBindTexture( orig, dsa.textureTarget, dsa.texture );
      }
    }
    void RestoreTexture() {
      if( dsa.texture != REGAL_DSA_INVALID ) {
        DsaActiveTexture( GL_TEXTURE0 );
        RglBindTexture( orig, drv.textureTarget, drv.texture );
        dsa.textureTarget = REGAL_DSA_INVALID;
        dsa.texture = REGAL_DSA_INVALID;
      }
    }
    void DeleteTextures( GLsizei n, const GLuint *textures ) {
      for( int i  = 0; i < n; i++ ) {
        if( textures[i] == drv.texture ) {
          drv.texture = 0;
        }
      }
      RestoreTexture();
      RestoreActiveTexture();
    }
    
    void ClientAttribDefault( GLbitfield mask )
    {
      if (mask&GL_CLIENT_PIXEL_STORE_BIT)
      {
        RglPixelStorei( orig, GL_UNPACK_SWAP_BYTES, 0 );
        RglPixelStorei( orig, GL_UNPACK_LSB_FIRST, 0 );
        RglPixelStorei( orig, GL_UNPACK_ROW_LENGTH, 0 );
        RglPixelStorei( orig, GL_UNPACK_SKIP_ROWS, 0 );
        RglPixelStorei( orig, GL_UNPACK_SKIP_PIXELS, 0 );
        RglPixelStorei( orig, GL_UNPACK_ALIGNMENT, 4 );
        RglPixelStorei( orig, GL_UNPACK_IMAGE_HEIGHT, 0 );
        RglPixelStorei( orig, GL_UNPACK_SKIP_IMAGES, 0 );
        RglPixelStorei( orig, GL_PACK_SWAP_BYTES, 0 );
        RglPixelStorei( orig, GL_PACK_LSB_FIRST, 0 );
        RglPixelStorei( orig, GL_PACK_IMAGE_HEIGHT, 0 );
        RglPixelStorei( orig, GL_PACK_SKIP_IMAGES, 0 );
        RglPixelStorei( orig, GL_PACK_ROW_LENGTH, 0 );
        RglPixelStorei( orig, GL_PACK_SKIP_ROWS, 0 );
        RglPixelStorei( orig, GL_PACK_SKIP_PIXELS, 0 );
        RglPixelStorei( orig, GL_PACK_ALIGNMENT, 4 );
        RglPixelStorei( orig, GL_PIXEL_PACK_BUFFER_BINDING, 0 );
        RglPixelStorei( orig, GL_PIXEL_UNPACK_BUFFER_BINDING, 0 );
        RglPixelStorei( orig, GL_MAP_COLOR, 0 );
        RglPixelStorei( orig, GL_MAP_STENCIL, 0 );
        RglPixelStorei( orig, GL_INDEX_SHIFT, 0 );
        RglPixelStorei( orig, GL_INDEX_OFFSET, 0 );
        
        RglPixelStoref( orig, GL_RED_SCALE, 1.0f );
        RglPixelStoref( orig, GL_GREEN_SCALE, 1.0f );
        RglPixelStoref( orig, GL_BLUE_SCALE, 1.0f );
        RglPixelStoref( orig, GL_ALPHA_SCALE, 1.0f );
        RglPixelStoref( orig, GL_DEPTH_SCALE, 1.0f );
        RglPixelStoref( orig, GL_RED_BIAS, 0.0f );
        RglPixelStoref( orig, GL_GREEN_BIAS, 0.0f );
        RglPixelStoref( orig, GL_BLUE_BIAS, 0.0f );
        RglPixelStoref( orig, GL_ALPHA_BIAS, 0.0f );
        RglPixelStoref( orig, GL_DEPTH_BIAS, 0.0f );
      }
      
      if (mask&GL_CLIENT_VERTEX_ARRAY_BIT)
      {
        // FIXME: need number of texture units
        int maxTextureUnit = 7;
        for( int i = maxTextureUnit; i >= 0; i-- ) {
          RglClientActiveTexture( orig, GL_TEXTURE0 + i );
          RglDisableClientState( orig, GL_TEXTURE_COORD_ARRAY );
        }
        for( int i = 0; i < 16; i++ ) {
          RglDisableVertexAttribArray( orig, i );
          RglVertexAttribPointer( orig, i, 4, GL_FLOAT, GL_FALSE, 0, NULL );
        }
        RglDisableClientState( orig, GL_VERTEX_ARRAY );
        RglDisableClientState( orig, GL_NORMAL_ARRAY );
        RglDisableClientState( orig, GL_FOG_COORD_ARRAY );
        RglDisableClientState( orig, GL_COLOR_ARRAY );
        RglDisableClientState( orig, GL_SECONDARY_COLOR_ARRAY );
        RglDisableClientState( orig, GL_INDEX_ARRAY );
        RglDisableClientState( orig, GL_EDGE_FLAG_ARRAY );
        RglVertexPointer( orig, 4, GL_FLOAT, 0, NULL );
        RglNormalPointer( orig, GL_FLOAT, 0, NULL );
        RglFogCoordPointer( orig, GL_FLOAT, 0, NULL );
        RglColorPointer( orig, 4, GL_FLOAT, 0, NULL );
        RglSecondaryColorPointer( orig, 3, GL_FLOAT, 0, NULL );
        RglIndexPointer( orig, GL_FLOAT, 0, NULL );
      }
    }
    
    template< typename T >
    bool GetIndexedv( GLenum pname, GLuint index, T * params ) {
      switch( pname ) {
        case GL_PROGRAM_MATRIX_EXT:
          DsaMatrixMode( GL_MATRIX0_ARB + index );
          DsaGetv( orig, GL_CURRENT_MATRIX_ARB, params );
          break;
        case GL_TRANSPOSE_PROGRAM_MATRIX_EXT:
          DsaMatrixMode( GL_MATRIX0_ARB + index );
          DsaGetv( orig, GL_TRANSPOSE_CURRENT_MATRIX_ARB, params );
          break;
        case GL_PROGRAM_MATRIX_STACK_DEPTH_EXT:
          DsaMatrixMode( GL_MATRIX0_ARB + index );
          DsaGetv( orig, GL_CURRENT_MATRIX_STACK_DEPTH_ARB, params );
          break;
        case GL_CURRENT_MATRIX_NV:
        case GL_CURRENT_MATRIX_STACK_DEPTH_NV:
        case GL_CURRENT_RASTER_TEXTURE_COORDS:
        case GL_CURRENT_TEXTURE_COORDS:
        case GL_TEXTURE_BINDING_1D:
        case GL_TEXTURE_BINDING_1D_ARRAY:
        case GL_TEXTURE_BINDING_2D:
        case GL_TEXTURE_BINDING_2D_ARRAY:
        case GL_TEXTURE_BINDING_3D:
        case GL_TEXTURE_BINDING_BUFFER_EXT:
        case GL_TEXTURE_BINDING_CUBE_MAP:
        case GL_TEXTURE_BINDING_CUBE_MAP_ARRAY:
        case GL_TEXTURE_BINDING_RECTANGLE_ARB:
        case GL_TEXTURE_BINDING_RENDERBUFFER_NV:
        case GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV:
        case GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT:
        case GL_TEXTURE_BUFFER_FORMAT_EXT:
        case GL_TEXTURE_GEN_Q:
        case GL_TEXTURE_GEN_R:
        case GL_TEXTURE_GEN_S:
        case GL_TEXTURE_GEN_T:
        case GL_TEXTURE_MATRIX:
        case GL_TEXTURE_STACK_DEPTH:
        case GL_TRANSPOSE_TEXTURE_MATRIX:
        case GL_TEXTURE_1D: case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_RECTANGLE_ARB:
          DsaActiveTexture( GL_TEXTURE0 + index );
          DsaGetv( orig, pname, params );
          break;
        case GL_TEXTURE_COORD_ARRAY:
        case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
          //case GL_TEXTURE_COORD_ARRAY_COUNT:
        case GL_TEXTURE_COORD_ARRAY_SIZE:
        case GL_TEXTURE_COORD_ARRAY_STRIDE:
        case GL_TEXTURE_COORD_ARRAY_TYPE:
          DsaClientActiveTexture( GL_TEXTURE0 + index );
          DsaGetv( orig, pname, params );
          break;
        default:
          return false;
      }
      return true;
    }
    
    void RestoreGet( GLenum pname ) {
      switch( pname ) {
        case GL_CURRENT_PROGRAM:
          RestoreGlslProgram();
          break;
        case GL_MATRIX_MODE:
        case GL_PROGRAM_MATRIX_EXT:
        case GL_TRANSPOSE_PROGRAM_MATRIX_EXT:
        case GL_PROGRAM_MATRIX_STACK_DEPTH_EXT:
          RestoreMatrixMode();
          break;
        case GL_ACTIVE_TEXTURE:
        case GL_CURRENT_MATRIX_NV:
        case GL_CURRENT_MATRIX_STACK_DEPTH_NV:
        case GL_CURRENT_RASTER_TEXTURE_COORDS:
        case GL_CURRENT_TEXTURE_COORDS:
        case GL_TEXTURE_BINDING_1D:
        case GL_TEXTURE_BINDING_1D_ARRAY:
        case GL_TEXTURE_BINDING_2D:
        case GL_TEXTURE_BINDING_2D_ARRAY:
        case GL_TEXTURE_BINDING_3D:
        case GL_TEXTURE_BINDING_BUFFER_EXT:
        case GL_TEXTURE_BINDING_CUBE_MAP:
        case GL_TEXTURE_BINDING_CUBE_MAP_ARRAY:
        case GL_TEXTURE_BINDING_RECTANGLE_ARB:
        case GL_TEXTURE_BINDING_RENDERBUFFER_NV:
        case GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV:
        case GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT:
        case GL_TEXTURE_BUFFER_FORMAT_EXT:
        case GL_TEXTURE_GEN_Q:
        case GL_TEXTURE_GEN_R:
        case GL_TEXTURE_GEN_S:
        case GL_TEXTURE_GEN_T:
        case GL_TEXTURE_MATRIX:
        case GL_TEXTURE_STACK_DEPTH:
        case GL_TRANSPOSE_TEXTURE_MATRIX:
        case GL_TEXTURE_1D: case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_RECTANGLE_ARB:
          RestoreActiveTexture();
          break;
        case GL_CLIENT_ACTIVE_TEXTURE:
        case GL_TEXTURE_COORD_ARRAY:
        case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
          //case GL_TEXTURE_COORD_ARRAY_COUNT:
        case GL_TEXTURE_COORD_ARRAY_SIZE:
        case GL_TEXTURE_COORD_ARRAY_STRIDE:
        case GL_TEXTURE_COORD_ARRAY_TYPE:
          RestoreClientActiveTexture();
          break;
        case GL_DRAW_BUFFER:
        case GL_READ_BUFFER:
        case GL_DRAW_BUFFER0:
        case GL_DRAW_BUFFER1:
        case GL_DRAW_BUFFER2:
        case GL_DRAW_BUFFER3:
        case GL_DRAW_BUFFER4:
        case GL_DRAW_BUFFER5:
        case GL_DRAW_BUFFER6:
        case GL_DRAW_BUFFER7:
        case GL_DRAW_BUFFER8:
        case GL_DRAW_BUFFER9:
        case GL_DRAW_BUFFER10:
        case GL_DRAW_BUFFER11:
        case GL_DRAW_BUFFER12:
        case GL_DRAW_BUFFER13:
        case GL_DRAW_BUFFER14:
        case GL_DRAW_BUFFER15:
          RestoreFramebuffer();
          
        default:
          break;
      }
    }
    
    GLboolean IsEnabledIndexed( GLenum pname, GLuint index ) {
      switch( pname ) {
        case GL_TEXTURE_1D:
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_RECTANGLE_ARB:
        case GL_TEXTURE_GEN_S:
        case GL_TEXTURE_GEN_T:
        case GL_TEXTURE_GEN_R:
        case GL_TEXTURE_GEN_Q:
          DsaActiveTexture( GL_TEXTURE0 + index );
          return RglIsEnabled( orig, pname );
          break;
        case GL_TEXTURE_COORD_ARRAY:
          DsaClientActiveTexture( GL_TEXTURE0 + index );
          return RglIsEnabled( orig, pname );
        default:
          break;
      }
      return GL_FALSE;
    }
    
    void RestoreIsEnabled( GLenum pname ) {
      switch( pname ) {
        case GL_TEXTURE_1D:
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_RECTANGLE_ARB:
        case GL_TEXTURE_GEN_S:
        case GL_TEXTURE_GEN_T:
        case GL_TEXTURE_GEN_R:
        case GL_TEXTURE_GEN_Q:
          RestoreActiveTexture();
          break;
        case GL_TEXTURE_COORD_ARRAY:
          RestoreClientActiveTexture();
        default:
          break;
      }
    }
    
  };
  
}

REGAL_NAMESPACE_END

#endif // ! __REGAL_DSA_H__







