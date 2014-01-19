#!/usr/bin/python -B

hintFormulae = {
  'Hint'       : {
    'entries' : [ 'glHint' ],
    'impl'    : [
      'if( _context->hint->glHint( *_context, ${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'Get'       : {
    'entries' : [ 'glGet(Integer|Float|Double|Boolean)v(EXT|)' ],
    'impl'    : [
      'if( _context->hint->glGet${m2}v( *_context, ${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
}
