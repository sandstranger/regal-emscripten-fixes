#!/usr/bin/python -B

quadsFormulae = {
  'options' : {
    'originate' : [
      'glBindBuffer',
      'glBufferData',
      'glDrawArrays',
      'glDrawElements',
      'glGenBuffers',
      'glPolygonMode',
    ],
  },
  'EnableDisable' : {
    'entries' : [ 'gl(En|Dis)able' ],
    'prefix'  : [ 'self->gl${m1}able( ${arg0plus} );', ],
  },
  'CullFrontFace' : {
    'entries'    : [ 'gl(Cull|Front)Face' ],
    'prefix'     : [ 'self->gl${m1}Face( ${arg0plus} );', ],
  },
  'PolygonMode' : {
    'entries'    : [ 'glPolygonMode' ],
    'prefix'     : [ 'self->glPolygonMode( ${arg0plus} );', ],
  },
  'ShadeModel' : {
    'entries'    : [ 'glShadeModel' ],
    'prefix'     : [ 'self->glShadeModel( ${arg0plus} );', ],
  },
  'ProvokingVertex' : {
    'entries'    : [ 'glProvokingVertex(EXT|)' ],
    'prefix'     : [ 'self->glProvokingVertex( ${arg0plus} );', ],
  },
  'BindBuffer' : {
    'entries'    : [ 'glBindBuffer(EXT|)' ],
    'prefix'     : [ 'self->glBindBuffer( ${arg0plus} );', ],
  },
  'quads'       : {
    'entries' : [ 'glDrawArrays(EXT|)' ],
    'impl'    : [
'''
if ( self->glDrawArrays( ${arg0plus} ) ) {
  return;
}
'''
    ],
  },
}
