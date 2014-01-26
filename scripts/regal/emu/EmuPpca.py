#!/usr/bin/python -B

ppcaFormulae = {

  # Push/Pop ClientAttrib & Get

  'PushClientAttrib' : {
    'entries'  : [ 'glPushClientAttrib' ],
    'impl'     : [ 'self->glPushClientAttrib( ${arg0} ); return;', ],
  },

  'PopClientAttrib' : {
    'entries' : [ 'glPopClientAttrib' ],
    'impl'    : [ 'self->glPopClientAttrib(); return;', ],
  },

  'PushClientAttribDefaultEXT' : {
    'entries'  : [ 'gl(Push|)ClientAttribDefaultEXT' ],
    'impl'     : [ 'self->gl${m1}ClientAttribDefaultEXT( ${arg0} ); return;', ],
  },

  'Get'       : {
    'entries' : [ 'glGet(Integer|Float|Double|Boolean)v(EXT|)' ],
    'impl'    : [
      'if ( self->glGetv( ${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  # shadow all the client state

  'Bind'      : {
    'entries' : [ 'glBind(Vertex|)(Buffer|Array)(s|)' ],
    'prefix'  : [ 'self->glBind${m1}${m2}${m3}( ${arg0plus} );', ],
  },
  'Delete'    : {
    'entries' : [ 'glDelete(VertexArrays|Buffers)' ],
    'prefix'  : [ 'self->glDelete${m1}( ${arg0plus} );', ],
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
  'PixelStore' : {
      'entries' : [ 'glPixelStore(i|f)' ],
      'prefix'  : [ 'self->glPixelStore( ${arg0plus} );', ],
  },
}
