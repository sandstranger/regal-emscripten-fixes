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

 Disk-based .png texture caching
 Nigel Stewart

 */

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalConfig.h"
#include "RegalToken.h"
#include "RegalCacheTexture.h"
#include "RegalScopedPtr.h"

#include <lookup3.h>           // 32-bit memory hashing function

#include <string>
#include "RegalPrint.h"

using namespace ::std;


REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Cache {

static void
cacheTextureTarget(RegalContext * ctx, REGALGLGETTEXLEVELPARAMETERIVPROC getTexLevelProc, REGALGLGETTEXIMAGEPROC getTexImageProc, GLenum target)
{
  RegalAssert(getTexLevelProc);
  RegalAssert(getTexImageProc);

  if (REGAL_CACHE && REGAL_CACHE_TEXTURE && Config::cache && Config::cacheTexture)
  {
    // Query the texture dimensions
    // TODO: Query the internal format and channels too?

    GLint width = 0;
    GLint height = 0;

    getTexLevelProc(ctx, target, 0, GL_TEXTURE_WIDTH,  &width);
    getTexLevelProc(ctx, target, 0, GL_TEXTURE_HEIGHT, &height);

    const GLint size = width*height;
    const GLint bytes = size*4;

    Internal("Regal::CacheTexture::texture","width=",width," height=",height);

    if (!width || !height)
      return;

    // Fetch the RGBA texture data
    // TODO: Handle packing alignment...

    scoped_array<GLbyte> buffer(new GLbyte [bytes]);

    getTexImageProc(ctx, target,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer.get());

    // Compute 32-bit hash

    uint32_t hash = 0;
    hash = Lookup3::hashlittle(buffer.get(), bytes, hash);

    string filename = makePath(Config::cacheDirectory,print_string(print_hex(hash),".png"));

    Info("glBindTexture(",Token::GLenumToString(target),") width=",width," height=",height," hash=",print_hex(hash));

    // Cache it to disk, iff it's not there yet
    // export REGAL_CACHE_TEXTURE_WRITE=1

    if (REGAL_CACHE_TEXTURE_WRITE && Config::cacheTextureWrite && !fileExists(filename.c_str()))
    {
      // use stb png writer
    }
    else
    {
      // Read from disk cache, if reading is enabled
      // export REGAL_CACHE_TEXTURE_READ=1

      if (REGAL_CACHE_TEXTURE_READ && Config::cacheTextureRead)
      {
        // use stb image reader
        return;
      }
    }
  }
}

void
bindTexture(RegalContext * ctx, REGALGLBINDTEXTUREPROC bindTextureProc, REGALGLGETTEXLEVELPARAMETERIVPROC getTexLevelProc, REGALGLGETTEXIMAGEPROC getTexImageProc, GLenum target, GLuint texture)
{
  RegalAssert(bindTextureProc);
  RegalAssert(getTexLevelProc);
  RegalAssert(getTexImageProc);

  bindTextureProc(ctx,target,texture);

  Internal("Cache::BindTexture(",Token::GLenumToString(target),",",texture,")");

  switch (target)
  {
    case GL_TEXTURE_CUBE_MAP:
      cacheTextureTarget(ctx,getTexLevelProc,getTexImageProc,GL_TEXTURE_CUBE_MAP_POSITIVE_X);
      cacheTextureTarget(ctx,getTexLevelProc,getTexImageProc,GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
      cacheTextureTarget(ctx,getTexLevelProc,getTexImageProc,GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
      cacheTextureTarget(ctx,getTexLevelProc,getTexImageProc,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
      cacheTextureTarget(ctx,getTexLevelProc,getTexImageProc,GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
      cacheTextureTarget(ctx,getTexLevelProc,getTexImageProc,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
      break;

    // TODO - arrays, etc

    default:
      cacheTextureTarget(ctx,getTexLevelProc,getTexImageProc,target);
      break;
  }
}

};

REGAL_NAMESPACE_END
