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
    'impl'    : [ 'self->glRect${m2}( ${arg0plus} ); return;', ],
  },
}
