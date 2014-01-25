#!/usr/bin/python -B

dsaFormulae = {
    'options' : {
      'originate' : [
        'glPixelStoref',
        'glPixelStorei',
      ],
    },
    'SelectorGetCommands' : {
        'entries' : [ 'glGet(Boolean|Integer|Float|Double)v' ],
        'prefix' : [ 'self->RestoreGet( ${arg0} );' ],
    },
    'DsaGetCommands1' : {
        'entries' : [ 'glGet(Float|Double)(Indexed|i_)v(EXT|)' ],
        'impl' : [
            'if ( self->GetIndexedv( ${arg0}, ${arg1}, ${arg2} ) ) {',
            '  return;',
            '}',
        ],
    },
    'DsaGetCommands2' : {
        'entries' : [ 'glGetPointerIndexedvEXT' ],
        'impl' : [
            '// if ( self->GetIndexedv( ${arg0}, ${arg1}, ${arg2} ) ) {',
            '//   return;',
            '// }',
        ],
    },
    'DsaGetCommands3' : {
        'entries' : [ 'glGetFramebufferParameterivEXT' ],
        'impl' : [
            'self->DsaFramebuffer( GL_FRAMEBUFFER, ${arg0} );',
            'RglGetIntegerv( orig, ${arg1plus});',
            'return;',
        ],
    },
    'SelectorIsEnabled' : {
        'entries' : [ 'glIsEnabled' ],
        'prefix' : [ 'self->RestoreIsEnabled( ${arg0} );']
    },
    'DsaIsEnabled' : {
        'entries' : [ 'glIsEnabled(Indexed|i)EXT' ],
        'impl' : [
            'GLboolean ret;',
            'ret = self->IsEnabledIndexed( ${arg0}, ${arg1} );',
            'return ret;',
            ],
    },
    'SelectorMatrixCommands' : {
        'entries' : [
            'gl(Load|Mult)(Transpose|)Matrix(f|d)',
            'glLoadIdentity',
            'gl(Rotate|Scale|Translate)(f|d)',
            'gl(Push|Pop)Matrix',
        ],
        'prefix' : [ 'self->RestoreMatrixMode();' ],
    },
    'MatrixMode' : {
        'entries' : [ 'glMatrixMode' ],
        'impl' : [
            'if (self->ShadowMatrixMode(${arg0})) {',
            '  return;',
            '}',
        ],
    },
    'MatrixLoadMultStack' : {
        'entries' : [
            'glMatrix(Load|Mult)(Transpose|)(f|d)EXT',
        ],
        'impl' : [
            'self->DsaMatrixMode( ${arg0} );',
            'Rgl${m1}${m2}Matrix${m3}( orig, ${arg1plus});',
            'return;',
        ],
    },
    'MatrixPushPopStack' : {
        'entries' : [
            'glMatrix(Push|Pop)EXT',
        ],
        'impl' : [
            'self->DsaMatrixMode( ${arg0} );',
            'Rgl${m1}Matrix( orig );',
            'return;',
        ],
    },
    'MatrixTransform' : {
        'entries' : [
            'glMatrix(Ortho|Frustum)()EXT',
            'glMatrix(Rotate|Scale|Translate)(f|d)EXT',
        ],
        'impl' : [
            'self->DsaMatrixMode( ${arg0} );',
            'Rgl${m1}${m2}( orig, ${arg1plus});',
            'return;',
        ],
    },

    'MatrixIdent' : {
        'entries' : [
            'glMatrixLoadIdentityEXT',
        ],
        'impl' : [
            'self->DsaMatrixMode( ${arg0} );',
            'RglLoadIdentity( orig );',
            'return;',
        ],
    },

    'ClientActiveTexture' : {
        'entries' : [ 'glClientActiveTexture(ARB|)' ],
        'impl' : [
            'if (self->ShadowClientActiveTexture( ${arg0} )) {',
            '  return;',
            '}',
        ],
    },
    'ClientActiveCommands' : {
        'entries' : [
            'gl(Enable|Disable)ClientState',
            'glTexCoordPointer',
        ],
        'prefix' : [
          'self->RestoreClientActiveTexture();',
          'self->RestoreBuffer();',
        ],
    },
    'ClientIndexed' : {
        'entries' : [ 'gl(Enable|Disable)ClientState(Indexed|i)EXT' ],
        'impl' : [
            'self->DsaClientActiveTexture( ${arg1} + GL_TEXTURE0 );',
            'Rgl${m1}ClientState( orig, ${arg0} );',
            'return;',
        ],
    },
    'ClientTexCoords' : {
        'entries' : [ 'glMultiTexCoordPointerEXT' ],
        'impl' : [
            'self->DsaClientActiveTexture( ${arg0} );',
            'RglTexCoordPointer( orig, ${arg1plus} );',
            'return;',
        ],
    },
    'EnableIndexed' : {
        'entries' : [ 'gl(Enable|Disable)IndexedEXT'],
        'impl' : [
            'self->DsaActiveTexture( ${arg1} + GL_TEXTURE0 );',
            'Rgl${m1}( orig, ${arg0} );',
            'return;',
        ],
    },
    'ActiveTexture' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'impl' : [
            'if( self->ShadowActiveTexture( ${arg0} ) ) {',
            '    return;',
            '}',
        ],
    },
    'TexCommands' : {
        'entries' : [
            'gl(Get|)Tex(Env|Parameter)(i|iv|f|fv)',
            'gl(Get|)TexParameter(Iiv|Iuiv)',
            'gl(Copy|Compressed|Get|)Tex(Sub|)Image(1|2|3)D(ARB|OES|)',
            'glTex(Buffer|Renderbuffer)(NV|)',
            'glGenerateMipmap(ARB|EXT|)',
        ],
        'prefix' : [
            'self->RestoreTexture();',
            'self->RestoreActiveTexture();',
        ],
    },
    'TexCommands2' : {
        'entries' : [ 'glTexGen(i|iv|f|fv)', ],
        'prefix' : [ 'self->RestoreActiveTexture();', ],
    },
    'TexCommands3' : {
        'entries' : [ 'gl(Enable|Disable)', ], # check that it's a texture target!
        'prefix' : [ 'self->RestoreActiveTexture();', ],
    },
    'MultiActiveTexCommands' : {
        'entries' : [
            'gl(Get|)MultiTex(Env|Gen|Parameter|LevelParameter)(i|iv|f|fv|d|dv)EXT',
            'gl(Get|)MultiTex(Parameter|LevelParameter)(Iiv|Iuiv)EXT',
            'gl(Copy|Compressed|)MultiTex(Image|SubImage)(1D|2D|3D)EXT',
            'gl(Get|GetCompressed)MultiTex(Image|SubImage)()EXT',
            'gl()MultiTex(Buffer|Renderbuffer)()EXT',
            'gl(Generate)MultiTex(Mipmap)()EXT',
        ],
        'subst' : {
            'nondsa' : {
                '.*Renderbuffer.*' : 'glTexRenderbufferNV',
                'glGenerate.*' : 'glGenerateMipmap',
                'default' : 'gl${m1}Tex${m2}${m3}',
            },
        },
        'impl' : [
            'self->DsaActiveTexture( ${arg0} );',
            'R${nondsa}( orig, ${arg1plus} );',
            'return;',
        ],
    },
    'BindTexture' : {
        'entries' : [ 'glBindTexture(EXT|)' ],
        'impl' : [
            'if (self->ShadowTexture( ${arg0}, ${arg1} )) {',
            '  return;',
            '}',
        ],
    },
    'BindMultiTexture' : {
        'entries' : [ 'glBindMultiTextureEXT' ],
        'impl' : [
            'self->DsaActiveTexture( ${arg0} );',
            'self->ShadowDsaTexture( ${arg1plus} );',
            'RglBindTexture( orig, ${arg1plus} );',
            'return;',
        ],
    },
    'BoundTextureCommands' : {
        'entries' : [
            'gl()Texture(Parameter)(i|iv|f|fv|Iiv|Iuiv)EXT',
            'gl(Get)Texture(Parameter|LevelParameter)(i|iv|f|fv|Iiv|Iuiv)EXT',
            'gl(Get|GetCompressed)Texture(Image)()EXT',
            'gl(Copy|Compressed|)Texture(Image|SubImage)(1D|2D|3D)EXT',
            'gl()Texture(Buffer|Renderbuffer)()EXT',
            'gl(Generate)Texture(Mipmap)()EXT',
            'gl()Texture(Storage)(1D|2D|3D)EXT',
        ],
        'subst' : {
            'nondsa' : {
                '.*Renderbuffer.*' : 'glTexRenderbufferNV',
                'glGenerate.*' : 'glGenerateMipmap',
                'default' : 'gl${m1}Tex${m2}${m3}',
            },
        },
        'impl' : [
            'self->DsaTexture( ${arg1}, ${arg0} );',
            'R${nondsa}( orig, ${arg1plus} );',
            'return;',
        ],
    },

    'UseProgram' : {
        'entries' : [ 'glUseProgram' ],
        'impl' : [
            'if( self->ShadowGlslProgram( ${arg0} ) ) {',
            '  return;',
            '}',
        ],
    },
    'SelectorProgramCommands' : {
        'entries' : [
            'glUniform(1|2|3|4)(i|f|ui|d)(v|)',
            'glUniformMatrix(2|3|4)(x2|x3|x4|)(fv|dv)',
        ],
        'prefix' : [ 'self->RestoreGlslProgram();' ],
    },
    'DsaProgramCommands' : {
        'entries' : [
            'glProgramUniform()(1|2|3|4)()(i|iv|f|fv|d|dv|ui|uiv)EXT',
            'glProgramUniform(Matrix)(2|3|4)(x2|x3|x4|)(fv|dv)EXT',
        ],
        'impl' : [
            'self->DsaGlslProgram( ${arg0});',
            'RglUniform${m1}${m2}${m3}${m4}( orig, ${arg1plus} );',
            'return;',
        ],
    },

    'BindAsmProgram' : {
        'entries' : [ 'glBindProgramARB' ],
        'impl' : [
            'if (self->ShadowAsmProgram( ${arg0}, ${arg1} ) ) {',
            '  return;',
            '}',
        ],
    },
    'AsmProgram' : {
        'entries' : [
            'gl(Get|)Program(Env|Local)Parameter(.*)(ARB|NV)',
            'gl(Get|)Program(String)(EXT)',
        ],
        'prefix' : [ 'self->RestoreAsmProgram( ${arg0} );' ],
    },
    'DsaAsmProgram' : {
        'entries' : [
            'gl(Get|)NamedProgram(Env|Local)Parameter(.*)(EXT)',
            'gl(Get|)NamedProgram(String|iv)()(EXT)',
        ],
        'subst' : {
            'nondsa' : {
                '.*Parameter4[df]v?EXT' : 'gl${m1}Program${m2}Parameter${m3}ARB',
                '.*Parameter[df]vEXT' : 'gl${m1}Program${m2}Parameter${m3}ARB',
                '.*Parameter(s|)I[4uiv]+EXT' : 'gl${m1}Program${m2}Parameter${m3}NV',
                '.*Program(String|iv).*' : 'gl${m1}Program${m2}ARB',
                'default' : 'gl${m1}Program${m2}Parameter${m3}EXT',
            },
        },
        'impl' : [
            'self->DsaAsmProgram( ${arg1}, ${arg0});',
            'R${nondsa}( orig, ${arg1plus} );',
            'return;',
        ],
    },

    'BindFramebuffer' : {
        'entries' : [ 'glBindFramebuffer(EXT|)' ],
        'impl' : [
            'if (self->ShadowFramebuffer( ${arg0}, ${arg1} ) ) {',
            '  return;',
            '}',
        ],
    },
    'SelectorFramebufferCommands' : {
        'entries' : [
            'glFramebufferTexture(1D|2D|3D|Layer|Face|)(ARB|EXT|)',
            'glFramebufferRenderbuffer(ARB|EXT|)',
            'glDrawBuffer(s|)(ARB|EXT|NV|)'
            'glGet(FramebufferAttachment)Parameteriv(ARB|EXT|)',
        ],
        'prefix' : [ 'self->RestoreFramebuffer();' ],
    },
    'SelectorFramebufferCommands2' : {
        'entries' : [
            'gl()Named(FramebufferTexture)(1D|2D|3D|Layer|Face|)EXT',
            'gl()Named(FramebufferRenderbuffer)()EXT',
            'gl(Get)Named(FramebufferAttachment)(Parameteriv)EXT',
        ],
        'subst' : {
            'nondsa' : {
                '.*CoverageEXT' : 'gl${m1}${m2}${m3}NV',
                '.*MultisampleEXT' : 'gl${m1}${m2}${m3}EXT',
                '.*FaceEXT' : 'gl${m1}${m2}${m3}ARB',
                'default' : 'gl${m1}${m2}${m3}',
            },
        },
        'impl' : [
            'self->DsaFramebuffer( GL_FRAMEBUFFER, ${arg0});',
            'R${nondsa}( orig, GL_FRAMEBUFFER, ${arg1plus} );',
            'return;',
        ],
    },
    'SelectorFramebufferCommands3' : {
        'entries' : [ 'glFramebuffer(DrawBuffer|ReadBuffer)(s|)EXT' ],
        'impl' : [
            'self->DsaFramebuffer( GL_FRAMEBUFFER, ${arg0});',
            'Rgl${m1}${m2}( orig, ${arg1plus} );',
            'return;',
        ],
    },
    'SelectorFramebufferCommands4' : {
        'entries' : [ 'glCheckNamedFramebufferStatusEXT' ],
        'impl' : [
            'self->DsaFramebuffer( GL_FRAMEBUFFER, ${arg0});',
            'return RglCheckFramebufferStatus( orig, ${arg1plus} );',
        ],
    },

    'BindRenderbuffer' : {
        'entries' : [ 'glBindRenderbuffer(EXT|)' ],
        'impl' : [
            'if (self->ShadowRenderbuffer( ${arg0}, ${arg1} ) ) {',
            '  return;',
            '}',
        ],
    },
    'SelectorRenderbufferCommands' : {
        'entries' : [
            'glRenderbufferStorage(Multisample|MultisampleCoverage|)(ARB|EXT|NV|)',
            'glGetRenderbufferParameteriv(ARB|EXT|)',
        ],
        'prefix' : [ 'self->RestoreRenderbuffer();' ],
    },
    'SelectorRenderbufferCommands2' : {
        'entries' : [
            'gl()Named(RenderbufferStorage)(Multisample|MultisampleCoverage|)EXT',
            'gl(Get)Named(Renderbuffer)(Parameteriv)EXT',
        ],
        'subst' : {
            'nondsa' : {
                '.*CoverageEXT' : 'gl${m1}${m2}${m3}NV',
                '.*MultisampleEXT' : 'gl${m1}${m2}${m3}',
                'default' : 'gl${m1}${m2}${m3}',
            },
        },
        'impl' : [
            'self->DsaRenderbuffer( GL_RENDERBUFFER, ${arg0});',
            'R${nondsa}( orig, GL_RENDERBUFFER, ${arg1plus} );',
            'return;',
        ],
    },

    'ClientAttribDefault' : {
        'entries' : ['glClientAttribDefaultEXT'],
        'impl' : [ 'self->ClientAttribDefault( ${arg0} ); return;' ],
    },
    'PushClientAttribDefault' : {
        'entries' : ['glPushClientAttribDefaultEXT'],
        'impl' : [
            'RglPushClientAttrib( orig, ${arg0});',
            'self->ClientAttribDefault(${arg0});',
            'return;',
        ],
    },
    'BindVertexArray' : {
        'entries' : [ 'glBindVertexArray' ],
        'impl' : [
            'if (self->ShadowVao( ${arg0} )) {',
            '  return;',
            '}',
        ],
    },
    # TODO - GL_ARB_base_instance ?
    'RestoreEverything' : {
        'entries' : [ 'glBegin', 'gl(Multi|)Draw(Arrays|Element|Elements)(Instanced|Indirect|BaseVertex|InstancedBaseVertex|Array|)(ARB|EXT|AMD|ATI|APPLE|)' ],
        'prefix' : [  'self->Restore();' ],
    },
    'BindBuffer' : {
        'entries' : [ 'glBindBuffer(ARB|)' ],
        'impl' : [
            'if (self->ShadowBuffer( ${arg0}, ${arg1} ) ) {',
            '  return;',
            '}',
        ],
    },
    'SelectorBufferCommands' : {
        'entries' : [
            'gl(Get|Copy|)Buffer(Sub|)Data',
            'gl(FlushMapped|Map|Unmap)Buffer(Range|)(ARB|EXT|)',
            'glGetBuffer(Parameteriv|Pointerv)',
            'gl(Vertex|Normal|Color|SecondaryColor)Pointer',
        ],
        'prefix' : [ 'self->RestoreBuffer();' ],
    },
    'DsaBufferCommands' : {
        'entries' : [
            'gl(Get|)Named(Copy|)Buffer(Data|SubData)EXT',
            'gl(FlushMapped)Named()Buffer(Range)EXT',
            'gl(Get)Named()Buffer(Parameteriv|Pointerv)EXT',
        ],
        'impl' : [
            'self->DsaBuffer( ${arg0});',
            'Rgl${m1}${m2}Buffer${m3}( orig, GL_ARRAY_BUFFER, ${arg1plus} );',
            'return;',
        ],
    },
    'DsaBufferCommands2' : {
        'entries' : [ 'gl(Map)NamedBuffer(Range|)EXT' ],
        'impl' : [
            'self->DsaBuffer( ${arg0});',
            'return Rgl${m1}Buffer${m2}( orig, GL_ARRAY_BUFFER, ${arg1plus} );',
        ],
    },
    'DsaBufferCommands3' : {
        'entries' : [ 'glUnmapNamedBufferEXT' ],
        'impl' : [
            'self->DsaBuffer( ${arg0});',
            'return RglUnmapBuffer( orig, GL_ARRAY_BUFFER );',
        ],
    },
    'DsaVertexArrayOffsetCommands3' : {
        'entries' : [ 'glVertexArray(EdgeFlag)OffsetEXT' ],
        'impl' : [
            'self->DsaVao( ${arg0} );'
            'self->DsaBuffer( ${arg1} );',
            'Rgl${m1}Pointer( orig, ${arg2}, reinterpret_cast<const GLvoid *>(${arg3}) );',
            'return;',
        ],
    },
    'DsaVertexArrayPointerOffsets4' : {
        'entries' : [ 'glVertexArray(Index|Normal|FogCoord)OffsetEXT' ],
        'impl' : [
            'self->DsaVao( ${arg0} );'
            'self->DsaBuffer( ${arg1} );',
            'Rgl${m1}Pointer( orig, ${arg2}, ${arg3}, reinterpret_cast<const GLvoid *>(${arg4}) );',
            'return;',
        ],
    },
    'DsaVertexArrayOffsetCommands5' : {
        'entries' : [ 'glVertexArray(Vertex|Color|TexCoord|SecondaryColor)OffsetEXT' ],
        'impl' : [
            'self->DsaVao( ${arg0} );'
            'self->DsaBuffer( ${arg1} );',
            'Rgl${m1}Pointer( orig, ${arg2}, ${arg3}, ${arg4}, reinterpret_cast<const GLvoid *>(${arg5}) );',
            'return;',
        ],
    },
    'DsaVertexArrayOffsetCommands6' : {
        'entries' : [ 'glVertexArray(VertexAttribI)OffsetEXT' ],
        'impl' : [
            'self->DsaVao( ${arg0} );'
            'self->DsaBuffer( ${arg1} );',
            'Rgl${m1}Pointer( orig, ${arg2}, ${arg3}, ${arg4}, ${arg5}, reinterpret_cast<const GLvoid *>(${arg6}) );',
            'return;',
        ],
    },
    'DsaVertexArrayOffsetCommands6EXT' : {
        'entries' : [ 'glVertexArray(MultiTexCoord)OffsetEXT' ],
        'impl' : [
            'self->DsaVao( ${arg0} );'
            'self->DsaBuffer( ${arg1} );',
            'Rgl${m1}PointerEXT( orig, ${arg2}, ${arg3}, ${arg4}, ${arg5}, reinterpret_cast<const GLvoid *>(${arg6}) );',
            'return;',
        ],
    },
    'DsaVertexArrayOffsetCommands7' : {
        'entries' : [ 'glVertexArray(VertexAttrib)OffsetEXT' ],
        'impl' : [
            'self->DsaVao( ${arg0} );'
            'self->DsaBuffer( ${arg1} );',
            'Rgl${m1}Pointer( orig, ${arg2}, ${arg3}, ${arg4}, ${arg5}, ${arg6}, reinterpret_cast<const GLvoid *>(${arg7}) );',
            'return;',
        ],
    },
    'Delete' : {
        'entries' : [
          'glDelete(Buffers)(ARB|)',
          'glDelete(Framebuffers)(EXT|OES|)',
          'glDelete(Renderbuffers)(EXT|OES|)',
          'glDelete(Program)',
          'glDelete(Programs)(ARB|NV)',
          'glDelete(Textures)(EXT|)',
          'glDelete(VertexArrays)(APPLE|OES|)',
        ],
        'subst' : {
            'method' : {
                '.*Program$'                 : 'GlslProgram',
                '.*Programs(ARB|NV)'         : 'AsmPrograms',
                '.*VertexArrays(APPLE|OES|)' : 'Vaos',
                'default'                    : '${m1}',
            },
        },
        'prefix' : [ 'self->Delete${method}( ${arg0plus} );', ],
    },
    'VertexArrayEnable' : {
        'entries' : [ 'gl(Enable|Disable)VertexArrayEXT' ],
        'impl' : [
            'self->DsaVao( ${arg0} );',
            'if ( ${arg1} >= GL_TEXTURE0 && ${arg1} <= GL_TEXTURE31) {',
            '  self->DsaClientActiveTexture( ${arg1} );',
            '  Rgl${m1}ClientState( orig, GL_TEXTURE_COORD_ARRAY );',
            '} else {',
            '  Rgl${m1}ClientState( orig, ${arg1} );',
            '}'
            'return;',
        ],
    },
    'VertexArrayAttribEnable' : {
        'entries' : [ 'gl(Enable|Disable)VertexArrayAttribEXT' ],
        'impl' : [
            'self->DsaVao( ${arg0} );',
            'Rgl${m1}VertexAttribArray( orig, ${arg1} );'
            'return;',
        ],
    },
}

