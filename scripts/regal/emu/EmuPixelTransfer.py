#!/usr/bin/python -B

xferFormulae = {
    'options' : {
      'originate' : [
        'glCompressedTexImage2D',
        'glCompressedTexSubImage2D',
        'glPixelStorei',
        'glTexImage2D',
        'glTexSubImage2D',
      ],
    },
    'PixelStore' : {
        'entries' : [ 'glPixelStore(i|f)' ],
        'prefix' : [ 'self->PixelStore(${arg0plus} );', ],
    },
   'ActiveTexture' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'prefix' : [ 'self->ShadowActiveTexture( ${arg0} );', ],
    },
    'TexImage2D' : {
        'entries' : [ 'gl(Compressed|)Tex(Sub|)Image2D(ARB|EXT|)' ],
        'impl' : [ 'self->${m1}Tex${m2}Image2D(${arg0plus} ); return;', ],
    }
}
