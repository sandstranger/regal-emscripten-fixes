#!/usr/bin/python -B

baseVertexFormulae = {
  'options' : {
    'originate' : [
      'glDrawElements',
      'glDrawRangeElements',
      'glDrawElementsInstanced',
      'glDrawElementsInstancedBaseInstance',
    ],
  },
  'DrawElements'       : {
    'entries' : [ 'gl(Multi|)Draw(Range|)Elements(Instanced|)BaseVertex(BaseInstance|)' ],
    'impl'    : [
      'if( self->gl${m1}Draw${m2}Elements${m3}BaseVertex${m4}( ${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'Bind'      : {
    'entries' : [ 'glBind(Vertex|)(Buffer|Array)' ],
    'prefix'  : [ 'self->glBind${m1}${m2}( ${arg0plus} );', ],
  },
  'EnableDisable' : {
    'entries' : [ 'gl(Enable|Disable)(ClientState|VertexAttribArray|)(i|Indexed|)(EXT|)' ],
    'prefix'  : [ 'self->gl${m1}${m2}${m3}${m4}( ${arg0plus} );', ],
  },
  'Pointer'   : {
    'entries' : [ 'gl(Color|EdgeFlag|FogCoord|Index|Normal|SecondaryColor|TexCoord|Vertex)Pointer' ],
    'prefix'  : [ 'self->gl${m1}Pointer( ${arg0plus} );', ],
  },
  'VertexAttrib' : {
    'entries' : [ 'glVertexAttrib(I|L|)(Binding|Format)' ],
    'prefix'  : [ 'self->glVertexAttrib${m1}${m2}( ${arg0plus} );', ],
  },
  'MultiTexCoordPointerEXT': {
    'entries' : [ 'glMultiTexCoordPointerEXT' ],
    'prefix'  : [ 'self->glMultiTexCoordPointerEXT( ${arg0plus} );', ],
  },
  'ClientActiveTexture' : {
    'entries' : [ 'glClientActiveTexture(ARB|)' ],
    'prefix'  : [ 'self->glClientActiveTexture( ${arg0plus} );', ],
  },
  'PrimitiveRestartIndex' : {
    'entries' : [ 'glPrimitiveRestartIndex' ],
    'prefix'  : [ 'self->glPrimitiveRestartIndex( ${arg0plus} );', ],
  },
  'VertexDivisor' : {
    'entries' : [ 'glVertex(Attrib|Binding)Divisor' ],
    'prefix'  : [ 'self->glVertex${m1}Divisor( ${arg0plus} );', ],
  },
  'VertexAttribPointer' : {
    'entries' : [ 'glVertexAttrib(I|L|)Pointer' ],
    'prefix'  : [ 'self->glVertexAttrib${m1}Pointer( ${arg0plus} );', ],
  },
  'VertexArrayEnable' : {
      'entries' : [ 'gl(Enable|Disable)VertexArrayEXT' ],
      'prefix'  : [ 'self->gl${m1}VertexArrayEXT( ${arg0plus} );', ],
  },
  'VertexArrayAttribEnable' : {
      'entries' : [ 'gl(Enable|Disable)VertexArrayAttribEXT' ],
      'prefix'  : [ 'self->gl${m1}VertexArrayAttribEXT( ${arg0plus} );', ],
  },
  'DsaVertexArrayOffsetCommands' : {
      'entries' : [ 'glVertexArray(EdgeFlag|Index|Normal|FogCoord|Vertex|Color|TexCoord|SecondaryColor|VertexAttribI|MultiTexCoord|VertexAttrib)OffsetEXT' ],
      'prefix'  : [ 'self->glVertexArray${m1}OffsetEXT( ${arg0plus} );', ],
  },
  'InterleavedArrays ' : {
      'entries' : [ 'glInterleavedArrays' ],
      'prefix'  : [ 'self->glInterleavedArrays( ${arg0plus} );', ],
  },
}
