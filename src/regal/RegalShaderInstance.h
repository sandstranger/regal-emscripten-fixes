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

// Create an instance of an app shader, which still has the same uniforms as
// the original program but includes support for things like fixed-function
// alpha test.  The idea is that Regal will swap between these instances
// based on API state, and the app won't need to know or care.

#ifndef __REGAL_SHADER_INSTANCE_H__
#define __REGAL_SHADER_INSTANCE_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <string>
#include <vector>
#include <map>

#include "RegalEmu.h"
#include "RegalPrivate.h"
#include "RegalContext.h"


REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace ShaderInstance {
  
  struct ShaderSource {
    GLenum type;
    std::string src;
  };

  struct Uniform {
    std::string name;
    GLint loc;
    GLint count;
    GLenum type;
    int offset; // byte offset into a uniform store
    GLuint64 ver;
  };
  
  struct Program {
    Program() : prog(0), ver(0) {}
    void AddUniform( const std::string & name, GLint loc, GLint count, GLenum type );
    void UpdateUniformStore( GLint location, GLint count, const void * ptr );
    
    GLint prog;
    GLuint64 ver;
    typedef std::map<GLint,Uniform,std::greater<GLint> > Map;
    Map uniforms;
    std::vector<GLuint> uniformStore;
  };
  
  struct UniformInstance {
    UniformInstance() : location(-1), instanceLocation(-1), ver(~ GLuint64(0)) {}
    GLint location;
    GLint instanceLocation;
    GLuint64 ver;
  };
  
  struct Procs {
    RegalContext * ctx;
    REGALGLUNIFORM1DVPROC          glUniform1dv;
    REGALGLUNIFORM2DVPROC          glUniform2dv;
    REGALGLUNIFORM3DVPROC          glUniform3dv;
    REGALGLUNIFORM4DVPROC          glUniform4dv;
    REGALGLUNIFORM1FVPROC          glUniform1fv;
    REGALGLUNIFORM2FVPROC          glUniform2fv;
    REGALGLUNIFORM3FVPROC          glUniform3fv;
    REGALGLUNIFORM4FVPROC          glUniform4fv;
    REGALGLUNIFORM1IVPROC          glUniform1iv;
    REGALGLUNIFORM2IVPROC          glUniform2iv;
    REGALGLUNIFORM3IVPROC          glUniform3iv;
    REGALGLUNIFORM4IVPROC          glUniform4iv;
    REGALGLUNIFORM1UIVPROC         glUniform1uiv;
    REGALGLUNIFORM2UIVPROC         glUniform2uiv;
    REGALGLUNIFORM3UIVPROC         glUniform3uiv;
    REGALGLUNIFORM4UIVPROC         glUniform4uiv;
    REGALGLUNIFORMMATRIX2DVPROC    glUniformMatrix2dv;
    REGALGLUNIFORMMATRIX3DVPROC    glUniformMatrix3dv;
    REGALGLUNIFORMMATRIX4DVPROC    glUniformMatrix4dv;
    REGALGLUNIFORMMATRIX2X3DVPROC  glUniformMatrix2x3dv;
    REGALGLUNIFORMMATRIX2X4DVPROC  glUniformMatrix2x4dv;
    REGALGLUNIFORMMATRIX3X2DVPROC  glUniformMatrix3x2dv;
    REGALGLUNIFORMMATRIX3X4DVPROC  glUniformMatrix3x4dv;
    REGALGLUNIFORMMATRIX4X2DVPROC  glUniformMatrix4x2dv;
    REGALGLUNIFORMMATRIX4X3DVPROC  glUniformMatrix4x3dv;
    REGALGLUNIFORMMATRIX2FVPROC    glUniformMatrix2fv;
    REGALGLUNIFORMMATRIX3FVPROC    glUniformMatrix3fv;
    REGALGLUNIFORMMATRIX4FVPROC    glUniformMatrix4fv;
    REGALGLUNIFORMMATRIX2X3FVPROC  glUniformMatrix2x3fv;
    REGALGLUNIFORMMATRIX2X4FVPROC  glUniformMatrix2x4fv;
    REGALGLUNIFORMMATRIX3X2FVPROC  glUniformMatrix3x2fv;
    REGALGLUNIFORMMATRIX3X4FVPROC  glUniformMatrix3x4fv;
    REGALGLUNIFORMMATRIX4X2FVPROC  glUniformMatrix4x2fv;
    REGALGLUNIFORMMATRIX4X3FVPROC  glUniformMatrix4x3fv;
    REGALGLGETUNIFORMDVPROC        glGetUniformdv;
    REGALGLGETUNIFORMFVPROC        glGetUniformfv;
    REGALGLGETUNIFORMIVPROC        glGetUniformiv;
    REGALGLGETUNIFORMUIVPROC       glGetUniformuiv;
    REGALGLGETUNIFORMLOCATIONPROC  glGetUniformLocation;
    REGALGLGETSHADERIVPROC         glGetShaderiv;
    REGALGLGETSHADERSOURCEPROC     glGetShaderSource;
    REGALGLGETATTACHEDSHADERSPROC  glGetAttachedShaders;
    REGALGLCREATESHADERPROC        glCreateShader;
    REGALGLSHADERSOURCEPROC        glShaderSource;
    REGALGLCOMPILESHADERPROC       glCompileShader;
    REGALGLGETSHADERINFOLOGPROC    glGetShaderInfoLog;
    REGALGLGETPROGRAMIVPROC        glGetProgramiv;
    REGALGLGETERRORPROC            glGetError;
    REGALGLGETACTIVEUNIFORMPROC    glGetActiveUniform;
    REGALGLLINKPROGRAMPROC         glLinkProgram;
    REGALGLGETACTIVEATTRIBPROC     glGetActiveAttrib;
    REGALGLGETATTRIBLOCATIONPROC   glGetAttribLocation;
    REGALGLBINDATTRIBLOCATIONPROC  glBindAttribLocation;
    REGALGLGETPROGRAMINFOLOGPROC   glGetProgramInfoLog;
    REGALGLCREATEPROGRAMPROC       glCreateProgram;
    REGALGLATTACHSHADERPROC        glAttachShader;

    template <typename ProcTable>
    void Initialize( const ProcTable & pt ) {
      glUniform1dv             = pt.glUniform1dv;
      glUniform2dv             = pt.glUniform2dv;
      glUniform3dv             = pt.glUniform3dv;
      glUniform4dv             = pt.glUniform4dv;
      glUniform1fv             = pt.glUniform1fv;
      glUniform2fv             = pt.glUniform2fv;
      glUniform3fv             = pt.glUniform3fv;
      glUniform4fv             = pt.glUniform4fv;
      glUniform1iv             = pt.glUniform1iv;
      glUniform2iv             = pt.glUniform2iv;
      glUniform3iv             = pt.glUniform3iv;
      glUniform4iv             = pt.glUniform4iv;
      glUniform1uiv            = pt.glUniform1uiv;
      glUniform2uiv            = pt.glUniform2uiv;
      glUniform3uiv            = pt.glUniform3uiv;
      glUniform4uiv            = pt.glUniform4uiv;
      glUniformMatrix2dv       = pt.glUniformMatrix2dv;
      glUniformMatrix3dv       = pt.glUniformMatrix3dv;
      glUniformMatrix4dv       = pt.glUniformMatrix4dv;
      glUniformMatrix2x3dv     = pt.glUniformMatrix2x3dv;
      glUniformMatrix2x4dv     = pt.glUniformMatrix2x4dv;
      glUniformMatrix3x2dv     = pt.glUniformMatrix3x2dv;
      glUniformMatrix3x4dv     = pt.glUniformMatrix3x4dv;
      glUniformMatrix4x2dv     = pt.glUniformMatrix4x2dv;
      glUniformMatrix4x3dv     = pt.glUniformMatrix4x3dv;
      glUniformMatrix2fv       = pt.glUniformMatrix2fv;
      glUniformMatrix3fv       = pt.glUniformMatrix3fv;
      glUniformMatrix4fv       = pt.glUniformMatrix4fv;
      glUniformMatrix2x3fv     = pt.glUniformMatrix2x3fv;
      glUniformMatrix2x4fv     = pt.glUniformMatrix2x4fv;
      glUniformMatrix3x2fv     = pt.glUniformMatrix3x2fv;
      glUniformMatrix3x4fv     = pt.glUniformMatrix3x4fv;
      glUniformMatrix4x2fv     = pt.glUniformMatrix4x2fv;
      glUniformMatrix4x3fv     = pt.glUniformMatrix4x3fv;
      glGetUniformdv           = pt.glGetUniformdv;
      glGetUniformfv           = pt.glGetUniformfv;
      glGetUniformiv           = pt.glGetUniformiv;
      glGetUniformuiv          = pt.glGetUniformuiv;
      glGetUniformLocation     = pt.glGetUniformLocation;
      glGetShaderiv            = pt.glGetShaderiv;
      glGetShaderSource        = pt.glGetShaderSource;
      glGetAttachedShaders     = pt.glGetAttachedShaders;
      glCreateShader           = pt.glCreateShader;
      glShaderSource           = pt.glShaderSource;
      glCompileShader          = pt.glCompileShader;
      glGetShaderInfoLog       = pt.glGetShaderInfoLog;
      glGetProgramiv           = pt.glGetProgramiv;
      glGetError               = pt.glGetError;
      glGetActiveUniform       = pt.glGetActiveUniform;
      glLinkProgram            = pt.glLinkProgram;
      glGetActiveAttrib        = pt.glGetActiveAttrib;
      glGetAttribLocation      = pt.glGetAttribLocation;
      glBindAttribLocation     = pt.glBindAttribLocation;
      glGetProgramInfoLog      = pt.glGetProgramInfoLog;
      glCreateProgram          = pt.glCreateProgram;
      glAttachShader           = pt.glAttachShader;
    }
  
  };
  
  struct ProgramInstance {
    ProgramInstance() : prog(0), ver(~ GLuint64(0)) {}
    GLint prog;
    GLuint64 ver;
    std::vector<UniformInstance> uniforms;
    void InitializeUniforms( Procs & tbl, const Program & p );
    void UpdateUniforms( Procs & tbl, const Program & p );
  };

  void GetShaderSource( Procs & tbl, GLuint shader, ShaderSource & ss );
  void GetProgramSources( Procs & tbl, GLuint prog, std::vector<ShaderSource> & sources );
  void InitProgram( Procs & tbl, GLuint prog, Program & p );
  void InitProgramInstance( Procs & tbl, const Program & p, GLuint inst, ProgramInstance & pi );
  void CreateProgramInstance( Procs & tbl, const Program & p, const std::vector<ShaderSource> & sources, ProgramInstance & pi );

  int GetTypeSize( GLenum type );
  void GetUniform( Procs & tbl, GLuint program, GLint location, GLsizei count, GLenum type, void *value );
  void SetUniform( Procs & tbl, GLint location, GLsizei count, GLenum type, const void *value );
}

REGAL_NAMESPACE_END

#endif // ! __REGAL_SHADER_INSTANCE_H__
