#!/usr/bin/python -B

hintFormulae = {
  'Hint'       : {
    'entries' : [ 'glHint' ],
    'impl'    : [
      'if( self->glHint( ${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'Get'       : {
    'entries' : [ 'glGet(Integer|Float|Double|Boolean)v(EXT|)' ],
    'impl'    : [
      'if( self->glGet${m2}v( ${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
}
