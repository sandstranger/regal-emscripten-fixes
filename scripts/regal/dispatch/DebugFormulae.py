#!/usr/bin/python -B

debugDispatchFormulae = {
    'MatrixMode' : {
        'entries' : [ 'glMatrixMode' ],
        'prefix' : [ 'self->MatrixMode( ${arg0plus} );', ],
    },
    'ClientActiveTexture' : {
        'entries' : [ 'gl(Client|)(ActiveTexture)(ARB|)' ],
        'prefix' : [ 'self->${m1}${m2}( ${arg0plus} );', ],
    },
}

