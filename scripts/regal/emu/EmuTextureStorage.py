#!/usr/bin/python -B

texstoFormulae = {
    'options' : {
      'originate' : [
        'glGetIntegerv',
        'glTexImage1D',
        'glTexImage2D',
        'glTexImage3D',
      ],
    },
    'TextureStorage' : {
        'entries' : [ 'glTexStorage(1|2|3)D(EXT|)' ],
        'impl' : [ 'self->TextureStorage(${arg0plus} ); return;', ],
    },
    'GetTexParameterv' : {
        'entries' : [ 'glGetTexParameter(I|)(u|)(f|i)v' ],
        'impl' : [
            'if ( self->GetTexParameterv(${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'DeleteTextures' : {
        'entries' : [ 'glDeleteTextures' ],
        'prefix' : [ 'self->DeleteTextures(${arg0plus} );' ],
    },

    #'TexImage' : { # disallow these if the object was specified with TextureStorage
    #    'entries' : [ 'gl(Copy|)TexImage(1|2|3)D(ARB|)' ],
    #    'impl' : [ 'self->${m1}TexImage(${arg0plus} );', ],
    #}
}
