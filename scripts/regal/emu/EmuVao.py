#!/usr/bin/python -B

vaoFormulae = {
    # TODO - GL_ARB_base_instance ?
    'Validate' : {
        'entries' : [ 'gl(Multi|)Draw(Arrays|Element|Elements)(Instanced|Indirect|BaseVertex|InstancedBaseVertex|Array|)(ARB|EXT|AMD|ATI|APPLE|)' ],
        'prefix' : [  '// self->Validate();' ],
    },
    'BufferBinding' : {
        'entries' : [ 'glBindBuffer(ARB|)' ],
        'prefix' : [ 'self->ShadowBufferBinding( ${arg0}, ${arg1} );' ],
    },
    'BindVertexArray' : {
        'entries' : [ 'glBindVertexArray(ARB|)' ],
        'impl' : [ 'self->BindVertexArray(${arg0} ); return;' ],
    },
   'GenVertexArrays' : {
        'entries' : [ 'glGenVertexArrays(ARB|)' ],
        'impl' : [ 'self->GenVertexArrays( ${arg0}, ${arg1} ); return;' ],
    },
   'DeleteVertexArrays' : {
        'entries' : [ 'glDeleteVertexArrays(ARB|)' ],
        'impl' : [ 'self->DeleteVertexArrays( ${arg0}, ${arg1} ); return;' ],
    },
   'IsVertexArray' : {
        'entries' : [ 'glIsVertexArray(ARB|)' ],
        'impl' : [ 'return self->IsVertexArray( ${arg0} );' ],
    },
   'EnableVertexAttribArray' : {
        'entries' : [ 'gl(Enable|Disable)VertexAttribArray(ARB|)' ],
        'impl' : [ 'return self->${m1}VertexAttribArray(${arg0} );' ],
    },
    'EnableDisableClientState' : {
        'entries' : [ 'gl(Enable|Disable)ClientState' ],
        'impl' : [ 'self->${m1}ClientState(${arg0} ); return;' ],
    },
   'AttribPointer' : {
        'entries' : [ 'glVertexAttribPointer(ARB|)' ],
        'impl' : [ 'return self->AttribPointer(${arg0}, ${arg1plus} );' ],
    },
    'GetAttrib' : {
        'entries' : [ 'glGetVertexAttrib(d|f|i|Pointer)v(ARB|)' ],
        'impl' : [ 'self->GetAttrib( ${arg0}, ${arg1}, ${arg2} ); return;' ],
    },
#    'GetVertexAttribPointerv' : {
#        'entries' : [ 'glGetVertexAttribPointerv(ARB|)' ],
#        'impl' : [
#            'if ( !self->GetVertexAttribPointerv( ${arg0}, ${arg1plus} ) ) {',
#            '   orig.glGetVertexAttribPointerv${m1}( ${arg0}, ${arg1plus} );',
#            '}',
#        ]
#    },
    'Get' : {
        'entries' : [ 'glGet(Boolean|Double|Float|Integer|Integer64)v' ],
        'impl' : [
            'if( self->Get( ${arg0}, ${arg1} ) ) {',
            '  return;',
            '}',
        ]
    },
    'InterleavedArrays' : {
        'entries' : [ 'glInterleavedArrays' ],
        'impl' : [ 'self->InterleavedArrays(${arg0}, ${arg1plus} ); return;' ],
    },
    'Pointer4EXT' : {
        'entries' : [ 'gl(Color|TexCoord|Vertex)PointerEXT' ],
        'impl' : [ 'self->${m1}Pointer(${arg0}, ${arg1}, ${arg2}, ${arg4} ); return;' ],
    },
    'Pointer4' : {
        'entries' : [ 'gl(Color|SecondaryColor|TexCoord|Vertex)Pointer' ],
        'impl' : [ 'self->${m1}Pointer(${arg0}, ${arg1}, ${arg2}, ${arg3} ); return;' ],
    },
    'glSecondaryColorPointerEXT' : {
        'entries' : [ 'gl(SecondaryColor)PointerEXT' ],
        'impl' : [ 'self->${m1}Pointer(${arg0}, ${arg1}, ${arg2}, ${arg3} ); return;' ],
    },
    'NormalPointer3EXT' : {
        'entries' : [ 'glNormalPointerEXT' ],
        'impl' : [ 'self->NormalPointer(${arg0}, ${arg1}, ${arg3} ); return;' ],
    },
    'FogCoordPointer3EXT' : {
        'entries' : [ 'glFogCoordPointerEXT' ],
        'impl' : [ 'self->FogCoordPointer(${arg0}, ${arg1}, ${arg2} ); return;' ],
    },
    'Pointer3' : {
        'entries' : [ 'gl(FogCoord|Normal)Pointer' ],
        'impl' : [ 'self->${m1}Pointer(${arg0}, ${arg1}, ${arg2} ); return;' ],
    },
    'ClientActiveTexture' : {
        'entries' : [ 'glClientActiveTexture(ARB|)' ],
        'prefix' : [ 'self->ClientActiveTexture(${arg0} );' ],
    },
}
