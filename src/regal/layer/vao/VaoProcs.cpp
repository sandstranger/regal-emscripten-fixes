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

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include "RegalPrivate.h"
#include "RegalContext.h"
#include "RegalDispatch.h"
#include "RegalVao.h"
#include "RegalEmuProcsVao.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

static void REGAL_CALL Vao_glBindBuffer(Layer *_layer, GLenum target, GLuint buffer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  _context->vao->ShadowBufferBinding( target, buffer );

  orig.glBindBuffer( orig.glBindBuffer_layer, target, buffer );

}

static void REGAL_CALL Vao_glBindBufferARB(Layer *_layer, GLenum target, GLuint buffer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  _context->vao->ShadowBufferBinding( target, buffer );

  orig.glBindBufferARB( orig.glBindBufferARB_layer, target, buffer );

}

static void REGAL_CALL Vao_glBindVertexArray(Layer *_layer, GLuint array)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->BindVertexArray(*_context, array ); return;

  orig.glBindVertexArray( orig.glBindVertexArray_layer, array );

}

static void REGAL_CALL Vao_glClientActiveTexture(Layer *_layer, GLenum texture)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  _context->vao->ClientActiveTexture(*_context, texture );

  orig.glClientActiveTexture( orig.glClientActiveTexture_layer, texture );

}

static void REGAL_CALL Vao_glClientActiveTextureARB(Layer *_layer, GLenum texture)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  _context->vao->ClientActiveTexture(*_context, texture );

  orig.glClientActiveTextureARB( orig.glClientActiveTextureARB_layer, texture );

}

static void REGAL_CALL Vao_glColorPointer(Layer *_layer, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->ColorPointer(*_context, size, type, stride, pointer ); return;

  orig.glColorPointer( orig.glColorPointer_layer, size, type, stride, pointer );

}

static void REGAL_CALL Vao_glColorPointerEXT(Layer *_layer, GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->ColorPointer(*_context, size, type, stride, pointer ); return;

  orig.glColorPointerEXT( orig.glColorPointerEXT_layer, size, type, stride, count, pointer );

}

static void REGAL_CALL Vao_glDeleteVertexArrays(Layer *_layer, GLsizei n, const GLuint *arrays)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->DeleteVertexArrays( n, arrays ); return;

  orig.glDeleteVertexArrays( orig.glDeleteVertexArrays_layer, n, arrays );

}

static void REGAL_CALL Vao_glDisableClientState(Layer *_layer, GLenum cap)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->DisableClientState(*_context, cap ); return;

  orig.glDisableClientState( orig.glDisableClientState_layer, cap );

}

static void REGAL_CALL Vao_glDisableVertexAttribArray(Layer *_layer, GLuint index)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  return _context->vao->DisableVertexAttribArray(*_context, index );

  orig.glDisableVertexAttribArray( orig.glDisableVertexAttribArray_layer, index );

}

static void REGAL_CALL Vao_glDisableVertexAttribArrayARB(Layer *_layer, GLuint index)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  return _context->vao->DisableVertexAttribArray(*_context, index );

  orig.glDisableVertexAttribArrayARB( orig.glDisableVertexAttribArrayARB_layer, index );

}

static void REGAL_CALL Vao_glDrawArrays(Layer *_layer, GLenum mode, GLint first, GLsizei count)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawArrays( orig.glDrawArrays_layer, mode, first, count );

}

static void REGAL_CALL Vao_glDrawArraysEXT(Layer *_layer, GLenum mode, GLint first, GLsizei count)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawArraysEXT( orig.glDrawArraysEXT_layer, mode, first, count );

}

static void REGAL_CALL Vao_glDrawArraysIndirect(Layer *_layer, GLenum mode, const GLvoid *indirect)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawArraysIndirect( orig.glDrawArraysIndirect_layer, mode, indirect );

}

static void REGAL_CALL Vao_glDrawArraysInstanced(Layer *_layer, GLenum mode, GLint start, GLsizei count, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawArraysInstanced( orig.glDrawArraysInstanced_layer, mode, start, count, primcount );

}

static void REGAL_CALL Vao_glDrawArraysInstancedARB(Layer *_layer, GLenum mode, GLint start, GLsizei count, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawArraysInstancedARB( orig.glDrawArraysInstancedARB_layer, mode, start, count, primcount );

}

static void REGAL_CALL Vao_glDrawArraysInstancedEXT(Layer *_layer, GLenum mode, GLint start, GLsizei count, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawArraysInstancedEXT( orig.glDrawArraysInstancedEXT_layer, mode, start, count, primcount );

}

static void REGAL_CALL Vao_glDrawElementArrayAPPLE(Layer *_layer, GLenum mode, GLint first, GLsizei count)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElementArrayAPPLE( orig.glDrawElementArrayAPPLE_layer, mode, first, count );

}

static void REGAL_CALL Vao_glDrawElementArrayATI(Layer *_layer, GLenum mode, GLsizei count)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElementArrayATI( orig.glDrawElementArrayATI_layer, mode, count );

}

static void REGAL_CALL Vao_glDrawElements(Layer *_layer, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElements( orig.glDrawElements_layer, mode, count, type, indices );

}

static void REGAL_CALL Vao_glDrawElementsBaseVertex(Layer *_layer, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElementsBaseVertex( orig.glDrawElementsBaseVertex_layer, mode, count, type, indices, basevertex );

}

static void REGAL_CALL Vao_glDrawElementsIndirect(Layer *_layer, GLenum mode, GLenum type, const GLvoid *indirect)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElementsIndirect( orig.glDrawElementsIndirect_layer, mode, type, indirect );

}

static void REGAL_CALL Vao_glDrawElementsInstanced(Layer *_layer, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElementsInstanced( orig.glDrawElementsInstanced_layer, mode, count, type, indices, primcount );

}

static void REGAL_CALL Vao_glDrawElementsInstancedARB(Layer *_layer, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElementsInstancedARB( orig.glDrawElementsInstancedARB_layer, mode, count, type, indices, primcount );

}

static void REGAL_CALL Vao_glDrawElementsInstancedBaseVertex(Layer *_layer, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElementsInstancedBaseVertex( orig.glDrawElementsInstancedBaseVertex_layer, mode, count, type, indices, primcount, basevertex );

}

static void REGAL_CALL Vao_glDrawElementsInstancedEXT(Layer *_layer, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glDrawElementsInstancedEXT( orig.glDrawElementsInstancedEXT_layer, mode, count, type, indices, primcount );

}

static void REGAL_CALL Vao_glEnableClientState(Layer *_layer, GLenum cap)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->EnableClientState(*_context, cap ); return;

  orig.glEnableClientState( orig.glEnableClientState_layer, cap );

}

static void REGAL_CALL Vao_glEnableVertexAttribArray(Layer *_layer, GLuint index)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  return _context->vao->EnableVertexAttribArray(*_context, index );

  orig.glEnableVertexAttribArray( orig.glEnableVertexAttribArray_layer, index );

}

static void REGAL_CALL Vao_glEnableVertexAttribArrayARB(Layer *_layer, GLuint index)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  return _context->vao->EnableVertexAttribArray(*_context, index );

  orig.glEnableVertexAttribArrayARB( orig.glEnableVertexAttribArrayARB_layer, index );

}

static void REGAL_CALL Vao_glFogCoordPointer(Layer *_layer, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->FogCoordPointer(*_context, type, stride, pointer ); return;

  orig.glFogCoordPointer( orig.glFogCoordPointer_layer, type, stride, pointer );

}

static void REGAL_CALL Vao_glFogCoordPointerEXT(Layer *_layer, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->FogCoordPointer(*_context, type, stride, pointer ); return;

  orig.glFogCoordPointerEXT( orig.glFogCoordPointerEXT_layer, type, stride, pointer );

}

static void REGAL_CALL Vao_glGenVertexArrays(Layer *_layer, GLsizei n, GLuint *arrays)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GenVertexArrays( n, arrays ); return;

  orig.glGenVertexArrays( orig.glGenVertexArrays_layer, n, arrays );

}

static void REGAL_CALL Vao_glGetBooleanv(Layer *_layer, GLenum pname, GLboolean *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  if( _context->vao->Get( pname, params ) ) {
    return;
  }

  orig.glGetBooleanv( orig.glGetBooleanv_layer, pname, params );

}

static void REGAL_CALL Vao_glGetDoublev(Layer *_layer, GLenum pname, GLdouble *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  if( _context->vao->Get( pname, params ) ) {
    return;
  }

  orig.glGetDoublev( orig.glGetDoublev_layer, pname, params );

}

static void REGAL_CALL Vao_glGetFloatv(Layer *_layer, GLenum pname, GLfloat *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  if( _context->vao->Get( pname, params ) ) {
    return;
  }

  orig.glGetFloatv( orig.glGetFloatv_layer, pname, params );

}

static void REGAL_CALL Vao_glGetInteger64v(Layer *_layer, GLenum pname, GLint64 *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  if( _context->vao->Get( pname, params ) ) {
    return;
  }

  orig.glGetInteger64v( orig.glGetInteger64v_layer, pname, params );

}

static void REGAL_CALL Vao_glGetIntegerv(Layer *_layer, GLenum pname, GLint *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  if( _context->vao->Get( pname, params ) ) {
    return;
  }

  orig.glGetIntegerv( orig.glGetIntegerv_layer, pname, params );

}

static void REGAL_CALL Vao_glGetVertexAttribPointerv(Layer *_layer, GLuint index, GLenum pname, GLvoid **pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GetAttrib( index, pname, pointer ); return;

  orig.glGetVertexAttribPointerv( orig.glGetVertexAttribPointerv_layer, index, pname, pointer );

}

static void REGAL_CALL Vao_glGetVertexAttribPointervARB(Layer *_layer, GLuint index, GLenum pname, GLvoid **pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GetAttrib( index, pname, pointer ); return;

  orig.glGetVertexAttribPointervARB( orig.glGetVertexAttribPointervARB_layer, index, pname, pointer );

}

static void REGAL_CALL Vao_glGetVertexAttribdv(Layer *_layer, GLuint index, GLenum pname, GLdouble *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GetAttrib( index, pname, params ); return;

  orig.glGetVertexAttribdv( orig.glGetVertexAttribdv_layer, index, pname, params );

}

static void REGAL_CALL Vao_glGetVertexAttribdvARB(Layer *_layer, GLuint index, GLenum pname, GLdouble *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GetAttrib( index, pname, params ); return;

  orig.glGetVertexAttribdvARB( orig.glGetVertexAttribdvARB_layer, index, pname, params );

}

static void REGAL_CALL Vao_glGetVertexAttribfv(Layer *_layer, GLuint index, GLenum pname, GLfloat *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GetAttrib( index, pname, params ); return;

  orig.glGetVertexAttribfv( orig.glGetVertexAttribfv_layer, index, pname, params );

}

static void REGAL_CALL Vao_glGetVertexAttribfvARB(Layer *_layer, GLuint index, GLenum pname, GLfloat *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GetAttrib( index, pname, params ); return;

  orig.glGetVertexAttribfvARB( orig.glGetVertexAttribfvARB_layer, index, pname, params );

}

static void REGAL_CALL Vao_glGetVertexAttribiv(Layer *_layer, GLuint index, GLenum pname, GLint *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GetAttrib( index, pname, params ); return;

  orig.glGetVertexAttribiv( orig.glGetVertexAttribiv_layer, index, pname, params );

}

static void REGAL_CALL Vao_glGetVertexAttribivARB(Layer *_layer, GLuint index, GLenum pname, GLint *params)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->GetAttrib( index, pname, params ); return;

  orig.glGetVertexAttribivARB( orig.glGetVertexAttribivARB_layer, index, pname, params );

}

static void REGAL_CALL Vao_glInterleavedArrays(Layer *_layer, GLenum format, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->InterleavedArrays(*_context, format, stride, pointer ); return;

  orig.glInterleavedArrays( orig.glInterleavedArrays_layer, format, stride, pointer );

}

static GLboolean REGAL_CALL Vao_glIsVertexArray(Layer *_layer, GLuint array)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  return _context->vao->IsVertexArray( array );

  return orig.glIsVertexArray( orig.glIsVertexArray_layer, array );

}

static void REGAL_CALL Vao_glMultiDrawArrays(Layer *_layer, GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawArrays( orig.glMultiDrawArrays_layer, mode, first, count, primcount );

}

static void REGAL_CALL Vao_glMultiDrawArraysEXT(Layer *_layer, GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawArraysEXT( orig.glMultiDrawArraysEXT_layer, mode, first, count, primcount );

}

static void REGAL_CALL Vao_glMultiDrawArraysIndirect(Layer *_layer, GLenum mode, const GLvoid *indirect, GLsizei primcount, GLsizei stride)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawArraysIndirect( orig.glMultiDrawArraysIndirect_layer, mode, indirect, primcount, stride );

}

static void REGAL_CALL Vao_glMultiDrawArraysIndirectAMD(Layer *_layer, GLenum mode, const GLvoid *indirect, GLsizei primcount, GLsizei stride)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawArraysIndirectAMD( orig.glMultiDrawArraysIndirectAMD_layer, mode, indirect, primcount, stride );

}

static void REGAL_CALL Vao_glMultiDrawElementArrayAPPLE(Layer *_layer, GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawElementArrayAPPLE( orig.glMultiDrawElementArrayAPPLE_layer, mode, first, count, primcount );

}

static void REGAL_CALL Vao_glMultiDrawElements(Layer *_layer, GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawElements( orig.glMultiDrawElements_layer, mode, count, type, indices, primcount );

}

static void REGAL_CALL Vao_glMultiDrawElementsBaseVertex(Layer *_layer, GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, const GLint *basevertex)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawElementsBaseVertex( orig.glMultiDrawElementsBaseVertex_layer, mode, count, type, indices, primcount, basevertex );

}

static void REGAL_CALL Vao_glMultiDrawElementsEXT(Layer *_layer, GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawElementsEXT( orig.glMultiDrawElementsEXT_layer, mode, count, type, indices, primcount );

}

static void REGAL_CALL Vao_glMultiDrawElementsIndirect(Layer *_layer, GLenum mode, GLenum type, const GLvoid *indirect, GLsizei primcount, GLsizei stride)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawElementsIndirect( orig.glMultiDrawElementsIndirect_layer, mode, type, indirect, primcount, stride );

}

static void REGAL_CALL Vao_glMultiDrawElementsIndirectAMD(Layer *_layer, GLenum mode, GLenum type, const GLvoid *indirect, GLsizei primcount, GLsizei stride)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // prefix
  // _context->vao->Validate(*_context );

  orig.glMultiDrawElementsIndirectAMD( orig.glMultiDrawElementsIndirectAMD_layer, mode, type, indirect, primcount, stride );

}

static void REGAL_CALL Vao_glNormalPointer(Layer *_layer, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->NormalPointer(*_context, type, stride, pointer ); return;

  orig.glNormalPointer( orig.glNormalPointer_layer, type, stride, pointer );

}

static void REGAL_CALL Vao_glNormalPointerEXT(Layer *_layer, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->NormalPointer(*_context, type, stride, pointer ); return;

  orig.glNormalPointerEXT( orig.glNormalPointerEXT_layer, type, stride, count, pointer );

}

static void REGAL_CALL Vao_glSecondaryColorPointer(Layer *_layer, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->SecondaryColorPointer(*_context, size, type, stride, pointer ); return;

  orig.glSecondaryColorPointer( orig.glSecondaryColorPointer_layer, size, type, stride, pointer );

}

static void REGAL_CALL Vao_glSecondaryColorPointerEXT(Layer *_layer, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->SecondaryColorPointer(*_context, size, type, stride, pointer ); return;

  orig.glSecondaryColorPointerEXT( orig.glSecondaryColorPointerEXT_layer, size, type, stride, pointer );

}

static void REGAL_CALL Vao_glTexCoordPointer(Layer *_layer, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->TexCoordPointer(*_context, size, type, stride, pointer ); return;

  orig.glTexCoordPointer( orig.glTexCoordPointer_layer, size, type, stride, pointer );

}

static void REGAL_CALL Vao_glTexCoordPointerEXT(Layer *_layer, GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->TexCoordPointer(*_context, size, type, stride, pointer ); return;

  orig.glTexCoordPointerEXT( orig.glTexCoordPointerEXT_layer, size, type, stride, count, pointer );

}

static void REGAL_CALL Vao_glVertexAttribPointer(Layer *_layer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  return _context->vao->AttribPointer(*_context, index, size, type, normalized, stride, pointer );

  orig.glVertexAttribPointer( orig.glVertexAttribPointer_layer, index, size, type, normalized, stride, pointer );

}

static void REGAL_CALL Vao_glVertexAttribPointerARB(Layer *_layer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  return _context->vao->AttribPointer(*_context, index, size, type, normalized, stride, pointer );

  orig.glVertexAttribPointerARB( orig.glVertexAttribPointerARB_layer, index, size, type, normalized, stride, pointer );

}

static void REGAL_CALL Vao_glVertexPointer(Layer *_layer, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->VertexPointer(*_context, size, type, stride, pointer ); return;

  orig.glVertexPointer( orig.glVertexPointer_layer, size, type, stride, pointer );

}

static void REGAL_CALL Vao_glVertexPointerEXT(Layer *_layer, GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
  Vao * self = static_cast<Vao *>(_layer);

  VaoOriginate & orig = self->orig;

  // impl
  _context->vao->VertexPointer(*_context, size, type, stride, pointer ); return;

  orig.glVertexPointerEXT( orig.glVertexPointerEXT_layer, size, type, stride, count, pointer );

}

void VaoIntercept( Dispatch::GL & dt ) {
  dt.glBindBuffer                      = RVao_glBindBuffer;
  dt.glBindBufferARB                   = RVao_glBindBufferARB;
  dt.glBindVertexArray                 = RVao_glBindVertexArray;
  dt.glClientActiveTexture             = RVao_glClientActiveTexture;
  dt.glClientActiveTextureARB          = RVao_glClientActiveTextureARB;
  dt.glColorPointer                    = RVao_glColorPointer;
  dt.glColorPointerEXT                 = RVao_glColorPointerEXT;
  dt.glDeleteVertexArrays              = RVao_glDeleteVertexArrays;
  dt.glDisableClientState              = RVao_glDisableClientState;
  dt.glDisableVertexAttribArray        = RVao_glDisableVertexAttribArray;
  dt.glDisableVertexAttribArrayARB     = RVao_glDisableVertexAttribArrayARB;
  dt.glDrawArrays                      = RVao_glDrawArrays;
  dt.glDrawArraysEXT                   = RVao_glDrawArraysEXT;
  dt.glDrawArraysIndirect              = RVao_glDrawArraysIndirect;
  dt.glDrawArraysInstanced             = RVao_glDrawArraysInstanced;
  dt.glDrawArraysInstancedARB          = RVao_glDrawArraysInstancedARB;
  dt.glDrawArraysInstancedEXT          = RVao_glDrawArraysInstancedEXT;
  dt.glDrawElementArrayAPPLE           = RVao_glDrawElementArrayAPPLE;
  dt.glDrawElementArrayATI             = RVao_glDrawElementArrayATI;
  dt.glDrawElements                    = RVao_glDrawElements;
  dt.glDrawElementsBaseVertex          = RVao_glDrawElementsBaseVertex;
  dt.glDrawElementsIndirect            = RVao_glDrawElementsIndirect;
  dt.glDrawElementsInstanced           = RVao_glDrawElementsInstanced;
  dt.glDrawElementsInstancedARB        = RVao_glDrawElementsInstancedARB;
  dt.glDrawElementsInstancedBaseVertex = RVao_glDrawElementsInstancedBaseVertex;
  dt.glDrawElementsInstancedEXT        = RVao_glDrawElementsInstancedEXT;
  dt.glEnableClientState               = RVao_glEnableClientState;
  dt.glEnableVertexAttribArray         = RVao_glEnableVertexAttribArray;
  dt.glEnableVertexAttribArrayARB      = RVao_glEnableVertexAttribArrayARB;
  dt.glFogCoordPointer                 = RVao_glFogCoordPointer;
  dt.glFogCoordPointerEXT              = RVao_glFogCoordPointerEXT;
  dt.glGenVertexArrays                 = RVao_glGenVertexArrays;
  dt.glGetBooleanv                     = RVao_glGetBooleanv;
  dt.glGetDoublev                      = RVao_glGetDoublev;
  dt.glGetFloatv                       = RVao_glGetFloatv;
  dt.glGetInteger64v                   = RVao_glGetInteger64v;
  dt.glGetIntegerv                     = RVao_glGetIntegerv;
  dt.glGetVertexAttribPointerv         = RVao_glGetVertexAttribPointerv;
  dt.glGetVertexAttribPointervARB      = RVao_glGetVertexAttribPointervARB;
  dt.glGetVertexAttribdv               = RVao_glGetVertexAttribdv;
  dt.glGetVertexAttribdvARB            = RVao_glGetVertexAttribdvARB;
  dt.glGetVertexAttribfv               = RVao_glGetVertexAttribfv;
  dt.glGetVertexAttribfvARB            = RVao_glGetVertexAttribfvARB;
  dt.glGetVertexAttribiv               = RVao_glGetVertexAttribiv;
  dt.glGetVertexAttribivARB            = RVao_glGetVertexAttribivARB;
  dt.glInterleavedArrays               = RVao_glInterleavedArrays;
  dt.glIsVertexArray                   = RVao_glIsVertexArray;
  dt.glMultiDrawArrays                 = RVao_glMultiDrawArrays;
  dt.glMultiDrawArraysEXT              = RVao_glMultiDrawArraysEXT;
  dt.glMultiDrawArraysIndirect         = RVao_glMultiDrawArraysIndirect;
  dt.glMultiDrawArraysIndirectAMD      = RVao_glMultiDrawArraysIndirectAMD;
  dt.glMultiDrawElementArrayAPPLE      = RVao_glMultiDrawElementArrayAPPLE;
  dt.glMultiDrawElements               = RVao_glMultiDrawElements;
  dt.glMultiDrawElementsBaseVertex     = RVao_glMultiDrawElementsBaseVertex;
  dt.glMultiDrawElementsEXT            = RVao_glMultiDrawElementsEXT;
  dt.glMultiDrawElementsIndirect       = RVao_glMultiDrawElementsIndirect;
  dt.glMultiDrawElementsIndirectAMD    = RVao_glMultiDrawElementsIndirectAMD;
  dt.glNormalPointer                   = RVao_glNormalPointer;
  dt.glNormalPointerEXT                = RVao_glNormalPointerEXT;
  dt.glSecondaryColorPointer           = RVao_glSecondaryColorPointer;
  dt.glSecondaryColorPointerEXT        = RVao_glSecondaryColorPointerEXT;
  dt.glTexCoordPointer                 = RVao_glTexCoordPointer;
  dt.glTexCoordPointerEXT              = RVao_glTexCoordPointerEXT;
  dt.glVertexAttribPointer             = RVao_glVertexAttribPointer;
  dt.glVertexAttribPointerARB          = RVao_glVertexAttribPointerARB;
  dt.glVertexPointer                   = RVao_glVertexPointer;
  dt.glVertexPointerEXT                = RVao_glVertexPointerEXT;
}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
