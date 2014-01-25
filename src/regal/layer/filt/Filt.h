/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012-2013 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
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

 Regal API filtering support for ES 2.0 and Core GL contexts.
 Cass Everitt

 */

#ifndef __REGAL_FILT_H__
#define __REGAL_FILT_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include "RegalEmu.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "FiltProcs.h"
#include "RegalBreak.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

  struct Filt : public Layer
  {
    Filt( RegalContext * ctx ) : Layer( ctx ), fboID(0) {}

    virtual std::string GetName() { return "filt"; }
    virtual bool Initialize( const std::string & instanceInfo ) {
      ResetInterception();
      return true;
    }
    virtual void ResetInterception() {
      RegalContext * ctx = this->GetContext();
      orig.Initialize( ctx->dispatchGL );
      FiltIntercept( this, ctx->dispatchGL );
    }
    
    bool BindTexture(GLenum target, GLuint name );
    bool BindFramebuffer(GLenum target, GLuint framebuffer);
    bool DrawBuffers(GLsizei n, const GLenum *bufs);

    template <typename T> bool Get(GLenum pname, T *params)
    {
      int retVal;
      if (FilterGet(pname,retVal)) {
        params[0] = T(retVal);
        return true;
      }
      return false;
    }

    bool GetTexParameteriv   (GLenum target, GLenum pname, GLint *params);
    bool TexParameter        (GLenum target, GLenum pname);
    bool FilterTexParameter  (GLenum target, GLenum pname, GLfloat param, GLfloat &newParam);
    bool FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    bool GenerateMipmap(GLenum target);
    bool PolygonMode (GLenum face, GLenum mode);
    bool ReadBuffer  (GLenum src);
    bool RenderMode  (GLenum mode);
    bool PixelStorei (GLenum pname, GLint param);
    bool FramebufferAttachmentSupported(GLenum attachment);

    bool FilterGet   (GLenum pname, int &retVal);

    bool TexImage2D  (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);

    FiltOriginate orig;
    
  private:
    GLuint fboID;
  };

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // __REGAL_FILT_H__
