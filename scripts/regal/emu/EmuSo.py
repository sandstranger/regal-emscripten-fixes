#!/usr/bin/python -B

soFormulae = {

    # TODO

    'GenSamplers' : {
        'entries' : [ 'glGenSamplers' ],
        'impl' : [ 'self->GenSamplers( ${arg0plus} ); return;' ],
    },
    'DeleteSamplers' : {
        'entries' : [ 'glDeleteSamplers' ],
        'impl' : [ 'self->DeleteSamplers( ${arg0plus} ); return;' ],
    },
    'IsSampler' : {
        'entries' : [ 'glIsSampler' ],
        'impl' : [ 'return self->IsSampler( ${arg0} );' ],
    },
    'BindSampler' : {
        'entries' : [ 'glBindSampler' ],
        'impl' : [ 'self->BindSampler( ${arg0plus} ); return;' ],
    },
    'GetSamplerParameterv' : {
        'entries' : [ 'glGetSamplerParameter(I|)(u|)(f|i)v' ],
        'impl' : [
            'if ( self->GetSamplerParameterv( ${arg0plus} )) {',
            '   return;',
            '}',
        ]
    },
    'SamplerParameter' : {
        'entries' : [ 'glSamplerParameter(I|)(u|)(f|i)(v|)' ],
        'impl' : [
            'if ( self->SamplerParameter${m4}( ${arg0plus} )) {',
            '   return;',
            '}',
        ]
    },
    'ActiveTexture' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'impl' : [
            'if ( self->ActiveTexture( ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'GenTextures' : {
        'entries' : [ 'glGenTextures' ],
        'impl' : [ 'self->GenTextures( ${arg0plus} ); return;' ],
    },
    'DeleteTextures' : {
        'entries' : [ 'glDeleteTextures' ],
        'prefix' : [ 'self->DeleteTextures( ${arg0plus} );' ],
    },
    'BindTexture' : {
        'entries' : [ 'glBindTexture' ],
        'impl' : [
          'if ( self->BindTexture( ${arg0plus} ) ) {',
            '   return;',
          '}',
      ]
    },
    'TexParameter' : {
        'entries' : [ 'glTexParameter(I|)(u|)(f|i)(v|)(EXT|)' ],
        'impl' : [
            'if ( self->TexParameter${m4}( ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'GetTexParameterv' : {
        'entries' : [ 'glGetTexParameter(I|)(u|)(f|i)v' ],
        'impl' : [
            'if ( self->GetTexParameterv( ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'Get' : {
        'entries' : [ 'glGet(Double|Float|Integer|Integer64)v' ],
        'impl' : [
            'if ( self->Get( ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'PreDraw' : {
        'entries' : [ 'gl(Multi|)Draw(Range|)(Arrays|Element|Elements)(Instanced|Indirect|BaseVertex|InstancedBaseVertex|Array|)(ARB|EXT|AMD|ATI|APPLE|)' ],
        'prefix' : [
          'self->PreDraw();',
        ],
    },
}
