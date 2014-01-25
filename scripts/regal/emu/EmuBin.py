#!/usr/bin/python -B

binFormulae = {
    'ShaderBinary' : {
        'entries' : [ 'glShaderBinary(ARB|)' ],
        'impl' : [ 'self->ShaderBinary( ${arg0plus} ); return;', ],
    }
}
