#!/usr/bin/python -B

soFormulae = {

    # TODO

    'GenSamplers' : {
        'entries' : [ 'glGenSamplers' ],
        'impl' : [ '_context->so->GenSamplers( ${arg0plus} ); return;' ],
    },
    'DeleteSamplers' : {
        'entries' : [ 'glDeleteSamplers' ],
        'impl' : [ '_context->so->DeleteSamplers( ${arg0plus} ); return;' ],
    },
    'IsSampler' : {
        'entries' : [ 'glIsSampler' ],
        'impl' : [ 'return _context->so->IsSampler( ${arg0} );' ],
    },
    'BindSampler' : {
        'entries' : [ 'glBindSampler' ],
        'impl' : [ '_context->so->BindSampler( ${arg0plus} ); return;' ],
    },
    'GetSamplerParameterv' : {
        'entries' : [ 'glGetSamplerParameter(I|)(u|)(f|i)v' ],
        'impl' : [
            'if ( _context->so->GetSamplerParameterv( *_context, ${arg0plus} )) {',
            '   return;',
            '}',
        ]
    },
    'SamplerParameter' : {
        'entries' : [ 'glSamplerParameter(I|)(u|)(f|i)(v|)' ],
        'impl' : [
            'if ( _context->so->SamplerParameter${m4}( *_context, ${arg0plus} )) {',
            '   return;',
            '}',
        ]
    },
    'ActiveTexture' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'impl' : [
            'if ( _context->so->ActiveTexture( *_context, ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'GenTextures' : {
        'entries' : [ 'glGenTextures' ],
        'impl' : [ '_context->so->GenTextures( *_context, ${arg0plus} ); return;' ],
    },
    'DeleteTextures' : {
        'entries' : [ 'glDeleteTextures' ],
        'prefix' : [ '_context->so->DeleteTextures( *_context, ${arg0plus} );' ],
    },
    'BindTexture' : {
        'entries' : [ 'glBindTexture' ],
        'impl' : [
          'if ( _context->so->BindTexture( *_context, ${arg0plus} ) ) {',
            '   return;',
          '}',
      ]
    },
    'TexParameter' : {
        'entries' : [ 'glTexParameter(I|)(u|)(f|i)(v|)(EXT|)' ],
        'impl' : [
            'if ( _context->so->TexParameter${m4}( *_context, ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'GetTexParameterv' : {
        'entries' : [ 'glGetTexParameter(I|)(u|)(f|i)v' ],
        'impl' : [
            'if ( _context->so->GetTexParameterv( *_context, ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'Get' : {
        'entries' : [ 'glGet(Double|Float|Integer|Integer64)v' ],
        'impl' : [
            'if ( _context->so->Get( ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'PreDraw' : {
        'entries' : [ 'gl(Multi|)Draw(Range|)(Arrays|Element|Elements)(Instanced|Indirect|BaseVertex|InstancedBaseVertex|Array|)(ARB|EXT|AMD|ATI|APPLE|)' ],
        'prefix' : [
          '_context->so->PreDraw( *_context );',
        ],
    },
}
