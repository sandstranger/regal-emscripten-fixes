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
 
 Regal glRect emu layer
 Scott Nations, Nigel Stewart
 
 */

#ifndef __REGAL_RECT_H__
#define __REGAL_RECT_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <vector>

#include <GL/Regal.h>

#include "RegalEmu.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RectProcs.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {
  
  template <typename T> inline void glVertex2(RectOriginate & orig, T x1, T y1)
  {
    RglVertex2f( orig, static_cast<GLfloat>(x1), static_cast<GLfloat>(y1));
  }
  
  template <> inline void glVertex2(RectOriginate & orig, GLfloat x1, GLfloat y1)
  {
    RglVertex2f( orig, x1, y1);
  }
  
  template <> inline void glVertex2(RectOriginate & orig, GLdouble x1, GLdouble y1)
  {
    RglVertex2d( orig, x1, y1);
  }
  
  template <> inline void glVertex2(RectOriginate & orig, GLint x1, GLint y1)
  {
    RglVertex2i( orig, x1, y1);
  }
  
  template <> inline void glVertex2(RectOriginate & orig, GLshort x1, GLshort y1)
  {
    RglVertex2s( orig, x1, y1);
  }
  
  struct Rect : public Layer
  {
    Rect( RegalContext * ctx ) : Layer( ctx ) {}
    
    virtual std::string GetName() const { return "rect"; }
    
    virtual bool Initialize( const std::string & instanceInfo ) {
      ResetInterception();
      return true;
    }
    
    virtual void ResetInterception() {
      RegalContext * ctx = GetContext();
      orig.Initialize( ctx->dispatchGL );
      RectIntercept( this, ctx->dispatchGL );
    }
    
    template <typename T> void glRect(T x1, T y1, T x2, T y2)
    {
      RegalContext * ctx = GetContext();
      // it's an error to call glRect between glBegin/glEnd, so just return when that happens.
      
      if (ctx->depthBeginEnd)
        return;
      
      // incrementing context->depthBeginEnd here avoids an assert in log_glBegin and
      // also keeps the log indentation as it should be
      
      ctx->depthBeginEnd++;
      
      RglBegin( orig, GL_POLYGON);
      
      Emu::glVertex2(orig, x1, y1);
      Emu::glVertex2(orig, x2, y1);
      Emu::glVertex2(orig, x2, y2);
      Emu::glVertex2(orig, x1, y2);
      
      // undo the above "artificial" increment
      
      ctx->depthBeginEnd--;
      
      RglEnd( orig);
    }
    
    template <typename T> inline void glRectv(const T *v1, const T *v2)
    {
      return glRect(v1[0], v1[1], v2[0], v2[1]);
    }
    
    RectOriginate orig;
  };
  
}

REGAL_NAMESPACE_END

#endif // ! __REGAL_RECT_H__
