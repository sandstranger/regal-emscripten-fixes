#!/usr/bin/python -B

iffFormulae = {
    'options' : {
      'originate' : [
        'glCreateProgram',
        'glDeleteBuffers',
        'glDeleteProgram',
        'glDeleteVertexArrays',
        'glAttachShader',
        'glCreateShader',
        'glDeleteShader',
        'glCompileShader',
        'glShaderSource',
        'glGetProgramiv',
        'glGetShaderiv',
        'glGetShaderInfoLog',
        'glGetProgramInfoLog',
        'glBindAttribLocation',
        'glGetUniformLocation',
        'glUniform4fv',
        'glUniform2f',
        'glUniform1i',
        'glUniformMatrix4fv',
        'glUniformMatrix3fv',
        'glGenVertexArrays',
        'glGenBuffers',
        'glBufferData',
      ],
    },
    'VaPointer4EXTOverride' : {
        'entries' : [ 'gl(Vertex|Color|TexCoord)PointerEXT' ],
        'impl' : [ 'self->${m1}Pointer(${arg0}, ${arg1}, ${arg2}, ${arg4} ); return;' ],
    },
    'EdgeFlagPointerEXT' : {
        'entries' : [ 'glEdgeFlagPointerEXT' ],
        'impl' : [ 'self->EdgeFlagPointer(${arg0}, ${arg2} ); return;' ],
    },
    'NormalPointerEXT' : {
        'entries' : [ 'glNormalPointerEXT' ],
        'impl' : [ 'self->NormalPointer(${arg0}, ${arg1}, ${arg3} ); return;' ],
    },
    'PointerEXT' : {
        'entries' : [ 'gl(FogCoord|SecondaryColor)PointerEXT' ],
        'impl' : [ 'self->${m1}Pointer(${arg0plus} ); return;' ],
    },
    'VaPointerOverride' : {
        'entries' : [ 'gl(Vertex|Normal|Color|SecondaryColor|FogCoord|EdgeFlag|TexCoord)Pointer' ],
        'impl' : [ 'self->${m1}Pointer(${arg0plus} ); return;' ],
    },
    'VaClientStateOverride' : {
        'entries' : [ 'gl(Enable|Disable)ClientState' ],
        'impl' : [ 'self->${m1}ClientState(${arg0} ); return;' ],
    },
    'IsEnabled' : {
        'entries' : [ 'glIsEnabled' ],
        'impl' : [
            'GLboolean enabled;',
            'if ( self->IsEnabled(${arg0plus}, enabled ) ) {',
            '  return enabled;',
            '}',
        ]
    },
    'ImmShadowVao' : {
        'entries' : [ 'glBindVertexArray.*', ],
        'prefix' : [ 'self->glBindVertexArray(${arg0} ); ', ],
    },
   'IsVertexArray' : {
        'entries' : [ 'glIsVertexArray(ARB|)' ],
        'impl' : [ 'return self->IsVertexArray(${arg0} );' ],
    },
    'ImmShadowClientActiveTexture' : {
        'entries' : [ 'glClientActiveTexture(ARB|)', ],
        'prefix' : [ 'self->ShadowClientActiveTexture( ${arg0} ); ', ],
    },
    'ImmBegin' : {
        'entries' : [ 'glBegin' ],
        'impl' : [ 'self->Begin(${arg0} ); return;', ],
    },
    'ImmEnd' : {
        'entries' : [ 'glEnd' ],
        'impl' : [ 'self->End( _context ); return;', ],
    },
    'ImmAttr' : {
        'entries' : [ 'glVertexAttrib(1|2|3|4)(N|)(b|d|f|i|s|ub|us)(v|)(ARB|)' ],
        'prefix' : [ 'self->Attr${m2}<${m1}>(${arg0plus} );', ],
    },
    'ImmFixedAttrf' : {
        'entries' : [ 'gl(SecondaryColor|Color|Normal|FogCoord)(2|3|4)(d|f)(v|)(EXT|)?' ],
        'impl' : [ 'self->Attr<${m2}>(self->AttrIndex( RFF2A_${m1} ), ${arg0plus} ); return;', ],
    },
    'ImmVertex' : {
        'entries' : [ 'gl(Vertex)(2|3|4)(d|f|i|s)(v|)' ],
        'impl' : [ 'self->Attr<${m2}>(self->AttrIndex( RFF2A_${m1} ), ${arg0plus} ); return;', ],
    },
    'ImmFixedAttri' : {
        'entries' : [ 'gl(SecondaryColor|Color|Normal)(2|3|4)(b|i|s|ub|ui|us)(v|)(EXT|)?' ],
        'impl' : [ 'self->AttrN<${m2}>(self->AttrIndex( RFF2A_${m1} ), ${arg0plus} ); return;', ],
    },
    'ImmTexCoord' : {
        'entries' : [ 'glTexCoord(1|2|3|4)(d|f|i|s)(v|)' ],
        'impl' : [ 'self->Attr<${m1}>(self->AttrIndex( RFF2A_TexCoord ), ${arg0plus} ); return;', ],
    },
    'ImmMultiTexCoord' : {
        'entries' : [ 'glMultiTexCoord(1|2|3|4)(d|f|i|s)(v|)(ARB|)' ],
        'impl' : [ 'self->Attr<${m1}>(self->AttrIndex( RFF2A_TexCoord, ${arg0} - GL_TEXTURE0 ), ${arg1plus} ); return;', ],
    },
    'ImmRestore' : {
        'entries' : [
            'glBindBuffer',
#           'gl(Enable|Disable)ClientState',
            'glVertexAttribPointer(ARB|)',
#           'gl(Vertex|Normal|Color|SecondaryColor|FogCoord|TexCoord)Pointer',
            'glGetVertexAttrib(d|f|i|Pointer)v(ARB|)',
            ],
        'prefix' : [ 'self->RestoreVao( _context );' ],
    },



    'FfnShadowARB' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'prefix' : [ 'self->ShadowActiveTexture( ${arg0} ); ', ],
    },
    'FfnShadeModel' : {
        'entries' : [ 'glShadeModel' ],
        'prefix' : [ 'self->ShadeModel( ${arg0plus} );', ],
    },
    'FfnShadow' : {
        'entries' : [ 'gl(MatrixMode|BindProgramPipeline|Enable|Disable)' ],
        'impl' : [
            'if( self->Shadow${m1}( ${arg0plus} ) ) {',
            '    return;',
            '}',
            ],
    },
    'FfnShadowProgram' : {
        'entries' : [ 'gl(UseProgram)(ObjectARB|)' ],
        'impl' : [
            'if( self->Shadow${m1}( ${arg0plus} ) ) {',
            '    return;',
            '}',
            ],
    },
    'FfnShadowIndexed' : {
        'entries' : [ 'gl(Enable|Disable)(i|IndexedEXT)' ],
        'impl' : [ 'self->${m1}Indexed( ${arg0plus} ); return;', ],
    },
    'FfnShadowTexBinding' : {
        'entries' : [ 'glBind(Multi|)Texture(EXT|)' ],
        'prefix' : [ 'self->Shadow${m1}TexBinding( ${arg0plus} );', ],
    },
    'FfnShadowDsaTexInfo' : {
        'entries' : [ 'gl(MultiTex|Texture)Image.*EXT' ],
        'prefix' : [ 'self->Shadow${m1}Info( ${arg0}, ${arg1}, ${arg3} );', ],
    },
    'FfnShadowTexInfo' : {
        'entries' : [ 'glTexImage(1|2|3|4)D(Multisample)?(EXT|ARB|SGIS|)' ],
        'prefix' : [ 'self->ShadowTexInfo( ${arg0}, ${arg2} );', ],
    },
    'FfnShadowTexInfo2' : {
        'entries' : [ 'glTexImage(1|2|3|4)DMultisampleCoverageNV' ],
        'prefix' : [ 'self->ShadowTexInfo( ${arg0}, ${arg3} );', ],
    },
    'FfnShadowTexInfo3' : {
        'entries' : [ 'glCopyTexImage2D' ],
        'prefix' : [ 'self->ShadowTexInfo( ${arg0}, ${arg2} );', ],
    },
    'FfnTexEnv' : {
        'entries' : [ 'gl(Multi|)TexEnv(i|f)(v|)(EXT|)' ],
        'impl' : [ 'self->TexEnv( ${arg0plus} ); return;', ],
    },
    'FfnGetTexEnv' : {
        'entries' : [ 'glGetTexEnv(i|f)(v|)(EXT|)' ],
        'impl' : [
            'self->RestoreVao( _context );',
            'if ( self->GetTexEnv( ${arg0plus} ) ) {',
            '    return;',
            '}',
        ],
    },
    'FfnLightMatModel' : {
        'entries' : [ 'gl(Light|Material|GetMaterial|LightModel)(i|f)(v|)' ],
        'impl' : [ 'self->${m1}( ${arg0plus} ); return;', ],
    },
    'FfnColorMaterial' : {
        'entries' : [ 'glColorMaterial' ],
        'impl' : [ 'self->ColorMaterial( ${arg0plus} ); return;', ],
    },
    'FfnTexGen' : {
        'entries' : [ 'glTexGen(i|f|d)(v|)' ],
        'impl' : [
            'if ( self->TexGen( ${arg0plus} ) ) {',
            '    return;',
            '}',
        ],
    },
    'FfnAlphaFunc' : {
        'entries' : [ 'glAlphaFunc' ],
        'impl' : [ 'self->AlphaFunc( ${arg0plus} ); return;', ],
    },
    'FfnClipPlane' : {
        'entries' : [ 'glClipPlane' ],
        'impl' : [ 'self->ClipPlane( ${arg0plus} ); return;', ],
    },
    'FfnFog' : {
        'entries' : [ 'glFog(f|i)(v|)' ],
        'impl' : [ 'self->Fog( ${arg0plus} ); return;', ],
    },
    'FfnGetBoolean' : {
        'entries' : [ 'glGetBooleanv' ],
        'impl' : [
            'self->RestoreVao( _context );',
            'if ( self->glGetBooleanv(${arg0plus} ) ) {',
            '    return;',
            '}',
        ],
    },
    'FfnGet' : {
        'entries' : [ 'glGet(Integer|Float|Double)v' ],
        'impl' : [
            'self->RestoreVao( _context );',
            'if ( self->Get(${arg0plus} ) ) {',
            '    return;',
            '}',
        ],
    },
    'FfnGetMultiTexGenv' : {
        'entries' : [ 'glGetMultiTexGen(d|f|i)vEXT' ],
        'impl' : [
            'self->RestoreVao( _context );',
            'if ( self->GetMultiTexGenv(${arg0plus} ) ) {',
            '    return;',
            '}',
        ],
    },
    'FfnGetTexGenv' : {
        'entries' : [ 'glGetTexGen(d|f|i)v' ],
        'impl' : [
            'self->RestoreVao( _context );',
            'if ( self->GetTexGenv(${arg0plus} ) ) {',
            '    return;',
            '}',
        ],
    },
    # TODO - GL_ARB_base_instance ?
    'FfnPreDraw' : {
        'entries' : [ 'gl(Multi|)Draw(Range|)(Arrays|Element|Elements)(Instanced|Indirect|BaseVertex|InstancedBaseVertex|Array|)(ARB|EXT|AMD|ATI|APPLE|)' ],
        'prefix' : [ 'self->PreDraw( _context );', ],
    },
    'FfnModifyMatrix' : {
        'entries' : [
            'gl(Load|Mult)(Transpose|)(Matrix)(f|d)(ARB|)',
            'gl(LoadIdentity)()()',
            'gl(Rotate|Scale|Translate)()()(f|d)',
            'gl(Ortho|Frustum)()()',
            'gl(Push|Pop)(Matrix)()()',
            'gl(Matrix)(Load|Mult)(Transpose|)(f|d)EXT',
            'gl(Matrix)(LoadIdentity)()()EXT',
            'gl(Matrix)(Rotate|Scale|Translate)()(f|d)EXT',
            'gl(Matrix)(Ortho|Frustum)()()EXT',
            'gl(Matrix)(Push|Pop)()()EXT',
        ],
        'impl' : [ 'self->${m1}${m2}${m3}( ${arg0plus} ); return;' ],
    },
    'Viewport' : {
        'entries' : [ 'glViewport', ],
        'prefix' : [ 'self->Viewport( ${arg0plus} );', ],
    },
    'DepthRange' : {
        'entries' : [ 'glDepthRange', ],
        'prefix' : [ 'self->DepthRange( GLfloat(${arg0}), GLfloat(${arg1}) );', ],
    },
    'RasterPosition' : {
        'entries' : [ 'gl(Raster|Window)Pos(2|3)(i|s|f|d)', ],
        'impl' : [ 'self->${m1}Position(${arg0plus} ); return;', ],
    },
    'EnableArray' : {
        'entries' : [ 'gl(Enable|Disable)VertexAttribArray(ARB|)' ],
        'prefix' : [
            'self->RestoreVao( _context );',
            'self->${m1}Array(${arg0} );',
        ],
    },

    'ShaderSource' : {
        'entries' : [ 'glShaderSource(ARB|)?', ],
        'impl' : [ 'self->ShaderSource(${arg0plus} ); return;', ],
    },
    'LinkProgram' : {
        'entries' : [ 'glLinkProgram(ARB|)?', ],
        'impl' : [ 'self->LinkProgram(${arg0} ); return;', ],
    },
    'CreateShader' : {
        'entries' : [ 'glCreateShader(ObjectARB)?', ],
        'impl' : [ 'return self->CreateShader(${arg0} );', ],
    },
    'Uniform' : {
        'entries' : [ 'glUniform(1|2|3|4)(d|f|i|ui)(ARB|)', ],
        'impl' : [ 
          'if( self->currinst ) {',
          '  self->Uniform(${m1}, ${arg0}, 1, ${arg1plus} );',
          '  return;',
          '}',
        ],
    },
    'Uniformv' : {
        'entries' : [ 'glUniform(1|2|3|4)(d|f|i|ui)v(ARB|)', ],
        'impl' : [ 
          'if( self->currinst ) {',
          '  self->Uniform(${m1}, ${arg0plus} );',
          '  return;',
          '}',
        ],
    },
    'UniformMatrix' : {
        'entries' : [ 'glUniformMatrix(2|3|4)(d|f)v(ARB|)', ],
        'impl' : [ 
          'if( self->currinst ) {',
          '  self->UniformMatrix(${m1}, ${m1}, ${arg0plus} );',
          '  return;',
          '}',
        ],
    },
    'UniformMatrixNonSquare' : {
        'entries' : [ 'glUniformMatrix(2|3|4)x(2|3|4)(d|f)v(ARB|)', ],
        'impl' : [ 
          'if( self->currinst ) {',
          '  self->UniformMatrix(${m1}, ${m2}, ${arg0plus} );',
          '  return;',
          '}',
        ],
    },
}

