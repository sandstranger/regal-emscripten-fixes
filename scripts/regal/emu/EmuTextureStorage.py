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
        'impl' : [ '_context->texsto->TextureStorage( _context, ${arg0plus} ); return;', ],
    },
    'GetTexParameterv' : {
        'entries' : [ 'glGetTexParameter(I|)(u|)(f|i)v' ],
        'impl' : [
            'if ( _context->texsto->GetTexParameterv( _context, ${arg0plus} ) ) {',
            '   return;',
            '}',
        ]
    },
    'DeleteTextures' : {
        'entries' : [ 'glDeleteTextures' ],
        'prefix' : [ '_context->texsto->DeleteTextures( _context, ${arg0plus} );' ],
    },

    #'TexImage' : { # disallow these if the object was specified with TextureStorage
    #    'entries' : [ 'gl(Copy|)TexImage(1|2|3)D(ARB|)' ],
    #    'impl' : [ '_context->texsto->${m1}TexImage( _context, ${arg0plus} );', ],
    #}
}
