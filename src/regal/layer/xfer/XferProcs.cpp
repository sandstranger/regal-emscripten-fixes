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
#include "RegalXfer.h"
#include "RegalEmuProcsXfer.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

static void REGAL_CALL Xfer_glActiveTexture(Layer *_layer, GLenum texture)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // prefix
  _context->xfer->ShadowActiveTexture( texture );

  orig.glActiveTexture( orig.glActiveTexture_layer, texture );

}

static void REGAL_CALL Xfer_glActiveTextureARB(Layer *_layer, GLenum texture)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // prefix
  _context->xfer->ShadowActiveTexture( texture );

  orig.glActiveTextureARB( orig.glActiveTextureARB_layer, texture );

}

static void REGAL_CALL Xfer_glCompressedTexImage2D(Layer *_layer, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // impl
  _context->xfer->CompressedTexImage2D( _context, target, level, internalformat, width, height, border, imageSize, data ); return;

  orig.glCompressedTexImage2D( orig.glCompressedTexImage2D_layer, target, level, internalformat, width, height, border, imageSize, data );

}

static void REGAL_CALL Xfer_glCompressedTexImage2DARB(Layer *_layer, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // impl
  _context->xfer->CompressedTexImage2D( _context, target, level, internalformat, width, height, border, imageSize, data ); return;

  orig.glCompressedTexImage2DARB( orig.glCompressedTexImage2DARB_layer, target, level, internalformat, width, height, border, imageSize, data );

}

static void REGAL_CALL Xfer_glCompressedTexSubImage2D(Layer *_layer, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // impl
  _context->xfer->CompressedTexSubImage2D( _context, target, level, xoffset, yoffset, width, height, format, imageSize, data ); return;

  orig.glCompressedTexSubImage2D( orig.glCompressedTexSubImage2D_layer, target, level, xoffset, yoffset, width, height, format, imageSize, data );

}

static void REGAL_CALL Xfer_glCompressedTexSubImage2DARB(Layer *_layer, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // impl
  _context->xfer->CompressedTexSubImage2D( _context, target, level, xoffset, yoffset, width, height, format, imageSize, data ); return;

  orig.glCompressedTexSubImage2DARB( orig.glCompressedTexSubImage2DARB_layer, target, level, xoffset, yoffset, width, height, format, imageSize, data );

}

static void REGAL_CALL Xfer_glPixelStoref(Layer *_layer, GLenum pname, GLfloat param)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // prefix
  _context->xfer->PixelStore( _context, pname, param );

  orig.glPixelStoref( orig.glPixelStoref_layer, pname, param );

}

static void REGAL_CALL Xfer_glPixelStorei(Layer *_layer, GLenum pname, GLint param)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // prefix
  _context->xfer->PixelStore( _context, pname, param );

  orig.glPixelStorei( orig.glPixelStorei_layer, pname, param );

}

static void REGAL_CALL Xfer_glTexImage2D(Layer *_layer, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // impl
  _context->xfer->TexImage2D( _context, target, level, internalformat, width, height, border, format, type, pixels ); return;

  orig.glTexImage2D( orig.glTexImage2D_layer, target, level, internalformat, width, height, border, format, type, pixels );

}

static void REGAL_CALL Xfer_glTexSubImage2D(Layer *_layer, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // impl
  _context->xfer->TexSubImage2D( _context, target, level, xoffset, yoffset, width, height, format, type, pixels ); return;

  orig.glTexSubImage2D( orig.glTexSubImage2D_layer, target, level, xoffset, yoffset, width, height, format, type, pixels );

}

static void REGAL_CALL Xfer_glTexSubImage2DEXT(Layer *_layer, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
  Xfer * self = static_cast<Xfer *>(_layer);

  XferOriginate & orig = self->orig;

  // impl
  _context->xfer->TexSubImage2D( _context, target, level, xoffset, yoffset, width, height, format, type, pixels ); return;

  orig.glTexSubImage2DEXT( orig.glTexSubImage2DEXT_layer, target, level, xoffset, yoffset, width, height, format, type, pixels );

}

void XferIntercept( Dispatch::GL & dt ) {
  dt.glActiveTexture              = RXfer_glActiveTexture;
  dt.glActiveTextureARB           = RXfer_glActiveTextureARB;
  dt.glCompressedTexImage2D       = RXfer_glCompressedTexImage2D;
  dt.glCompressedTexImage2DARB    = RXfer_glCompressedTexImage2DARB;
  dt.glCompressedTexSubImage2D    = RXfer_glCompressedTexSubImage2D;
  dt.glCompressedTexSubImage2DARB = RXfer_glCompressedTexSubImage2DARB;
  dt.glPixelStoref                = RXfer_glPixelStoref;
  dt.glPixelStorei                = RXfer_glPixelStorei;
  dt.glTexImage2D                 = RXfer_glTexImage2D;
  dt.glTexSubImage2D              = RXfer_glTexSubImage2D;
  dt.glTexSubImage2DEXT           = RXfer_glTexSubImage2DEXT;
}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
