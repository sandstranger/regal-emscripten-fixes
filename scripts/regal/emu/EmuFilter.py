#!/usr/bin/python -B

#
# Filter or remap calls that don't exist for ES 2.0
#
# This is the last layer of the emulation dispatch.
#

# TODO List
#
#   - Partial list of entries not supported for ES 2.0
#     Can we do something more systematic for ES 2.0?
#     Perhaps for the Regal.cpp entry points, rather than
#     enlarging the emulation dispatch layer excessively?
#     Compile-time support for REGAL_SYS_ES2, etc would be
#     helpful too, in a desktop OpenGL situation.
#
#   - Partial GL_ARB_shader_objects remapping to OpenGL 2.0 / ES
#     Could be extended to a complete GL_ARB_shader_objects emulation layer?
#
#   - GL_ARB_vertex_program filtering for ES 2.0 isn't complete
#     In addition to matching by entry-point name, could we match
#     according to extension name too?

formulae = {

  # options that aren't part of the normal formulae
  'options' : {
    'originateAllEntries' : True,

    'originate' : [
      'glReadBufferNV',
      'glDrawElements',
      'glMapBufferOES',
      'glUnmapBufferOES',
      'glDrawBuffersNV',
      'glBlitFramebufferNV',
      'glActiveTexture',
      'glAttachShader',
      'glCompileShader',
      'glCreateProgram',
      'glIsProgram',
      'glGetProgramInfoLog',
      'glGetShaderInfoLog',
      'glGetProgramiv',
      'glGetShaderiv',
      'glGetUniformLocation',
      'glUniform1i',
      'glBufferData',
      'glBindAttribLocation',
      'glBlendColor',
      'glBlendEquation',
      'glColorMask',
      'glDisable',
      'glEnable',
      'glIsEnabled',
      'glBindRenderbuffer',
      'glCheckFramebufferStatus',
      'glDeleteFramebuffers',
      'glDeleteRenderbuffers',
      'glGenFramebuffers',
      'glGenRenderbuffers',
      'glGetRenderbufferParameteriv',
      'glIsFramebuffer',
      'glIsRenderbuffer',
      'glRenderbufferStorage',
    ],
  },

  # Skip calls that ES 2.0 and/or core do not support
  # Issues: This list is not complete

  'filterOutES2' : {
    'entries' : [
      'glClientActiveTexture',
      'glGenSamplers',                                   # Sampler object emulation
      'glGetTexImage',
      'glTexImage(1|3)D',
      'glGetTexLevelParameter(f|i)v',
      'glBlitFramebufferANGLE'                           # Emulate glBlitFramebuffer?
    ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  Warning("Regal does not support ${name} for ES 2.0 - skipping.");',
       '  #if REGAL_BREAK',
       '  Break::Filter();',
       '  #endif',
       '  return ${dummyretval};',
       '}'
     ],
    'originate' : [
    ]
  },

  'filterOutCore' : {
    'entries' : [ 'glLineWidth', ],
    'impl' : [
       'if (_context->isCore())',
       '{',
       '   Warning("Regal does not support ${name} for core profile - skipping.");',
       '   #if REGAL_BREAK',
       '   Break::Filter();',
       '   #endif',
       '   return ${dummyretval};',
       '}'
     ]
  },

  'filterOutES2AndCore' : {
    'entries' : [
      'glAccum',
      'glBitmap',
      'glCallList',
      'glClearAccum',
      'glCopyPixels',
      'glDeleteLists',
      'glDrawPixels',
      'glEdgeFlag',
      'glEndList',
      'glEvalCoord(1|2)(d|f)(v|)',
      'glEvalMesh(1|2)',
      'glEvalPoint(1|2)',
      'glGenLists',
      'glLineStipple',
      'glMap(1|2)(d|f)',
      'glMapGrid(1|2)(d|f)',
      'glNewList',
      'glPixelStoref',
      'glPixelTransfer(f|i)',
      'glPixelZoom',
      'glRasterPos(2|3|4)(d|f|i|s)(v|)',
      'glRect(d|f|i|s)',
      'glShadeModel',
      'glWindowPos(2|3)(d|f|i|s)(v|)',
      ],
    'impl' : [
       'if (_context->isES2() || _context->isCore())',
       '{',
       '   Warning("Regal does not support ${name} for core or ES2 profiles - skipping.");',
       '   #if REGAL_BREAK',
       '   Break::Filter();',
       '   #endif',
       '   return ${dummyretval};',
       '}'
     ]
  },


  'filterArgs' : {
    'entries' : [
      'gl(BindTexture)',
      'gl(FramebufferTexture2D)',
      'gl(GenerateMipmap)',
      'gl(Get)(Boolean|Double|Float|Integer|Integer64)v',
      'gl(GetTexParameteriv)',
      'gl(PixelStorei)',
      'gl(PolygonMode)',
      'gl(RenderMode)',
      'gl(TexImage2D)',
      ],
    'impl' : [
       'if (_context->filt->${m1}(*_context, ${arg0plus}))',
       '{',
       '  #if REGAL_BREAK',
       '  Break::Filter();',
       '  #endif',
       '  return ${dummyretval};',
       '}'
     ]
  },

  'texParameter' : {
    'entries' : [
      'glTexParameter(i|f)',
      ],
    'impl' : '''
if (_context->filt->TexParameter(*_context, ${arg0}, ${arg1}))
  return;
GLfloat newparam;
if (_context->filt->FilterTexParameter(*_context, ${arg0}, ${arg1}, static_cast<GLfloat>(${arg2}), newparam))
  _context->filt->orig.glTexParameterf(${arg0}, ${arg1}, newparam);
else
  _context->filt->orig.glTexParameter${m1}(${arg0plus});
return;'''
  },

  'texParameterv' : {
    'entries' : [
      'glTexParameter(i|f)v',
      ],
    'impl' : '''
if (_context->filt->TexParameter(*_context, ${arg0}, ${arg1}))
  return;
GLfloat newparam;
if (${arg2} && _context->filt->FilterTexParameter(*_context, ${arg0}, ${arg1}, static_cast<GLfloat>(${arg2}[0]), newparam))
  _context->filt->orig.glTexParameterf(${arg0}, ${arg1}, newparam);
else
  _context->filt->orig.glTexParameter${m1}v(${arg0plus});
return;'''
  },

  # Remap glBlitFramebuffer calls to GL_NV_framebuffer_blit
  # http://www.khronos.org/registry/gles/extensions/NV/GL_NV_framebuffer_blit.txt

  'blitFBO' : {
    'entries' : [ 'glBlitFramebuffer' ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  if (_context->info->gl_nv_framebuffer_blit)  return _context->filt->orig.${m0}NV(${arg0plus});',
       '  if (_context->info->gl_ext_framebuffer_blit) return _context->filt->orig.${m0}EXT(${arg0plus});',
       '}'
     ]
  },

  # Filter out glDrawBuffer unless GL_NV_framebuffer_blit
  # or GL_EXT_framebuffer_blit is available

  'blitDrawRead' : {
    'entries' : [ 'glDrawBuffer' ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  if (_context->info->gl_nv_framebuffer_blit || _context->info->gl_ext_framebuffer_blit)',
       '    return _context->filt->orig.${m0}(${arg0plus});',
       '}'
     ]
  },

  # Convert DrawRangeElements to ES DrawElements, ignoring second and third arguments

  'glDrawRangeElements' : {
    'entries' : [ 'glDrawRangeElements' ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  return _context->filt->orig.glDrawElements(${arg0}, ${arg3plus});',
       '}'
     ]
  },

  # Convert DrawRangeElementsBaseVertex to DrawElements iff baseVertex == 0
  # GL_ARB_draw_elements_base_vertex
  # http://www.opengl.org/registry/specs/ARB/draw_elements_base_vertex.txt

  'glDrawRangeElementsBaseVertex' : {
    'entries' : [ 'glDrawRangeElementsBaseVertex' ],
    'impl' : [
       'if (REGAL_FORCE_ES2_PROFILE || !_context->info->gl_arb_draw_elements_base_vertex)',
       '{',
       '  if (basevertex==0)',
       '  {',
       '    return _context->filt->orig.glDrawElements(${arg0}, ${arg3}, ${arg4}, ${arg5});',
       '  }',
       '  else',
       '  {',
       '    Warning("Regal does not support ${name} (GL_ARB_draw_elements_base_vertex extension not available) for basevertex!=0 for ES 2.0 - skipping.");',
       '    return;',
       '  }',
       '}'
     ]
  },

  'glDrawBuffers' : {
    'entries' : [ 'glDrawBuffers' ],
    'impl' : [
       'if (_context->filt->DrawBuffers(*_context, ${arg0plus}))',
       '{',
       '  #if REGAL_BREAK',
       '  Break::Filter();',
       '  #endif',
       '  return ${dummyretval};',
       '}',
       'if (_context->isES2())',
       '{',
       '  if (_context->info->gl_nv_draw_buffers)',
       '  {',
       '    _context->filt->orig.${name}NV(${arg0plus});',
       '    return;',
       '  }',
       '}'
     ]
  },

  # http://www.opengl.org/registry/specs/ATI/draw_buffers.txt
  'GL_ATI_draw_buffers' : {
    'entries' : [ 'glDrawBuffersATI' ],
    'impl' : [
       'if (!_context->info->gl_ati_draw_buffers)',
       '{',
       '  _context->emuLevel++;',
       '  _context->filt->orig.glDrawBuffers(${arg0plus});',
       '  return;',
       '}'
     ]
  },

  # http://www.opengl.org/registry/specs/ARB/draw_buffers.txt
  'GL_ARB_draw_buffers' : {
    'entries' : [ 'glDrawBuffersARB'],
    'impl' : [
       'if (!_context->info->gl_arb_draw_buffers)',
       '{',
       '  _context->emuLevel++;',
       '  _context->filt->orig.glDrawBuffers(${arg0plus});',
       '  return;',
       '}'
     ]
  },

  # http://www.opengl.org/registry/specs/ARB/vertex_program.txt
  # ARB assembly programs not supported or emulated for ES 2.0 (yet)

  'GL_ARB_vertex_program' : {
    'entries' : [ 'glGenProgramsARB', 'glBindProgramARB', 'glProgramStringARB', 'glGetProgramivARB' ],
    'impl' : [
      'if (_context->isES2())',
      '{',
      '  Warning("Regal does not support ${name} (GL_ARB_vertex_program) for ES 2.0 context - skipping.");',
      '  return;',
      '}'
     ]
  },

  # Remap GL_ARB_shader_objects to GL 2.0 API
  # http://www.opengl.org/registry/specs/ARB/shader_objects.txt
  #
  # TODO - Is this complete for GL_ARB_shader_objects API?

#    'glCreateShaderObjectARB' : {
#        'entries' : [ 'glCreateShaderObjectARB' ],
#        'impl' : [
#           '#if !REGAL_FORCE_ES2_PROFILE',
#           'if (_context->info->es2)',
#           '#endif',
#           '{',
#           '  return _context->filt->orig.glCreateShader(${arg0plus});',
#           '}'
#         ]
#    },

  'glCreateProgramObjectARB' : {
    'entries' : [ 'glCreateProgramObjectARB' ],
    'impl' : [
      'if (_context->isES2() || !_context->info->gl_arb_shader_objects)',
      '{',
      '  return _context->filt->orig.glCreateProgram(${arg0plus});',
      '}'
    ]
  },

#    'glShaderSourceARB' : {
#        'entries' : [ 'glShaderSourceARB' ],
#        'impl' : [
#           '#if !REGAL_FORCE_ES2_PROFILE',
#           'if (_context->info->es2)',
#           '#endif',
#           '{',
#           '  _context->filt->orig.glShaderSource(${arg0plus});',
#           '  return;',
#           '}'
#         ]
#    },

  'glCompileShaderARB' : {
    'entries' : [ 'glCompileShaderARB' ],
    'impl' : [
      'if (_context->isES2() || !_context->info->gl_arb_shader_objects)',
      '{',
      '  _context->filt->orig.glCompileShader(${arg0plus});',
      '  return;',
      '}'
    ]
  },

  'glActiveTextureARB' : {
    'entries' : [ 'glActiveTextureARB' ],
    'impl' : [
      'if (!_context->info->gl_arb_multitexture)',
      '{',
      '  _context->filt->orig.glActiveTexture(${arg0plus});',
      '  return;',
      '}'
    ]
  },

  'glClientActiveTextureARB' : {
    'entries' : [ 'glClientActiveTextureARB' ],
    'impl' : [
      'if (!_context->info->gl_arb_multitexture)',
      '{',
      '  _context->emuLevel++;',
      '  _context->filt->orig.glClientActiveTexture(${arg0plus});',
      '  return;',
      '}'
    ]
  },

  'glAttachObjectARB' : {
    'entries' : [ 'glAttachObjectARB' ],
    'impl' : [
      'if (_context->isES2() || !_context->info->gl_arb_shader_objects)',
      '{',
      '  _context->filt->orig.glAttachShader(${arg0plus});',
      '  return;',
      '}'
    ]
  },

  'glBindAttribLocationARB' : {
    'entries' : [ 'glBindAttribLocationARB' ],
    'impl' : [
      'if (_context->isES2() || !_context->info->gl_arb_shader_objects)',
      '{',
      '  _context->filt->orig.glBindAttribLocation(${arg0plus});',
      '  return;',
      '}'
    ]
  },

  'glGetUniformLocationARB' : {
    'entries' : [ 'glGetUniformLocationARB' ],
    'impl' : [
      'if (_context->isES2() || !_context->info->gl_arb_shader_objects)',
      '{',
      '  return _context->filt->orig.glGetUniformLocation(${arg0plus});',
      '}'
    ]
  },

  'glUniform1iARB' : {
    'entries' : [ 'glUniform1iARB' ],
    'impl' : [
      'if (_context->isES2() || !_context->info->gl_arb_shader_objects)',
      '{',
      '  _context->filt->orig.glUniform1i(${arg0plus});',
      '  return;',
      '}'
    ]
 },

  'glGetObjectParameterivARB' : {
    'entries' : [ 'glGetObjectParameterivARB' ],
    'impl' : [
      'if (_context->isES2() || !_context->info->gl_arb_shader_objects)',
       '{',
       '  if (_context->filt->orig.glIsProgram(obj))',
       '    _context->filt->orig.glGetProgramiv(${arg0plus});',
       '  else',
       '    _context->filt->orig.glGetShaderiv(${arg0plus});',
       '  return;',
       '}'
     ]
  },

  'glGetInfoLogARB' : {
    'entries' : [ 'glGetInfoLogARB' ],
    'impl' : [
      'if (_context->isES2() || !_context->info->gl_arb_shader_objects)',
       '{',
       '  if (_context->filt->orig.glIsProgram(obj))',
       '    _context->filt->orig.glGetProgramInfoLog(${arg0plus});',
       '  else',
       '    _context->filt->orig.glGetShaderInfoLog(${arg0plus});',
       '  return;',
       '}'
     ]
  },

  # Remap glBlendEquationEXT for ES 2.0

  'glBlendEquationEXT' : {
    'entries' : [ 'glBlendEquationEXT' ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  _context->filt->orig.glBlendEquation(${arg0plus});',
       '  return;',
       '}'
     ]
  },

  # Remap glBlendColorEXT for ES 2.0

  'glBlendColorEXT' : {
    'entries' : [ 'glBlendColorEXT' ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  _context->filt->orig.glBlendColor(${arg0plus});',
       '  return;',
       '}'
     ]
  },

# http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
# Map to GL_OES_mapbuffer for ES 2.0

  'MapUnmap' : {
    'entries' : [ 'gl(Map|Unmap)Buffer(ARB|)' ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  return _context->filt->orig.gl${m1}BufferOES(${arg0plus});',
       '}'
     ]
  },

  'glBufferDataARB' : {
    'entries' : [ 'glBufferDataARB' ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  _context->filt->orig.glBufferData(${arg0plus});',
       '  return;',
       '}'
     ]
  },

# http://www.opengl.org/registry/specs/ARB/framebuffer_object.txt
# Force target = GL_FRAMEBUFFER for ES 2.0

  'glBindFramebuffer' : {
    'entries' : [ 'glBindFramebuffer','glBindFramebufferOES' ],
    'impl' : [
       'if (_context->isES2())',
       '{',
       '  const bool hasFBBlit = _context->info->gl_ext_framebuffer_blit || _context->info->gl_nv_framebuffer_blit || _context->info->gl_version_major >= 3;',
       '  if (!hasFBBlit && (target==GL_DRAW_FRAMEBUFFER || target==GL_READ_FRAMEBUFFER)) target = GL_FRAMEBUFFER;',
       '}',
       'if (_context->filt->BindFramebuffer(*_context, ${arg0plus}))',
       '{',
       '  #if REGAL_BREAK',
       '  Break::Filter();',
       '  #endif',
       '  return ${dummyretval};',
       '}'
     ]
  },

# Check for unsupported GL_COLOR_ATTACHMENT1+ on ES 2.0

  'framebuffer_object_attachment' : {
    'entries' : [ 'glFramebuffer(Texture1D|Texture3D|Renderbuffer)' ],
    'impl' : [
       'if (_context->filt->FramebufferAttachmentSupported(*_context, ${arg1}))',
       '  _context->filt->orig.glFramebuffer${m1}(${arg0plus});',
       'return;'
     ]
  },

# Check for unsupported GL_COLOR_ATTACHMENT1+ on ES 2.0
# FIXME - also filter out FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET?

  'glGetFramebufferAttachmentParameteriv' : {
    'entries' : [ 'glGetFramebufferAttachmentParameteriv' ],
    'impl' : [
       'if (!_context->filt->FramebufferAttachmentSupported(*_context, ${arg1}))',
       '  *${arg3} = 0;',
       'else',
       '  _context->filt->orig.glGetFramebufferAttachmentParameteriv(${arg0plus});',
       'return;'
     ]
  },

# http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
#
# Note we deliberately call the target (non-extension) functions at the same
# emulation level so that we can do any necessary parameter filtering in
# fewer places.

  'EXT_framebuffer_object' : {
    'entries' : [ 'gl(BindRenderbuffer|DeleteRenderbuffers|GenRenderbuffers)EXT',
                  'gl(GetRenderbufferParameteriv|RenderbufferStorage)EXT',
                  'gl(DeleteFramebuffers|GenFramebuffers|BindFramebuffer)EXT',
                  'gl(GenerateMipmap|GetFramebufferAttachmentParameteriv)EXT',
                  'gl(FramebufferTexture1D|FramebufferTexture2D|FramebufferTexture3D)EXT',
                  'gl(FramebufferRenderbuffer)EXT'
    ],
    'impl' : [
       'if (!_context->info->gl_ext_framebuffer_object)',
       '{',
       '  _context->emuLevel++;',
       '  _context->filt->orig.gl${m1}(${arg0plus});',
       '  return;',
       '}'
     ]
  },

  'EXT_framebuffer_object_returning' : {
    'entries' : [ 'gl(CheckFramebufferStatus|IsFramebuffer|IsRenderbuffer)EXT' ],
    'impl' : [
       'if (!_context->info->gl_ext_framebuffer_object)',
       '{',
       '  _context->emuLevel++;',
       '  return _context->filt->orig.gl${m1}(${arg0plus});',
       '}'
     ]
  },

# http://www.opengl.org/registry/specs/EXT/framebuffer_blit.txt

  'glBlitFramebufferEXT' : {
    'entries' : [ 'glBlitFramebufferEXT' ],
    'impl' : [
       'if (!_context->info->gl_ext_framebuffer_blit)',
       '{',
       '  _context->emuLevel++;',
       '  _context->filt->orig.glBlitFramebuffer(${arg0plus});',
       '  return;',
       '}'
     ]
  },

# glReadBuffer

  'glReadBuffer' : {
    'entries' : [ 'glReadBuffer' ],
    'impl' : [
       'if (_context->filt->ReadBuffer(*_context, ${arg0plus}))',
       '{',
       '  #if REGAL_BREAK',
       '  Break::Filter();',
       '  #endif',
       '  return ${dummyretval};',
       '}',
       'if (_context->isES2() && _context->info->gl_nv_read_buffer)',
       '  _context->filt->orig.glReadBufferNV(${arg0plus});',
       'else',
       '  _context->filt->orig.glReadBuffer(${arg0plus});',
       'return;'
     ]
  },



#
# http://www.opengl.org/registry/specs/ARB/draw_buffers.txt
# http://www.opengl.org/registry/specs/EXT/draw_buffers2.txt
#
# Without MRT, limit calls to DRAW_BUFFER0_ARB for ES 2.0
#

  'glColorMaskIndexedEXT' : {
    'entries' : [ 'glColorMaskIndexedEXT' ],
    'impl' : [
       'if (REGAL_FORCE_ES2_PROFILE || !_context->info->gl_ext_draw_buffers2)',
       '{',
       '  if (!buf)'
       '  {',
       '    _context->filt->orig.glColorMask(${arg1plus});',
       '  }',
       '  return;',
       '}'
     ]
  },

  'glGetBooleanIndexedvEXT' : {
    'entries' : [ 'glGetBooleanIndexedvEXT' ],
    'impl' : [
       'if (REGAL_FORCE_ES2_PROFILE || !_context->info->gl_ext_draw_buffers2)',
       '{',
       '  if (!index)'
       '  {',
       '    _context->filt->orig.glGetBooleanv(${arg0},${arg2});',
       '  }',
       '  return;',
       '}'
     ]
  },

  'glGetIntegerIndexedvEXT' : {
    'entries' : [ 'glGetIntegerIndexedvEXT' ],
    'impl' : [
       'if (REGAL_FORCE_ES2_PROFILE || !_context->info->gl_ext_draw_buffers2)',
       '{',
       '  if (!index)'
       '  {',
       '    _context->filt->orig.glGetIntegerv(${arg0},${arg2});',
       '  }',
       '  return;',
       '}'
     ]
  },

  'glEnableIndexedEXT' : {
    'entries' : [ 'glEnableIndexedEXT' ],
    'impl' : [
       'if (${arg0}==GL_BLEND && (REGAL_FORCE_ES2_PROFILE || !_context->info->gl_ext_draw_buffers2))',
       '{',
       '  if (!index)'
       '  {',
       '    _context->filt->orig.glEnable(${arg0});',
       '  }',
       '  return;',
       '}'
     ]
  },

  'glDisableIndexedEXT' : {
    'entries' : [ 'glDisableIndexedEXT' ],
    'impl' : [
       'if (${arg0}==GL_BLEND && (REGAL_FORCE_ES2_PROFILE || !_context->info->gl_ext_draw_buffers2))',
       '{',
       '  if (!index)'
       '  {',
       '    _context->filt->orig.glDisable(${arg0});',
       '  }',
       '  return;',
       '}'
     ]
  },

  'glIsEnabledIndexedEXT' : {
    'entries' : [ 'glIsEnabledIndexedEXT' ],
    'impl' : [
       'if (${arg0}==GL_BLEND && !_context->info->gl_ext_draw_buffers2)',
       '{',
       '  if (!index)'
       '  {',
       '    return _context->filt->orig.glIsEnabled(${arg0});',
       '  }',
       '  return GL_FALSE;',
       '}'
     ]
  }

}
