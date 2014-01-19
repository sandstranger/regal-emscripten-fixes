#!/usr/bin/python -B

rectFormulae = {
  'options' : {
    'originate' : [
      'glBegin',
      'glEnd',
      'glVertex2d',
      'glVertex2f',
      'glVertex2i',
      'glVertex2s',
    ],
  },
  'rect'       : {
    'entries' : [ 'glRect(d|f|i|s)(v|)' ],
    'impl'    : [ '_context->rect->glRect${m2}( _context, ${arg0plus} ); return;', ],
  },
}
