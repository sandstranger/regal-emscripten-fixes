/*
  Copyright (c) 2011-2012 NVIDIA Corporation
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

#ifndef __REGAL_CLIENT_STATE_H__
#define __REGAL_CLIENT_STATE_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <cstring>    // For memset, memcpy

#include <string>
#include <algorithm>  // For std::swap

#include "RegalPrint.h"

#include "RegalEmu.h"
#include "RegalToken.h"
#include "RegalDispatch.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

//
// OpenGL Client State Tracking
//
// Motivating requirements:
//
//  - Emulation of glPushClientAttrib and glPopClientAttrib for OpenGL ES and
//    core OpenGL that lack the functionality.
//
//  - OpenGL client state capture, browsing, diff and serialization.
//
//  - Emulation of BaseVertex commands
//
// See also:
//
//  - Gallium3D
//    http://wiki.freedesktop.org/wiki/Software/gallium
//    http://dri.freedesktop.org/doxygen/gallium/p__state_8h-source.html
//
//  - Tracking Graphics State For Networked Rendering
//    Ian Buck, Greg Humphreys and Pat Hanrahan.
//    Stanford University
//    Proceedings of the 2000 Eurographics/SIGGRAPH Workshop on Graphics Hardware
//    http://graphics.stanford.edu/papers/state_tracking/
//
//  - Chromium: A Stream Processing Framework for Interactive Rendering on Clusters
//    Greg Humphreys, Mike Houston, Ren Ng
//    Stanford University
//    SIGGRAPH 2002
//    http://graphics.stanford.edu/papers/cr/
//

namespace ClientState
{

  typedef StringList string_list;

  template <typename Procs>
  inline static void enable(Procs &dt, RegalContext * ctx, const GLenum cap, const GLboolean enable)
  {
    if (enable)
      dt.glEnableClientState(ctx,cap);
    else
      dt.glDisableClientState(ctx,cap);
  }

  template <typename Procs>
  inline static void enablei(Procs &dt, RegalContext * ctx, const GLenum cap, const GLuint index, const GLboolean enable)
  {
    if (enable)
      dt.glEnableClientStateiEXT(ctx,cap,index);
    else
      dt.glDisableClientStateiEXT(ctx,cap,index);
  }

  inline static void enableToString(string_list& tmp, const GLboolean b, const char* bEnum, const char *delim = "\n")
  {
      tmp << print_string(b ? "glEnableClientState(" : "glDisableClientState(",bEnum,");",delim);
  }

  inline static void enableiToString(string_list& tmp, const GLboolean b, const char* bEnum, const GLuint index, const char *delim = "\n")
  {
      tmp << print_string(b ? "glEnableClientStateiEXT(" : "glDisableClientStateiEXT(",bEnum,",",index,");",delim);
  }

  //
  // glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT)
  //

  const GLuint nNamedArrays = 7 + REGAL_EMU_MAX_TEXTURE_COORDS;

  typedef enum _vaName
  {
    VERTEX          = 0,
    NORMAL          = 1,
    FOG_COORD       = 2,
    COLOR           = 3,
    SECONDARY_COLOR = 4,
    INDEX           = 5,
    EDGE_FLAG       = 6,
    TEX_COORD       = 7
  } vaName;

  const GLenum vaEnum[][6] =
  {
    { GL_VERTEX_ARRAY,          GL_VERTEX_ARRAY_POINTER,          GL_VERTEX_ARRAY_BUFFER_BINDING,          GL_VERTEX_ARRAY_SIZE,          GL_VERTEX_ARRAY_TYPE,          GL_VERTEX_ARRAY_STRIDE          },
    { GL_NORMAL_ARRAY,          GL_NORMAL_ARRAY_POINTER,          GL_NORMAL_ARRAY_BUFFER_BINDING,          GL_ZERO,                       GL_NORMAL_ARRAY_TYPE,          GL_NORMAL_ARRAY_STRIDE          },
    { GL_FOG_COORD_ARRAY,       GL_FOG_COORD_ARRAY_POINTER,       GL_FOG_COORD_ARRAY_BUFFER_BINDING,       GL_ZERO,                       GL_FOG_COORD_ARRAY_TYPE,       GL_FOG_COORD_ARRAY_STRIDE       },
    { GL_COLOR_ARRAY,           GL_COLOR_ARRAY_POINTER,           GL_COLOR_ARRAY_BUFFER_BINDING,           GL_COLOR_ARRAY_SIZE,           GL_COLOR_ARRAY_TYPE,           GL_COLOR_ARRAY_STRIDE           },
    { GL_SECONDARY_COLOR_ARRAY, GL_SECONDARY_COLOR_ARRAY_POINTER, GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING, GL_SECONDARY_COLOR_ARRAY_SIZE, GL_SECONDARY_COLOR_ARRAY_TYPE, GL_SECONDARY_COLOR_ARRAY_STRIDE },
    { GL_INDEX_ARRAY,           GL_INDEX_ARRAY_POINTER,           GL_INDEX_ARRAY_BUFFER_BINDING,           GL_ZERO,                       GL_INDEX_ARRAY_TYPE,           GL_INDEX_ARRAY_STRIDE           },
    { GL_EDGE_FLAG_ARRAY,       GL_EDGE_FLAG_ARRAY_POINTER,       GL_EDGE_FLAG_ARRAY_BUFFER_BINDING,       GL_ZERO,                       GL_ZERO,                       GL_EDGE_FLAG_ARRAY_STRIDE       },
    { GL_TEXTURE_COORD_ARRAY,   GL_TEXTURE_COORD_ARRAY_POINTER,   GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING,   GL_TEXTURE_COORD_ARRAY_SIZE,   GL_TEXTURE_COORD_ARRAY_TYPE,   GL_TEXTURE_COORD_ARRAY_STRIDE   },
  };

  struct NamedVertexArray
  {
    GLboolean  enabled;  // GL_x_ARRAY
    const GLvoid* pointer;  // GL_x_ARRAY_POINTER
    GLuint     buffer;   // GL_x_ARRAY_BUFFER_BINDING
    GLint      size;     // GL_x_ARRAY_SIZE
    GLenum     type;     // GL_x_ARRAY_TYPE
    GLsizei    stride;   // GL_x_ARRAY_STRIDE

    inline NamedVertexArray()
    {
      Reset();
    }

    inline void Reset()
    {
      enabled = GL_FALSE;
      pointer = NULL;
      buffer = 0;
      size = 4;
      type = GL_FLOAT;
      stride = 0;
    }

    inline NamedVertexArray &swap(NamedVertexArray &other)
    {
      std::swap(enabled,other.enabled);
      std::swap(pointer,other.pointer);
      std::swap(buffer,other.buffer);
      std::swap(size,other.size);
      std::swap(type,other.type);
      std::swap(stride,other.stride);
      return *this;
    }

    template <typename Procs>
    NamedVertexArray &get(Procs &dt, vaName va)
    {
      GLint vaInt = static_cast<GLint>(va);
      RegalAssert(vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays));
      if (vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays))
      {
        if (vaInt < 7)
        {
          enabled = dt.glIsEnabled(vaEnum[vaInt][0]);
          dt.glGetPointerv(vaEnum[vaInt][1],const_cast<GLvoid**>(&pointer));
          dt.glGetIntegerv(vaEnum[vaInt][2],reinterpret_cast<GLint*>(&buffer));
          if (vaEnum[vaInt][3] != GL_ZERO)
            dt.glGetIntegerv(vaEnum[vaInt][3],&size);
          if (vaEnum[vaInt][4] != GL_ZERO)
            dt.glGetIntegerv(vaEnum[vaInt][4],reinterpret_cast<GLint*>(&type));
          dt.glGetIntegerv(vaEnum[vaInt][5],&stride);
        }
        else
        {
          GLuint index = static_cast<GLuint>(vaInt - 7);
          enabled = dt.glIsEnabledi(vaEnum[7][0],index);
          dt.glGetPointeri_vEXT(vaEnum[7][1],index,const_cast<GLvoid**>(&pointer));
          dt.glGetIntegeri_v(vaEnum[7][2],index,reinterpret_cast<GLint*>(&buffer));
          dt.glGetIntegeri_v(vaEnum[7][3],index,&size);
          dt.glGetIntegeri_v(vaEnum[7][4],index,reinterpret_cast<GLint*>(&type));
          dt.glGetIntegeri_v(vaEnum[7][5],index,&stride);
        }
      }
      return *this;
    }

    template <typename Procs>
    const NamedVertexArray &set(Procs &dt, RegalContext * ctx, vaName va, bool driverAllowsVertexAttributeArraysWithoutBoundBuffer) const
    {
      GLint vaInt = static_cast<GLint>(va);
      RegalAssert(vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays));
      if (buffer || driverAllowsVertexAttributeArraysWithoutBoundBuffer)
      {
        if (vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays))
        {
          if (vaInt < 7)
          {
            enable(dt,ctx,vaEnum[vaInt][0],enabled);
            dt.glBindBuffer(ctx, GL_ARRAY_BUFFER,buffer);
            switch (vaInt)
            {
              case VERTEX:
                dt.glVertexPointer(ctx, size,type,stride,pointer);
                break;
              case NORMAL:
                dt.glNormalPointer(ctx, type,stride,pointer);
                break;
              case FOG_COORD:
                dt.glFogCoordPointer(ctx, type,stride,pointer);
                break;
              case COLOR:
                dt.glColorPointer(ctx, size,type,stride,pointer);
                break;
              case SECONDARY_COLOR:
                dt.glSecondaryColorPointer(ctx, size,type,stride,pointer);
                break;
              case INDEX:
                dt.glIndexPointer(ctx, type,stride,pointer);
                break;
              case EDGE_FLAG:
                dt.glEdgeFlagPointer(ctx, stride,pointer);
                break;
              default:
                break;
            }
          }
          else
          {
            GLuint index = static_cast<GLuint>(vaInt - 7);
            enablei(dt,ctx,vaEnum[7][0],index,enabled);
            dt.glBindBuffer(ctx, GL_ARRAY_BUFFER,buffer);
            dt.glMultiTexCoordPointerEXT(ctx, GL_TEXTURE0+index,size,type,stride,pointer);
          }
        }
      }
      return *this;
    }

    template <typename Procs>
    void transition(Procs &dt, RegalContext * ctx, NamedVertexArray &to, vaName va, bool driverAllowsVertexAttributeArraysWithoutBoundBuffer)
    {
      GLint vaInt = static_cast<GLint>(va);
      RegalAssert(vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays));
      if (buffer || driverAllowsVertexAttributeArraysWithoutBoundBuffer)
      {
        if (vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays))
        {
          if (vaInt < 7)
          {
            if (enabled != to.enabled)
            {
              enabled = to.enabled;
              enable(dt,ctx,vaEnum[vaInt][0],enabled);
            }
            if (buffer != to.buffer)
            {
              buffer = to.buffer;
              dt.glBindBuffer(ctx, GL_ARRAY_BUFFER,buffer);
            }
            if (size != to.size ||
                type != to.type ||
                stride != to.stride ||
                pointer != to.pointer)
            {
              size = to.size;
              type = to.type;
              stride = to.stride;
              pointer = to.pointer;
              switch (vaInt)
              {
                case VERTEX:
                  dt.glVertexPointer(ctx, size,type,stride,pointer);
                  break;
                case NORMAL:
                  dt.glNormalPointer(ctx, type,stride,pointer);
                  break;
                case FOG_COORD:
                  dt.glFogCoordPointer(ctx, type,stride,pointer);
                  break;
                case COLOR:
                  dt.glColorPointer(ctx, size,type,stride,pointer);
                  break;
                case SECONDARY_COLOR:
                  dt.glSecondaryColorPointer(ctx, size,type,stride,pointer);
                  break;
                case INDEX:
                  dt.glIndexPointer(ctx, type,stride,pointer);
                  break;
                case EDGE_FLAG:
                  dt.glEdgeFlagPointer(ctx, stride,pointer);
                  break;
                default:
                  break;
              }
            }
          }
          else
          {
            GLuint index = static_cast<GLuint>(vaInt - 7);
            if (enabled != to.enabled)
            {
              enabled = to.enabled;
              enablei(dt,ctx,vaEnum[7][0], index, enabled);
            }
            if (buffer != to.buffer)
            {
              buffer = to.buffer;
              dt.glBindBuffer(ctx, GL_ARRAY_BUFFER,buffer);
            }
            if (size != to.size ||
                type != to.type ||
                stride != to.stride ||
                pointer != to.pointer)
            {
              size = to.size;
              type = to.type;
              stride = to.stride;
              pointer = to.pointer;
              dt.glMultiTexCoordPointerEXT(ctx, GL_TEXTURE0+index,size,type,stride,pointer);
            }
          }
        }
      }
    }

    std::string toString(vaName va, const char *delim = "\n") const
    {
      string_list tmp;

      GLint vaInt = static_cast<GLint>(va);
      RegalAssert(vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays));
      if (vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays))
      {
        if (vaInt < 7)
        {
          enableToString(tmp, enabled, Token::toString(vaEnum[vaInt][0]), delim);
          tmp << print_string("glBindBuffer(GL_ARRAY_BUFFER,",buffer,");",delim);
          switch (vaInt)
          {
            case VERTEX:
              tmp << print_string("glVertexPointer(",size,",",type,",",stride,",0x",pointer,");",delim);
              break;
            case NORMAL:
              tmp << print_string("glNormalPointer(",type,",",stride,",0x",pointer,");",delim);
              break;
            case FOG_COORD:
              tmp << print_string("glFogCoordPointer(",type,",",stride,",0x",pointer,");",delim);
              break;
            case COLOR:
              tmp << print_string("glColorPointer(",size,",",type,",",stride,",0x",pointer,");",delim);
              break;
            case SECONDARY_COLOR:
              tmp << print_string("glSecondaryColorPointer(",size,",",type,",",stride,",0x",pointer,");",delim);
              break;
            case INDEX:
              tmp << print_string("glIndexPointer(",type,",",stride,",0x",pointer,");",delim);
              break;
            case EDGE_FLAG:
              tmp << print_string("glEdgeFlagPointer(",stride,",0x",pointer,");",delim);
              break;
            default:
              break;
          }
        }
        else
        {
          GLuint index = static_cast<GLuint>(vaInt - 7);
          enableiToString(tmp, enabled, Token::toString(vaEnum[vaInt][0]), index, delim);
          tmp << print_string("glBindBuffer(GL_ARRAY_BUFFER,",buffer,");",delim);
          tmp << print_string("glMultiTexCoordPointerEXT(",Token::toString(GL_TEXTURE0+index),",",size,",",type,",",stride,",0x",pointer,");",delim);
        }
      }
      return tmp.str();
    }
  };

  struct VertexBufferBindPoint
  {
    GLuint    buffer;   // GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING
    GLintptr  offset;   // GL_VERTEX_ATTRIB_RELATIVE_OFFSET
    GLsizei   stride;   // GL_VERTEX_ATTRIB_ARRAY_STRIDE
    GLuint    divisor;  // GL_VERTEX_ATTRIB_ARRAY_DIVISOR

    inline VertexBufferBindPoint()
    {
      Reset();
    }

    inline void Reset()
    {
      buffer = 0;
      offset = 0;
      stride = 16;
      divisor = 0;
    }

    inline VertexBufferBindPoint &swap(VertexBufferBindPoint &other)
    {
      std::swap(buffer,other.buffer);
      std::swap(offset,other.offset);
      std::swap(stride,other.stride);
      std::swap(divisor,other.divisor);
      return *this;
    }

    template <typename Procs>
    VertexBufferBindPoint &get(Procs &dt, RegalContext * ctx, GLuint index)
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS)
      {
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,reinterpret_cast<GLint*>(&buffer));
        dt.glGetVertexAttribPointerv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_POINTER,reinterpret_cast<GLvoid **>(&offset));
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_STRIDE,reinterpret_cast<GLint*>(&stride));
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_DIVISOR,reinterpret_cast<GLint*>(&divisor));
      }
      return *this;
    }

    template <typename Procs>
    const VertexBufferBindPoint &set(Procs &dt, RegalContext * ctx, GLuint index) const
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS)
      {
        dt.glBindVertexBuffer(ctx, index,buffer,offset,stride);
        dt.glVertexBindingDivisor(ctx, index,divisor);
      }
      return *this;
    }

    template <typename Procs>
    void transition(Procs &dt, RegalContext * ctx, VertexBufferBindPoint &to, GLuint index)
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS)
      {
        if (buffer != to.buffer ||
            offset != to.offset ||
            stride != to.stride)
        {
          buffer = to.buffer;
          offset = to.offset;
          stride = to.stride;
          dt.glBindVertexBuffer(ctx, index,buffer,offset,stride);
        }
        if (divisor != to.divisor)
        {
          divisor = to.divisor;
          dt.glVertexBindingDivisor(ctx, index,divisor);
        }
      }
    }

    std::string toString(GLuint index, const char *delim = "\n") const
    {
      string_list tmp;
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS)
      {
        tmp << print_string("glBindVertexBuffer(",index,",",buffer,",",offset,",",stride,");",delim);
        tmp << print_string("glVertexBindingDivisor(",index,",",divisor,");",delim);
      }
      return tmp.str();
    }
  };

  struct GenericVertexArray
  {
    GLboolean  enabled;         // GL_VERTEX_ATTRIB_ARRAY_ENABLED
    GLuint     size;            // GL_VERTEX_ATTRIB_ARRAY_SIZE
    GLenum     type;            // GL_VERTEX_ATTRIB_ARRAY_TYPE
    GLuint     relativeOffset;  // GL_VERTEX_ATTRIB_RELATIVE_OFFSET
    GLboolean  normalized;      // GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
    GLboolean  isInteger;       // GL_VERTEX_ATTRIB_ARRAY_INTEGER
    GLboolean  isLong;          // GL_VERTEX_ATTRIB_ARRAY_LONG
    GLuint     bindingIndex;    // GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING

    inline GenericVertexArray()
    {
      Reset(0);
    }

    inline void Reset(GLuint index)
    {
      enabled = GL_FALSE;
      size = 4;
      type = GL_FLOAT;
      relativeOffset = 0;
      normalized = GL_FALSE;
      isInteger = GL_FALSE;
      isLong = GL_FALSE;
      bindingIndex = index;
    }

    inline GenericVertexArray &swap(GenericVertexArray &other)
    {
      std::swap(enabled,other.enabled);
      std::swap(size,other.size);
      std::swap(type,other.type);
      std::swap(relativeOffset,other.relativeOffset);
      std::swap(normalized,other.normalized);
      std::swap(isInteger,other.isInteger);
      std::swap(isLong,other.isLong);
      std::swap(bindingIndex,other.bindingIndex);
      return *this;
    }

    template <typename Procs>
    GenericVertexArray &get(Procs &dt, RegalContext * ctx, GLuint index)
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIBS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIBS)
      {
        GLint val;

        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&val);
        enabled = static_cast<GLboolean>(val);
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_SIZE,reinterpret_cast<GLint*>(&size));
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_TYPE,reinterpret_cast<GLint*>(&type));
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_RELATIVE_OFFSET,reinterpret_cast<GLint*>(&relativeOffset));
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,&val);
        normalized = static_cast<GLboolean>(val);
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_INTEGER,&val);
        isInteger = static_cast<GLboolean>(val);
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_LONG,&val);
        isLong = static_cast<GLboolean>(val);
        dt.glGetVertexAttribiv(ctx, index,GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,reinterpret_cast<GLint*>(&bindingIndex));
      }
      return *this;
    }

    template <typename Procs>
    const GenericVertexArray &set(Procs &dt, RegalContext * ctx, GLuint index) const
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIBS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIBS)
      {
        if (enabled)
          dt.glEnableVertexAttribArray(ctx, index);
        else
          dt.glDisableVertexAttribArray(ctx, index);
        if (isInteger)
          dt.glVertexAttribIFormat(ctx, index,size,type,relativeOffset);
        else if (isLong)
          dt.glVertexAttribLFormat(ctx, index,size,type,relativeOffset);
        else
          dt.glVertexAttribFormat(ctx, index,size,type,normalized,relativeOffset);
        dt.glVertexAttribBinding(ctx, index,bindingIndex);
      }
      return *this;
    }

    template <typename Procs>
    void transition(Procs &dt, RegalContext * ctx, GenericVertexArray &to, GLuint index)
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIBS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIBS)
      {
        if (enabled != to.enabled)
        {
          enabled = to.enabled;
          if (enabled)
            dt.glEnableVertexAttribArray(ctx, index);
          else
            dt.glDisableVertexAttribArray(ctx, index);
        }
        if (isInteger != to.isInteger ||
            isLong != to.isLong ||
            normalized != to.normalized ||
            size != to.size ||
            type != to.type ||
            relativeOffset != to.relativeOffset)
        {
          isInteger = to.isInteger;
          isLong = to.isLong;
          normalized = to.normalized;
          size = to.size;
          type = to.type;
          relativeOffset = to.relativeOffset;

          if (isInteger)
            dt.glVertexAttribIFormat(ctx, index,size,type,relativeOffset);
          else if (isLong)
            dt.glVertexAttribLFormat(ctx, index,size,type,relativeOffset);
          else
            dt.glVertexAttribFormat(ctx, index,size,type,normalized,relativeOffset);
        }
        if (bindingIndex != to.bindingIndex)
        {
          bindingIndex = to.bindingIndex;
          dt.glVertexAttribBinding(ctx, index,bindingIndex);
        }
      }
    }

    std::string toString(GLuint index, const char *delim = "\n") const
    {
      string_list tmp;

      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIBS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIBS)
      {
        if (enabled)
          tmp << print_string("glEnableVertexAttribArray(",index,");",delim);
        else
          tmp << print_string("glDisableVertexAttribArray(",index,");",delim);
        if (isInteger)
          tmp << print_string("glVertexAttribIFormat(",index,",",size,",",type,",",relativeOffset,");",delim);
        else if (isLong)
          tmp << print_string("glVertexAttribLFormat(",index,",",size,",",type,",",relativeOffset,");",delim);
        else
          tmp << print_string("glVertexAttribFormat(",index,",",size,",",type,",",normalized,",",relativeOffset,");",delim);
        tmp << print_string("glVertexAttribBinding(",index,",",bindingIndex,");",delim);
      }
      return tmp.str();
    }
  };

  struct VertexArray
  {
    NamedVertexArray      named[nNamedArrays];
    VertexBufferBindPoint bindings[REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS];
    GenericVertexArray    generic[REGAL_EMU_MAX_VERTEX_ATTRIBS];

    GLuint     elementArrayBufferBinding;        // GL_ELEMENT_ARRAY_BUFFER_BINDING

    // Table 23.8. Vertex Array Data (not in Vertex Array objects)
    //
    //  dsn: primitiveRestartFixedIndex is not in this table but should be (khronos bug 10250)

    GLenum     clientActiveTexture;              // GL_CLIENT_ACTIVE_TEXTURE
    GLboolean  primitiveRestartFixedIndex;       // GL_PRIMITIVE_RESTART_FIXED_INDEX
    GLboolean  primitiveRestart;                 // GL_PRIMITIVE_RESTART
    GLuint     primitiveRestartIndex;            // GL_PRIMITIVE_RESTART_INDEX
    GLuint     arrayBufferBinding;               // GL_ARRAY_BUFFER_BINDING
    GLuint     vertexArrayBinding;               // GL_VERTEX_ARRAY_BINDING

    inline VertexArray()
    {
      Reset();
    }

    void Reset()
    {
      elementArrayBufferBinding = 0;
      clientActiveTexture = GL_TEXTURE0;
      primitiveRestartFixedIndex = GL_FALSE;
      primitiveRestart = GL_FALSE;
      primitiveRestartIndex = 0;
      arrayBufferBinding = 0;
      vertexArrayBinding = 0;

      size_t n = array_size( named );
      for (size_t ii=0; ii<n; ii++)
        named[ii].Reset();

      RegalAssertArrayIndex( named, NORMAL );
      RegalAssertArrayIndex( named, FOG_COORD );
      RegalAssertArrayIndex( named, SECONDARY_COLOR );
      RegalAssertArrayIndex( named, INDEX );
      RegalAssertArrayIndex( named, EDGE_FLAG );

      named[NORMAL].size = 3;
      named[FOG_COORD].size = 1;
      named[SECONDARY_COLOR].size = 3;
      named[INDEX].size = 1;
      named[EDGE_FLAG].size = 1;
      named[EDGE_FLAG].type = GL_BOOL;

      n = array_size( bindings );
      for (size_t ii=0; ii<n; ii++)
        bindings[ii].Reset();

      n = array_size( generic );
      for (GLuint ii=0; ii<n; ii++)
        generic[ii].Reset(ii);
    }

    VertexArray &swap(VertexArray &other)
    {
      size_t n = array_size( named );
      for (size_t ii=0; ii<n; ii++)
        named[ii].swap(other.named[ii]);
      n = array_size( bindings );
      for (size_t ii=0; ii<n; ii++)
        bindings[ii].swap(other.bindings[ii]);
      n = array_size( generic );
      for (size_t ii=0; ii<n; ii++)
        generic[ii].swap(other.generic[ii]);
      std::swap(elementArrayBufferBinding,other.elementArrayBufferBinding);
      std::swap(clientActiveTexture,other.clientActiveTexture);
      std::swap(primitiveRestartFixedIndex,other.primitiveRestartFixedIndex);
      std::swap(primitiveRestart,other.primitiveRestart);
      std::swap(primitiveRestartIndex,other.primitiveRestartIndex);
      std::swap(arrayBufferBinding,other.arrayBufferBinding);
      std::swap(vertexArrayBinding,other.vertexArrayBinding);
      return *this;
    }

    template <typename Procs>
    VertexArray &get(Procs &dt, RegalContext * ctx)
    {
      dt.glGetIntegerv(ctx, GL_VERTEX_ARRAY_BINDING,reinterpret_cast<GLint*>(&vertexArrayBinding));
      if (vertexArrayBinding)
        dt.glBindVertexArray(ctx, 0);
      size_t n = array_size( named );
      for (size_t ii=0; ii<n; ii++)
        named[ii].get(dt,static_cast<vaName>(ii));
      n = array_size( bindings );
      for (size_t ii=0; ii<n; ii++)
        bindings[ii].get(dt,static_cast<GLuint>(ii));
      n = array_size( generic );
      for (size_t ii=0; ii<n; ii++)
        generic[ii].get(dt,static_cast<GLuint>(ii));
      dt.glGetIntegerv(ctx, GL_ELEMENT_ARRAY_BUFFER_BINDING,reinterpret_cast<GLint*>(&elementArrayBufferBinding));
      dt.glGetIntegerv(ctx, GL_CLIENT_ACTIVE_TEXTURE,reinterpret_cast<GLint*>(&clientActiveTexture));
      primitiveRestartFixedIndex = dt.glIsEnabled(ctx, GL_PRIMITIVE_RESTART_FIXED_INDEX);
      primitiveRestart = dt.glIsEnabled(ctx, GL_PRIMITIVE_RESTART);
      dt.glGetIntegerv(ctx, GL_PRIMITIVE_RESTART_INDEX,reinterpret_cast<GLint*>(&primitiveRestartIndex));
      dt.glGetIntegerv(ctx, GL_ARRAY_BUFFER_BINDING,reinterpret_cast<GLint*>(&arrayBufferBinding));
      if (vertexArrayBinding)
        dt.glBindVertexArray(ctx, vertexArrayBinding);
      return *this;
    }

    template <typename Procs>
    const VertexArray &set(Procs &dt, RegalContext * ctx, bool driverAllowsVertexAttributeArraysWithoutBoundBuffer) const
    {
      dt.glBindVertexArray(ctx, 0);
      size_t n = array_size( named );
      for (size_t ii=0; ii<n; ii++)
        named[n-ii-1].set(dt,ctx,static_cast<vaName>(n-ii-1),driverAllowsVertexAttributeArraysWithoutBoundBuffer);
      n = array_size( bindings );
      for (size_t ii=0; ii<n; ii++)
        bindings[ii].set(dt,ctx,static_cast<GLuint>(ii));
      n = array_size( generic );
      for (size_t ii=0; ii<n; ii++)
        generic[n-ii-1].set(dt,ctx,static_cast<GLuint>(n-ii-1));
      dt.glBindBuffer(ctx, GL_ELEMENT_ARRAY_BUFFER,elementArrayBufferBinding);
      dt.glClientActiveTexture(ctx, clientActiveTexture);
      if (primitiveRestartFixedIndex)
        dt.glEnable(ctx, GL_PRIMITIVE_RESTART_FIXED_INDEX);
      else
        dt.glDisable(ctx, GL_PRIMITIVE_RESTART_FIXED_INDEX);
      if (primitiveRestart)
        dt.glEnable(ctx, GL_PRIMITIVE_RESTART);
      else
        dt.glDisable(ctx, GL_PRIMITIVE_RESTART);
      dt.glPrimitiveRestartIndex(ctx, primitiveRestartIndex);
      dt.glBindBuffer(ctx, GL_ARRAY_BUFFER,arrayBufferBinding);
      dt.glBindVertexArray(ctx, vertexArrayBinding);
      return *this;
    }

    template <typename Procs>
    void transition(Procs &dt, RegalContext * ctx, VertexArray &to, bool driverAllowsVertexAttributeArraysWithoutBoundBuffer)
    {
      GLuint tmpVertexArrayBinding = to.vertexArrayBinding;
      if (vertexArrayBinding) {
        vertexArrayBinding = 0;
        dt.glBindVertexArray(ctx,vertexArrayBinding);
      }
      size_t n = array_size( named );
      for (size_t ii=0; ii<n; ii++)
        named[n-ii-1].transition(dt,ctx,to.named[n-ii-1],static_cast<vaName>(n-ii-1),driverAllowsVertexAttributeArraysWithoutBoundBuffer);
      n = array_size( bindings );
      for (size_t ii=0; ii<n; ii++)
        bindings[ii].transition(dt,ctx,to.bindings[ii],static_cast<GLuint>(ii));
      n = array_size( generic );
      for (size_t ii=0; ii<n; ii++)
        generic[n-ii-1].transition(dt,ctx,to.generic[n-ii-1],static_cast<GLuint>(n-ii-1));
      if (elementArrayBufferBinding != to.elementArrayBufferBinding) {
        elementArrayBufferBinding = to.elementArrayBufferBinding;
        dt.glBindBuffer(ctx,GL_ELEMENT_ARRAY_BUFFER,elementArrayBufferBinding);
      }
      if (clientActiveTexture != to.clientActiveTexture) {
        clientActiveTexture = to.clientActiveTexture;
        dt.glClientActiveTexture(ctx,clientActiveTexture);
      }
      if (primitiveRestartFixedIndex != to.primitiveRestartFixedIndex) {
        primitiveRestartFixedIndex = to.primitiveRestartFixedIndex;
        if (primitiveRestartFixedIndex)
          dt.glEnable(ctx,GL_PRIMITIVE_RESTART_FIXED_INDEX);
        else
          dt.glDisable(ctx,GL_PRIMITIVE_RESTART_FIXED_INDEX);
      }
      if (primitiveRestart != to.primitiveRestart) {
        primitiveRestart = to.primitiveRestart;
        if (primitiveRestart)
          dt.glEnable(ctx,GL_PRIMITIVE_RESTART);
        else
          dt.glDisable(ctx,GL_PRIMITIVE_RESTART);
      }
      if (primitiveRestartIndex != to.primitiveRestartIndex) {
        primitiveRestartIndex = to.primitiveRestartIndex;
        dt.glPrimitiveRestartIndex(ctx,primitiveRestartIndex);
      }
      if (arrayBufferBinding != to.arrayBufferBinding) {
        arrayBufferBinding = to.arrayBufferBinding;
        dt.glBindBuffer(ctx,GL_ARRAY_BUFFER,arrayBufferBinding);
      }
      if (vertexArrayBinding != tmpVertexArrayBinding) {
        vertexArrayBinding = tmpVertexArrayBinding;
        dt.glBindVertexArray(ctx,vertexArrayBinding);
      }
    }

    std::string toString(const char *delim = "\n") const
    {
      string_list tmp;
      size_t n = array_size( named );
      for (size_t ii=0; ii<n; ii++)
        named[ii].toString(static_cast<vaName>(ii),delim);
      n = array_size( bindings );
      for (size_t ii=0; ii<n; ii++)
        bindings[ii].toString(static_cast<GLuint>(ii),delim);
      n = array_size( generic );
      for (size_t ii=0; ii<n; ii++)
        generic[ii].toString(static_cast<GLuint>(ii),delim);

      tmp << print_string("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER",elementArrayBufferBinding,");",delim);
      tmp << print_string("glClientActiveTexture(",Token::GLtextureToString(clientActiveTexture),");",delim);

      if (primitiveRestartFixedIndex)
        tmp << print_string("glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);",delim);
      else
        tmp << print_string("glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);",delim);
      if (primitiveRestart)
        tmp << print_string("glEnable(GL_PRIMITIVE_RESTART);",delim);
      else
        tmp << print_string("glDisable(GL_PRIMITIVE_RESTART);",delim);
      tmp << print_string("glPrimitiveRestartIndex(",primitiveRestartIndex,");",delim);
      tmp << print_string("glBindBuffer(GL_ARRAY_BUFFER,",arrayBufferBinding,");",delim);
      tmp << print_string("glBindVertexArray(",vertexArrayBinding,");",delim);
      return tmp.str();
    }

    void SetEnablei(GLenum cap, GLuint index, GLboolean enabled)
    {
      UNUSED_PARAMETER(index);
      switch (cap)
      {
        case GL_PRIMITIVE_RESTART_FIXED_INDEX:
          primitiveRestartFixedIndex = enabled;
          break;
        case GL_PRIMITIVE_RESTART:
          primitiveRestart = enabled;
          break;
        default:
          break;
      }
    }

    inline void glEnable(GLenum cap)
    {
      SetEnablei(cap, 0, GL_TRUE);
    }

    inline void glDisable(GLenum cap)
    {
      SetEnablei(cap, 0, GL_FALSE);
    }

    inline void glEnablei(GLenum cap, GLuint index)
    {
      SetEnablei(cap, index, GL_TRUE);
    }

    inline void glDisablei(GLenum cap, GLuint index)
    {
      SetEnablei(cap, index, GL_FALSE);
    }

    inline void glEnableIndexedEXT(GLenum cap, GLuint index)
    {
      SetEnablei(cap, index, GL_TRUE);
    }

    inline void glDisableIndexedEXT(GLenum cap, GLuint index)
    {
      SetEnablei(cap, index, GL_FALSE);
    }

    void SetEnableClientStatei(GLuint vao, GLenum cap, GLuint index, GLboolean enabled)
    {
      // only handle these if no VAO is bound

      if (vao)
        return;

      switch (cap)
      {
        case GL_VERTEX_ARRAY:
          RegalAssertArrayIndex( named, VERTEX );
          named[VERTEX].enabled = enabled;
          break;
        case GL_NORMAL_ARRAY:
          RegalAssertArrayIndex( named, NORMAL );
          named[NORMAL].enabled = enabled;
          break;
        case GL_FOG_COORD_ARRAY:
          RegalAssertArrayIndex( named, FOG_COORD );
          named[FOG_COORD].enabled = enabled;
          break;
        case GL_COLOR_ARRAY:
          RegalAssertArrayIndex( named, COLOR );
          named[COLOR].enabled = enabled;
          break;
        case GL_SECONDARY_COLOR_ARRAY:
          RegalAssertArrayIndex( named, SECONDARY_COLOR );
          named[SECONDARY_COLOR].enabled = enabled;
          break;
        case GL_INDEX_ARRAY:
          RegalAssertArrayIndex( named, INDEX );
          named[INDEX].enabled = enabled;
          break;
        case GL_EDGE_FLAG_ARRAY:
          RegalAssertArrayIndex( named, EDGE_FLAG );
          named[EDGE_FLAG].enabled = enabled;
          break;
        case GL_TEXTURE_COORD_ARRAY:
          if ( (index+7) < array_size( named ) )
          {
            RegalAssertArrayIndex( named, index+7 );
            named[index+7].enabled = enabled;
          }
          break;
        default:
          break;
      }
    }

    inline void glEnableClientState(GLenum cap)
    {
      if (cap == GL_TEXTURE_COORD_ARRAY)
        SetEnableClientStatei(vertexArrayBinding, cap, clientActiveTexture - GL_TEXTURE0, GL_TRUE);
      else
        SetEnableClientStatei(vertexArrayBinding, cap, 0, GL_TRUE);
    }

    inline void glDisableClientState(GLenum cap)
    {
      if (cap == GL_TEXTURE_COORD_ARRAY)
        SetEnableClientStatei(vertexArrayBinding, cap, clientActiveTexture - GL_TEXTURE0, GL_FALSE);
      else
        SetEnableClientStatei(vertexArrayBinding, cap, 0, GL_FALSE);
    }

    inline void glEnableClientStateiEXT(GLenum cap, GLuint index)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, index, GL_TRUE);
    }

    inline void glDisableClientStateiEXT(GLenum cap, GLuint index)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, index, GL_FALSE);
    }

    inline void glEnableClientStateIndexedEXT(GLenum cap, GLuint index)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, index, GL_TRUE);
    }

    inline void glDisableClientStateIndexedEXT(GLenum cap, GLuint index)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, index, GL_FALSE);
    }

    void glBindBuffer( GLenum target, GLuint buffer )
    {
      switch (target)
      {
        case GL_ARRAY_BUFFER:
          arrayBufferBinding = buffer;
          break;
        case GL_ELEMENT_ARRAY_BUFFER:
          if (!vertexArrayBinding)
            elementArrayBufferBinding = buffer;
          break;
        default:
          break;
      }
    }

    void glDeleteBuffers( GLsizei n, const GLuint *buffers )
    {
      for (GLsizei ii=0; ii<n; ii++)
      {
        if (buffers[ii] == arrayBufferBinding)
          arrayBufferBinding = 0;
        if (buffers[ii] == elementArrayBufferBinding)
          elementArrayBufferBinding = 0;
        size_t nBindings = array_size( bindings );
        for (size_t jj=0; jj<nBindings; jj++)
        {
          RegalAssertArrayIndex( bindings, jj );
          if (buffers[ii] == bindings[jj].buffer)
            bindings[jj].buffer = 0;
        }
      }
    }

    inline void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayVertexOffsetEXT(vertexArrayBinding, arrayBufferBinding, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayNormalOffsetEXT(vertexArrayBinding, arrayBufferBinding, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayColorOffsetEXT(vertexArrayBinding, arrayBufferBinding, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glSecondaryColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArraySecondaryColorOffsetEXT(vertexArrayBinding, arrayBufferBinding, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glIndexPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayIndexOffsetEXT(vertexArrayBinding, arrayBufferBinding, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glEdgeFlagPointer(GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayEdgeFlagOffsetEXT(vertexArrayBinding, arrayBufferBinding, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glFogCoordPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayFogCoordOffsetEXT(vertexArrayBinding, arrayBufferBinding, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glMultiTexCoordPointerEXT(GLenum index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayMultiTexCoordOffsetEXT(vertexArrayBinding, arrayBufferBinding, index, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayMultiTexCoordOffsetEXT(vertexArrayBinding, arrayBufferBinding, clientActiveTexture, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    GLsizei computeEffectiveStride(GLsizei stride, GLint size, GLenum type)
    {
      if (stride != 0)
        return stride;

      // when stride == 0, compute effectiveStride based on size and type

      if (size == GL_BGRA)
        return 4;

      switch (type)
      {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
          break;
        case GL_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
          // size must be 4 for these. Just return that.
          break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_HALF_FLOAT:
          size *= 2;
          break;
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_FLOAT:
        case GL_FIXED:
          size *= 4;
          break;
        case GL_DOUBLE:
          size *= 8;
          break;
        default:
          size = 0;
          break;
      }

      return size;
    }

    inline void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
    {
      if (!vertexArrayBinding &&
          index < array_size( generic ) &&
          index < array_size( bindings ))
        glVertexArrayVertexAttribOffsetEXT(vertexArrayBinding, arrayBufferBinding, index, size, type, normalized, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
    {
      if (!vertexArrayBinding &&
          index < array_size( generic ) &&
          index < array_size( bindings ))
        glVertexArrayVertexAttribIOffsetEXT(vertexArrayBinding, arrayBufferBinding, index, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    void glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
    {
      if (!vertexArrayBinding &&
          index < array_size( generic ) &&
          index < array_size( bindings ))
      {
        glVertexAttribLFormat(index, size, type, 0);
        glVertexAttribBinding(index, index);
        GLsizei effectiveStride = computeEffectiveStride(stride, size, type);
        glBindVertexBuffer(index, arrayBufferBinding, (char *)pointer - (char *)NULL, effectiveStride);
      }
    }

    void glBindVertexBuffers( GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides )
    {
      if (!vertexArrayBinding && ((first + count) >= first) && (first + count) <= array_size( bindings ))
      {
        if (buffers)
          for (GLsizei ii = 0; ii < count; ii++)
          {
            RegalAssertArrayIndex( bindings, first );
            bindings[first].buffer = buffers[ii];
            bindings[first].offset = offsets[ii];
            bindings[first].stride = strides[ii];
            first++;
          }
        else
          for (GLsizei ii = 0; ii < count; ii++)
          {
            RegalAssertArrayIndex( bindings, first );
            bindings[first].buffer = 0;
            bindings[first].offset = 0;
            bindings[first].stride = 16;
            first++;
          }
      }
    }

    void glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
    {
      glBindVertexBuffers( bindingindex, 1, &buffer, &offset, &stride );
    }

    inline void glVertexAttribDivisor(GLuint index, GLuint divisor)
    {
      if (!vertexArrayBinding &&
          index < array_size( generic ) &&
          index < array_size( bindings ) )
      {
        RegalAssertArrayIndex( generic, index );
        RegalAssertArrayIndex( bindings, index );
        generic[index].bindingIndex = index;
        bindings[index].divisor = divisor;
      }
    }

    inline void glVertexBindingDivisor(GLuint bindingindex, GLuint divisor)
    {
      if (!vertexArrayBinding && bindingindex < array_size( bindings ))
      {
        RegalAssertArrayIndex( bindings, bindingindex );
        bindings[bindingindex].divisor = divisor;
      }
    }

    inline void glEnableVertexAttribArray(GLuint index)
    {
      if (!vertexArrayBinding && index < array_size( generic ))
      {
        RegalAssertArrayIndex( generic, index );
        generic[index].enabled = GL_TRUE;
      }
    }

    inline void glDisableVertexAttribArray(GLuint index)
    {
      if (!vertexArrayBinding && index < array_size( generic ))
      {
        RegalAssertArrayIndex( generic, index );
        generic[index].enabled = GL_FALSE;
      }
    }

    inline void glVertexAttribBinding(GLuint attribindex, GLuint bindingindex)
    {
      if (!vertexArrayBinding && attribindex < array_size( generic ))
      {
        RegalAssertArrayIndex( generic, attribindex );
        generic[attribindex].bindingIndex = bindingindex;
      }
    }

    inline void glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
    {
      if (!vertexArrayBinding && attribindex < array_size( generic ))
      {
        RegalAssertArrayIndex( generic, attribindex );
        generic[attribindex].size = size;
        generic[attribindex].type = type;
        generic[attribindex].relativeOffset = relativeoffset;
        generic[attribindex].normalized = GL_FALSE;
        generic[attribindex].isInteger = GL_TRUE;
        generic[attribindex].isLong = GL_FALSE;
      }
    }

    inline void glVertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
    {
      if (!vertexArrayBinding && attribindex < array_size( generic ))
      {
        RegalAssertArrayIndex( generic, attribindex );
        generic[attribindex].size = size;
        generic[attribindex].type = type;
        generic[attribindex].relativeOffset = relativeoffset;
        generic[attribindex].normalized = GL_FALSE;
        generic[attribindex].isInteger = GL_FALSE;
        generic[attribindex].isLong = GL_TRUE;
      }
    }

    inline void glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
    {
      if (!vertexArrayBinding && attribindex < array_size( generic ))
      {
        RegalAssertArrayIndex( generic, attribindex );
        generic[attribindex].size = size;
        generic[attribindex].type = type;
        generic[attribindex].relativeOffset = relativeoffset;
        generic[attribindex].normalized = normalized;
        generic[attribindex].isInteger = GL_FALSE;
        generic[attribindex].isLong = GL_FALSE;
      }
    }

    inline void glClientActiveTexture(GLenum texture)
    {
      GLuint index = texture - GL_TEXTURE0;
      if ( texture >= GL_TEXTURE0 && index < array_size( generic ))
        clientActiveTexture = texture;
      else
        Warning( "Client active texture out of range: ", Token::GLtextureToString(texture), " > ", Token::GLtextureToString(static_cast<GLenum>(GL_TEXTURE0 + array_size( generic ) - 1)));
    }

    inline void glPrimitiveRestartIndex(GLuint index)
    {
      primitiveRestartIndex = index;
    }

    inline void glBindVertexArray(GLuint array)
    {
      vertexArrayBinding = array;
    }

    inline void glDeleteVertexArrays( GLsizei n, const GLuint *arrays )
    {
      for (GLsizei ii=0; ii<n; ii++)
        if (arrays[ii] == vertexArrayBinding)
          vertexArrayBinding = 0;
    }

    inline void glEnableVertexArrayEXT(GLuint vaobj, GLenum array)
    {
      if (!vaobj)
      {
        if (array == GL_TEXTURE_COORD_ARRAY)
          SetEnableClientStatei(vaobj, array, clientActiveTexture - GL_TEXTURE0, GL_TRUE);
        else
          SetEnableClientStatei(vaobj, array, 0, GL_TRUE);
      }
    }

    inline void glDisableVertexArrayEXT(GLuint vaobj, GLenum array)
    {
      if (!vaobj)
      {
        if (array == GL_TEXTURE_COORD_ARRAY)
          SetEnableClientStatei(vaobj, array, clientActiveTexture - GL_TEXTURE0, GL_FALSE);
        else
          SetEnableClientStatei(vaobj, array, 0, GL_FALSE);
      }
    }

    inline void glEnableVertexArrayAttribEXT(GLuint vaobj, GLuint index)
    {
      if (!vaobj && index < array_size( generic ))
        generic[index].enabled = GL_TRUE;
    }

    inline void glDisableVertexArrayAttribEXT(GLuint vaobj, GLuint index)
    {
      if (!vaobj && index < array_size( generic ))
        generic[index].enabled = GL_FALSE;
    }

    void glVertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        RegalAssertArrayIndex( named, VERTEX );
        named[VERTEX].buffer = buffer;
        named[VERTEX].size = size;
        named[VERTEX].type = type;
        named[VERTEX].stride = stride;
        named[VERTEX].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        RegalAssertArrayIndex( named, NORMAL );
        named[NORMAL].buffer = buffer;
        named[NORMAL].size = 3;
        named[NORMAL].type = type;
        named[NORMAL].stride = stride;
        named[NORMAL].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        RegalAssertArrayIndex( named, COLOR );
        named[COLOR].buffer = buffer;
        named[COLOR].size = size;
        named[COLOR].type = type;
        named[COLOR].stride = stride;
        named[COLOR].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        RegalAssertArrayIndex( named, SECONDARY_COLOR );
        named[SECONDARY_COLOR].buffer = buffer;
        named[SECONDARY_COLOR].size = size;
        named[SECONDARY_COLOR].type = type;
        named[SECONDARY_COLOR].stride = stride;
        named[SECONDARY_COLOR].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        RegalAssertArrayIndex( named, INDEX );
        named[INDEX].buffer = buffer;
        named[INDEX].size = 1;
        named[INDEX].type = type;
        named[INDEX].stride = stride;
        named[INDEX].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayEdgeFlagOffsetEXT(GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        RegalAssertArrayIndex( named, EDGE_FLAG );
        named[EDGE_FLAG].buffer = buffer;
        named[EDGE_FLAG].size = 1;
        named[EDGE_FLAG].type = GL_BOOL;
        named[EDGE_FLAG].stride = stride;
        named[EDGE_FLAG].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        RegalAssertArrayIndex( named, FOG_COORD );
        named[FOG_COORD].buffer = buffer;
        named[FOG_COORD].size = 1;
        named[FOG_COORD].type = type;
        named[FOG_COORD].stride = stride;
        named[FOG_COORD].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum texture, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj && texture >= GL_TEXTURE0)
      {
        GLuint index = texture - GL_TEXTURE0;
        if ( (index+7) < array_size( named ) )
        {
          GLuint ii = 7 + index;
          RegalAssertArrayIndex( named, ii );
          named[ii].buffer = buffer;
          named[ii].size = size;
          named[ii].type = type;
          named[ii].stride = stride;
          named[ii].pointer = reinterpret_cast<const GLvoid *>(offset);
        }
      }
    }

    inline void glVertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      glVertexArrayMultiTexCoordOffsetEXT(vaobj, buffer, clientActiveTexture, size, type, stride, offset);
    }

    void glVertexArrayVertexAttribOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        GLuint originalVABinding = vertexArrayBinding;
        vertexArrayBinding = 0;
        glVertexAttribFormat(index, size, type, normalized, 0);
        glVertexAttribBinding(index, index);
        GLsizei effectiveStride = computeEffectiveStride(stride, size, type);
        glBindVertexBuffers(index, 1, &buffer, &offset, &effectiveStride);
        vertexArrayBinding = originalVABinding;
      }
    }

    void glVertexArrayVertexAttribIOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        GLuint originalVABinding = vertexArrayBinding;
        vertexArrayBinding = 0;
        glVertexAttribIFormat(index, size, type, 0);
        glVertexAttribBinding(index, index);
        GLsizei effectiveStride = computeEffectiveStride(stride, size, type);
        glBindVertexBuffers(index, 1, &buffer, &offset, &effectiveStride);
        vertexArrayBinding = originalVABinding;
      }
    }

    void glInterleavedArrays(GLenum format, GLsizei stride, const GLvoid *pointer)
    {
      if (vertexArrayBinding)
        return;

      // from glspec43.compatibility.20130214.withchanges.pdf
      // p. 365, section 10.5.1 "Interleaved Arrays"
      //
      // set et; ec; en; st; sc; sv; tc; pc; pn; pv; and s as a function
      // of table 10.6 and the value of format.

      GLboolean et, ec, en;
      GLenum    tc;
      GLsizei   st, sc, sv;
      GLsizei   pc, pn, pv;
      GLsizei   s;

      const int f = sizeof(GLfloat);
      const int c = int( float( 4 * sizeof(GLubyte) ) / float(f) + float(0.5) ) * f;

      switch(format)
      {
        case GL_V2F:
          et = GL_FALSE;
          ec = GL_FALSE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 0;
          sc = 0;
          sv = 2;
          pc = 0;
          pn = 0;
          pv = 0;
          pv = 0;
          s  = 2 * f;
          break;
        case GL_V3F:
          et = GL_FALSE;
          ec = GL_FALSE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 0;
          sc = 0;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = 0;
          s  = 3 * f;
          break;
        case GL_C4UB_V2F:
          et = GL_FALSE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_UNSIGNED_BYTE;
          st = 0;
          sc = 4;
          sv = 2;
          pc = 0;
          pn = 0;
          pv = c;
          s  = c + 2 * f;
          break;
        case GL_C4UB_V3F:
          et = GL_FALSE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_UNSIGNED_BYTE;
          st = 0;
          sc = 4;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = c;
          s  = c + 3 * f;
          break;
        case GL_C3F_V3F:
          et = GL_FALSE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 0;
          sc = 3;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = 3 * f;
          s  = 6 * f;
          break;
        case GL_N3F_V3F:
          et = GL_FALSE;
          ec = GL_FALSE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 0;
          sc = 0;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = 3 * f;
          s  = 6 * f;
          break;
        case GL_C4F_N3F_V3F:
          et = GL_FALSE;
          ec = GL_TRUE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 0;
          sc = 4;
          sv = 3;
          pc = 0;
          pn = 4 * f;
          pv = 7 * f;
          s  = 10 * f;
          break;
        case GL_T2F_V3F:
          et = GL_TRUE;
          ec = GL_FALSE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 2;
          sc = 0;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = 2 * f;
          s  = 5 * f;
          break;
        case GL_T4F_V4F:
          et = GL_TRUE;
          ec = GL_FALSE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 4;
          sc = 0;
          sv = 4;
          pc = 0;
          pn = 0;
          pv = 4 * f;
          s  = 8 * f;
          break;
        case GL_T2F_C4UB_V3F:
          et = GL_TRUE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_UNSIGNED_BYTE;
          st = 2;
          sc = 4;
          sv = 3;
          pc = 2 * f;
          pn = 0;
          pv = c + 2 * f;
          s  = c + 5 * f;
          break;
        case GL_T2F_C3F_V3F:
          et = GL_TRUE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 2;
          sc = 3;
          sv = 3;
          pc = 2 * f;
          pn = 0;
          pv = 5 * f;
          s  = 8 * f;
          break;
        case GL_T2F_N3F_V3F:
          et = GL_TRUE;
          ec = GL_FALSE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 2;
          sc = 0;
          sv = 3;
          pc = 0;
          pn = 2 * f;
          pv = 5 * f;
          s  = 8 * f;
          break;
        case GL_T2F_C4F_N3F_V3F:
          et = GL_TRUE;
          ec = GL_TRUE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 2;
          sc = 4;
          sv = 3;
          pc = 2 * f;
          pn = 6 * f;
          pv = 9 * f;
          s  = 12 * f;
          break;
        case GL_T4F_C4F_N3F_V4F:
          et = GL_TRUE;
          ec = GL_TRUE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 4;
          sc = 4;
          sv = 4;
          pc = 4 * f;
          pn = 8 * f;
          pv = 11 * f;
          s  = 15 * f;
          break;
        default:
          Warning( "Unhandled format value. format = 0x", print_hex(format), " (", Token::GLenumToString(format), ")");
          return;
      }

      if ( stride == 0 )
        stride = s;

      glDisableClientState(GL_EDGE_FLAG_ARRAY);
      glDisableClientState(GL_INDEX_ARRAY);
      glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
      glDisableClientState(GL_FOG_COORD_ARRAY);

      GLubyte *p = static_cast<GLubyte *>( const_cast<GLvoid *>(pointer) );

      if (et)
      {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(st, GL_FLOAT, stride, p);
      }
      else
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

      if (ec)
      {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(sc, tc, stride, p + pc);
      }
      else
        glDisableClientState(GL_COLOR_ARRAY);

      if (en)
      {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, stride, p + pn);
      }
      else
        glDisableClientState(GL_NORMAL_ARRAY);

      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(sv, GL_FLOAT, stride, p + pv);
    }
  };

  struct PixelStore
  {
    GLboolean  unpackSwapBytes;           // GL_UNPACK_SWAP_BYTES
    GLboolean  unpackLsbFirst;            // GL_UNPACK_LSB_FIRST
    GLint      unpackImageHeight;         // GL_UNPACK_IMAGE_HEIGHT
    GLint      unpackSkipImages;          // GL_UNPACK_SKIP_IMAGES
    GLint      unpackRowLength;           // GL_UNPACK_ROW_LENGTH
    GLint      unpackSkipRows;            // GL_UNPACK_SKIP_ROWS
    GLint      unpackSkipPixels;          // GL_UNPACK_SKIP_PIXELS
    GLint      unpackAlignment;           // GL_UNPACK_ALIGNMENT
    GLboolean  packSwapBytes;             // GL_PACK_SWAP_BYTES
    GLboolean  packLsbFirst;              // GL_PACK_LSB_FIRST
    GLint      packImageHeight;           // GL_PACK_IMAGE_HEIGHT
    GLint      packSkipImages;            // GL_PACK_SKIP_IMAGES
    GLint      packRowLength;             // GL_PACK_ROW_LENGTH
    GLint      packSkipRows;              // GL_PACK_SKIP_ROWS
    GLint      packSkipPixels;            // GL_PACK_SKIP_PIXELS
    GLint      packAlignment;             // GL_PACK_ALIGNMENT
    GLuint     pixelUnpackBufferBinding;  // GL_PIXEL_UNPACK_BUFFER_BINDING
    GLuint     pixelPackBufferBinding;    // GL_PIXEL_PACK_BUFFER_BINDING

    inline PixelStore()
    {
      Reset();
    }

    inline void Reset()
    {
      unpackSwapBytes = GL_FALSE;
      unpackLsbFirst = GL_FALSE;
      unpackImageHeight = 0;
      unpackSkipImages = 0;
      unpackRowLength = 0;
      unpackSkipRows = 0;
      unpackSkipPixels = 0;
      unpackAlignment = 4;
      packSwapBytes = GL_FALSE;
      packLsbFirst = GL_FALSE;
      packImageHeight = 0;
      packSkipImages = 0;
      packRowLength = 0;
      packSkipRows = 0;
      packSkipPixels = 0;
      packAlignment = 4;
      pixelUnpackBufferBinding = 0;
      pixelPackBufferBinding = 0;
    }

    inline PixelStore &swap(PixelStore &other)
    {
      std::swap(unpackSwapBytes,other.unpackSwapBytes);
      std::swap(unpackLsbFirst,other.unpackLsbFirst);
      std::swap(unpackImageHeight,other.unpackImageHeight);
      std::swap(unpackSkipImages,other.unpackSkipImages);
      std::swap(unpackRowLength,other.unpackRowLength);
      std::swap(unpackSkipRows,other.unpackSkipRows);
      std::swap(unpackSkipPixels,other.unpackSkipPixels);
      std::swap(unpackAlignment,other.unpackAlignment);
      std::swap(packSwapBytes,other.packSwapBytes);
      std::swap(packLsbFirst,other.packLsbFirst);
      std::swap(packImageHeight,other.packImageHeight);
      std::swap(packSkipImages,other.packSkipImages);
      std::swap(packRowLength,other.packRowLength);
      std::swap(packSkipRows,other.packSkipRows);
      std::swap(packSkipPixels,other.packSkipPixels);
      std::swap(packAlignment,other.packAlignment);
      std::swap(pixelUnpackBufferBinding,other.pixelUnpackBufferBinding);
      std::swap(pixelPackBufferBinding,other.pixelPackBufferBinding);
      return *this;
    }

    template <typename Procs>
    PixelStore &get(Procs &dt, RegalContext * ctx)
    {
      dt.glGetBooleanv(ctx, GL_UNPACK_SWAP_BYTES,&unpackSwapBytes);
      dt.glGetBooleanv(ctx, GL_UNPACK_LSB_FIRST,&unpackLsbFirst);
      dt.glGetIntegerv(ctx, GL_UNPACK_IMAGE_HEIGHT,&unpackImageHeight);
      dt.glGetIntegerv(ctx, GL_UNPACK_SKIP_IMAGES,&unpackSkipImages);
      dt.glGetIntegerv(ctx, GL_UNPACK_ROW_LENGTH,&unpackRowLength);
      dt.glGetIntegerv(ctx, GL_UNPACK_SKIP_ROWS,&unpackSkipRows);
      dt.glGetIntegerv(ctx, GL_UNPACK_SKIP_PIXELS,&unpackSkipPixels);
      dt.glGetIntegerv(ctx, GL_UNPACK_ALIGNMENT,&unpackAlignment);
      dt.glGetBooleanv(ctx, GL_PACK_SWAP_BYTES,&packSwapBytes);
      dt.glGetBooleanv(ctx, GL_PACK_LSB_FIRST,&packLsbFirst);
      dt.glGetIntegerv(ctx, GL_PACK_IMAGE_HEIGHT,&packImageHeight);
      dt.glGetIntegerv(ctx, GL_PACK_SKIP_IMAGES,&packSkipImages);
      dt.glGetIntegerv(ctx, GL_PACK_ROW_LENGTH,&packRowLength);
      dt.glGetIntegerv(ctx, GL_PACK_SKIP_ROWS,&packSkipRows);
      dt.glGetIntegerv(ctx, GL_PACK_SKIP_PIXELS,&packSkipPixels);
      dt.glGetIntegerv(ctx, GL_PACK_ALIGNMENT,&packAlignment);
      dt.glGetIntegerv(ctx, GL_PIXEL_UNPACK_BUFFER_BINDING,reinterpret_cast<GLint*>(&pixelUnpackBufferBinding));
      dt.glGetIntegerv(ctx, GL_PIXEL_PACK_BUFFER_BINDING,reinterpret_cast<GLint*>(&pixelPackBufferBinding));
      return *this;
    }

    template <typename Procs>
    const PixelStore &set(Procs &dt, RegalContext * ctx) const
    {
      dt.glPixelStorei(ctx, GL_UNPACK_SWAP_BYTES,unpackSwapBytes);
      dt.glPixelStorei(ctx, GL_UNPACK_LSB_FIRST,unpackLsbFirst);
      dt.glPixelStorei(ctx, GL_UNPACK_IMAGE_HEIGHT,unpackImageHeight);
      dt.glPixelStorei(ctx, GL_UNPACK_SKIP_IMAGES,unpackSkipImages);
      dt.glPixelStorei(ctx, GL_UNPACK_ROW_LENGTH,unpackRowLength);
      dt.glPixelStorei(ctx, GL_UNPACK_SKIP_ROWS,unpackSkipRows);
      dt.glPixelStorei(ctx, GL_UNPACK_SKIP_PIXELS,unpackSkipPixels);
      dt.glPixelStorei(ctx, GL_UNPACK_ALIGNMENT,unpackAlignment);
      dt.glPixelStorei(ctx, GL_PACK_SWAP_BYTES,packSwapBytes);
      dt.glPixelStorei(ctx, GL_PACK_LSB_FIRST,packLsbFirst);
      dt.glPixelStorei(ctx, GL_PACK_IMAGE_HEIGHT,packImageHeight);
      dt.glPixelStorei(ctx, GL_PACK_SKIP_IMAGES,packSkipImages);
      dt.glPixelStorei(ctx, GL_PACK_ROW_LENGTH,packRowLength);
      dt.glPixelStorei(ctx, GL_PACK_SKIP_ROWS,packSkipRows);
      dt.glPixelStorei(ctx, GL_PACK_SKIP_PIXELS,packSkipPixels);
      dt.glPixelStorei(ctx, GL_PACK_ALIGNMENT,packAlignment);
      dt.glBindBuffer(ctx, GL_PIXEL_UNPACK_BUFFER,pixelUnpackBufferBinding);
      dt.glBindBuffer(ctx, GL_PIXEL_PACK_BUFFER,pixelPackBufferBinding);
      return *this;
    }

    template <typename Procs>
    void transition(Procs &dt, RegalContext * ctx, PixelStore &to)
    {
      if (unpackSwapBytes != to.unpackSwapBytes) {
        unpackSwapBytes = to.unpackSwapBytes;
        dt.glPixelStorei(ctx, GL_UNPACK_SWAP_BYTES,unpackSwapBytes);
      }
      if (unpackLsbFirst != to.unpackLsbFirst) {
        unpackLsbFirst = to.unpackLsbFirst;
        dt.glPixelStorei(ctx, GL_UNPACK_LSB_FIRST,unpackLsbFirst);
      }
      if (unpackImageHeight != to.unpackImageHeight) {
        unpackImageHeight = to.unpackImageHeight;
        dt.glPixelStorei(ctx, GL_UNPACK_IMAGE_HEIGHT,unpackImageHeight);
      }
      if (unpackSkipImages != to.unpackSkipImages) {
        unpackSkipImages = to.unpackSkipImages;
        dt.glPixelStorei(ctx, GL_UNPACK_SKIP_IMAGES,unpackSkipImages);
      }
      if (unpackRowLength != to.unpackRowLength) {
        unpackRowLength = to.unpackRowLength;
        dt.glPixelStorei(ctx, GL_UNPACK_ROW_LENGTH,unpackRowLength);
      }
      if (unpackSkipRows != to.unpackSkipRows) {
        unpackSkipRows = to.unpackSkipRows;
        dt.glPixelStorei(ctx, GL_UNPACK_SKIP_ROWS,unpackSkipRows);
      }
      if (unpackSkipPixels != to.unpackSkipPixels) {
        unpackSkipPixels = to.unpackSkipPixels;
        dt.glPixelStorei(ctx, GL_UNPACK_SKIP_PIXELS,unpackSkipPixels);
      }
      if (unpackAlignment != to.unpackAlignment) {
        unpackAlignment = to.unpackAlignment;
        dt.glPixelStorei(ctx, GL_UNPACK_ALIGNMENT,unpackAlignment);
      }
      if (packSwapBytes != to.packSwapBytes) {
        packSwapBytes = to.packSwapBytes;
        dt.glPixelStorei(ctx, GL_PACK_SWAP_BYTES,packSwapBytes);
      }
      if (packLsbFirst != to.packLsbFirst) {
        packLsbFirst = to.packLsbFirst;
        dt.glPixelStorei(ctx, GL_PACK_LSB_FIRST,packLsbFirst);
      }
      if (packImageHeight != to.packImageHeight) {
        packImageHeight = to.packImageHeight;
        dt.glPixelStorei(ctx, GL_PACK_IMAGE_HEIGHT,packImageHeight);
      }
      if (packSkipImages != to.packSkipImages) {
        packSkipImages = to.packSkipImages;
        dt.glPixelStorei(ctx, GL_PACK_SKIP_IMAGES,packSkipImages);
      }
      if (packRowLength != to.packRowLength) {
        packRowLength = to.packRowLength;
        dt.glPixelStorei(ctx, GL_PACK_ROW_LENGTH,packRowLength);
      }
      if (packSkipRows != to.packSkipRows) {
        packSkipRows = to.packSkipRows;
        dt.glPixelStorei(ctx, GL_PACK_SKIP_ROWS,packSkipRows);
      }
      if (packSkipPixels != to.packSkipPixels) {
        packSkipPixels = to.packSkipPixels;
        dt.glPixelStorei(ctx, GL_PACK_SKIP_PIXELS,packSkipPixels);
      }
      if (packAlignment != to.packAlignment) {
        packAlignment = to.packAlignment;
        dt.glPixelStorei(ctx, GL_PACK_ALIGNMENT,packAlignment);
      }
      if (pixelUnpackBufferBinding != to.pixelUnpackBufferBinding) {
        pixelUnpackBufferBinding = to.pixelUnpackBufferBinding;
        dt.glBindBuffer(ctx, GL_PIXEL_UNPACK_BUFFER,pixelUnpackBufferBinding);
      }
      if (pixelPackBufferBinding != to.pixelPackBufferBinding) {
        pixelPackBufferBinding = to.pixelPackBufferBinding;
        dt.glBindBuffer(ctx, GL_PIXEL_PACK_BUFFER,pixelPackBufferBinding);
      }
    }

    std::string toString(const char *delim = "\n") const
    {
      string_list tmp;
      tmp << print_string("glPixelStorei(GL_UNPACK_SWAP_BYTES,",unpackSwapBytes,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_LSB_FIRST,",unpackLsbFirst,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_IMAGE_HEIGHT,",unpackImageHeight,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_SKIP_IMAGES,",unpackSkipImages,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_ROW_LENGTH,",unpackRowLength,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_SKIP_ROWS,",unpackSkipRows,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_SKIP_PIXELS,",unpackSkipPixels,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_ALIGNMENT,",unpackAlignment,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_SWAP_BYTES,",packSwapBytes,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_LSB_FIRST,",packLsbFirst,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_IMAGE_HEIGHT,",packImageHeight,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_SKIP_IMAGES,",packSkipImages,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_ROW_LENGTH,",packRowLength,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_SKIP_ROWS,",packSkipRows,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_SKIP_PIXELS,",packSkipPixels,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_ALIGNMENT,",packAlignment,");",delim);
      tmp << print_string("glBindBuffer(GL_PIXEL_UNPACK_BUFFER,",pixelUnpackBufferBinding,");",delim);
      tmp << print_string("glBindBuffer(GL_PIXEL_PACK_BUFFER,",pixelPackBufferBinding,");",delim);
      return tmp.str();
    }

    template <typename T> void glPixelStore( GLenum pname, T param )
    {
      switch (pname)
      {
        case GL_UNPACK_SWAP_BYTES:   unpackSwapBytes   = static_cast<GLboolean>(param != T(0)); break;
        case GL_UNPACK_LSB_FIRST:    unpackLsbFirst    = static_cast<GLboolean>(param != T(0)); break;
        case GL_UNPACK_IMAGE_HEIGHT: unpackImageHeight = static_cast<GLint>(param);             break;
        case GL_UNPACK_SKIP_IMAGES:  unpackSkipImages  = static_cast<GLint>(param);             break;
        case GL_UNPACK_ROW_LENGTH:   unpackRowLength   = static_cast<GLint>(param);             break;
        case GL_UNPACK_SKIP_ROWS:    unpackSkipRows    = static_cast<GLint>(param);             break;
        case GL_UNPACK_SKIP_PIXELS:  unpackSkipPixels  = static_cast<GLint>(param);             break;
        case GL_UNPACK_ALIGNMENT:    unpackAlignment   = static_cast<GLint>(param);             break;
        case GL_PACK_SWAP_BYTES:     packSwapBytes     = static_cast<GLboolean>(param != T(0)); break;
        case GL_PACK_LSB_FIRST:      packLsbFirst      = static_cast<GLboolean>(param != T(0)); break;
        case GL_PACK_IMAGE_HEIGHT:   packImageHeight   = static_cast<GLint>(param);             break;
        case GL_PACK_SKIP_IMAGES:    packSkipImages    = static_cast<GLint>(param);             break;
        case GL_PACK_ROW_LENGTH:     packRowLength     = static_cast<GLint>(param);             break;
        case GL_PACK_SKIP_ROWS:      packSkipRows      = static_cast<GLint>(param);             break;
        case GL_PACK_SKIP_PIXELS:    packSkipPixels    = static_cast<GLint>(param);             break;
        case GL_PACK_ALIGNMENT:      packAlignment     = static_cast<GLint>(param);             break;
        default:
          break;
      }
    }

    void glBindBuffer( GLenum target, GLuint buffer )
    {
      switch (target)
      {
        case GL_PIXEL_UNPACK_BUFFER:
          pixelUnpackBufferBinding = buffer;
          break;
        case GL_PIXEL_PACK_BUFFER:
          pixelPackBufferBinding = buffer;
          break;
        default:
          break;
      }
    }

    void glDeleteBuffers( GLsizei n, const GLuint *buffers )
    {
      for (GLsizei ii=0; ii<n; ii++)
      {
        if (buffers[ii] == pixelUnpackBufferBinding)
          pixelUnpackBufferBinding = 0;
        if (buffers[ii] == pixelPackBufferBinding)
          pixelPackBufferBinding = 0;
      }
    }
  };

}

REGAL_NAMESPACE_END

#endif

#endif
