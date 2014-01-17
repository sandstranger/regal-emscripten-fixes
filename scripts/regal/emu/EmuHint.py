#!/usr/bin/python -B

hintFormulae = {
  'Hint'       : {
    'entries' : [ 'glHint' ],
    'impl'    : [
      'if( ! _context->hint->glHint( *_context, ${arg0plus} ) ) {',
      '  orig.glHint( ${arg0plus} );',
      '}',
    ],
  },
  'Get'       : {
    'entries' : [ 'glGet(Integer|Float|Double|Boolean)v(EXT|)' ],
    'impl'    : [
      'if( ! _context->hint->glGet${m2}v( *_context, ${arg0plus} ) ) {',
      '  orig.glGet${m1}v${m2}( ${arg0plus} );',
      '}',
    ],
  },
}
