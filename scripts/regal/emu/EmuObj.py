#!/usr/bin/python -B

objFormulae = {
    'options' : {
      'originate' : [
        'glGenBuffers',
        'glDeleteBuffers',
        'glGenVertexArrays',
        'glDeleteVertexArrays',
        'glGenTextures',
        'glDeleteTextures',
      ],
    },
    'Buffers' : {
        'entries' : [ 'gl(GenBuffers|DeleteBuffers|BindBuffer)(ARB|)' ],
        'impl' : [ 'self->${m1}(${arg0plus}); return;', ],
    },
    'VAOs' : {
        'entries' : [ 'gl(GenVertexArrays|DeleteVertexArrays|BindVertexArray)(ARB|)' ],
        'impl' : [ 'self->${m1}(${arg0plus}); return;', ],
    },
    'Textures' : {
        'entries' : [ 'gl(GenTextures|DeleteTextures|BindTexture)' ],
        'impl' : [ 'self->${m1}(${arg0plus}); return;', ],
    },
    'FramebufferTexture' : {
        'entries' : [ 'glFramebufferTexture(1D|1DEXT|2D|2DEXT|2DOES|2DMultisampleEXT|2DMultisampleOES|3DEXT|3DOES)' ],
        'impl' : ['RglFramebufferTexture${m1}( orig,${arg0}, ${arg1}, ${arg2}, self->textureNames.ToDriverName(${arg3}), ${arg4plus}); return;', ],
     },
    'GetFramebufferAttachmentParameteriv' : {
        'entries' : [ 'glGetFramebufferAttachmentParameteriv(EXT|)' ],
        'impl' : [
            'RglGetFramebufferAttachmentParameteriv${m1}( orig,${arg0plus});',
            'if (${arg2} == GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME)',
            '{',
            '  GLint attachType = GL_RENDERBUFFER;',
            '  RglGetFramebufferAttachmentParameteriv${m1}( orig,${arg0}, ${arg1}, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachType);',
            '  if (attachType == GL_TEXTURE)',
            '    *${arg3} = self->textureNames.ToAppName(*${arg3});',
            '}',
            'return;',
        ],
    },
    'IsObj' : {
        'entries' : [ 'glIs(Buffer|VertexArray|Texture)(ARB|)' ],
        'impl' : [ 'return self->Is${m1}(${arg0plus});', ],
    },
}
