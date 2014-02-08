/*
  Copyright (c) 2011-2014 NVIDIA Corporation
  Copyright (c) 2011-2014 Cass Everitt
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

#ifndef __REGAL_DISPATCH_H__
#define __REGAL_DISPATCH_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct Layer {

  Layer( RegalContext * context ) : ctx( context ) {}
  virtual ~Layer() {}
  RegalContext * GetContext() { return ctx; }
  virtual std::string GetName() const = 0;
  virtual bool Initialize( const std::string & instanceInfo ) = 0;
  virtual void ResetInterception() = 0;
  private:
  RegalContext *ctx;

};

template <typename T>
struct RegalProc {
  RegalProc() : proc(NULL), layer(NULL) {}
  RegalProc( T p, Layer * l ) : proc(p), layer(l) {}
  T proc;
  Layer *layer;
};

template <typename T> RegalProc<T> MakeRegalProc( T p, Layer *l ) { return RegalProc<T>( p, l ); }

REGAL_NAMESPACE_END

#endif // __REGAL_DISPATCH_H__
