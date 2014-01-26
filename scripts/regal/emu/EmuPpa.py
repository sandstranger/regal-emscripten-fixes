#!/usr/bin/python -B

from RegalStateProcs import stateProcs

ppaFormulae = {
  'options' : {
    'originate' : stateProcs,
  },
  'PushAtrrib' : {
    'entries'  : [ 'glPushAttrib' ],
    'impl'     : [ 'self->PushAttrib(${arg0} ); return;', ],
  },
  'PopAttrib' : {
    'entries' : [ 'glPopAttrib' ],
    'impl'    : [ 'self->PopAttrib(); return;', ],
  },
  'Get'       : {
    'entries' : [ 'glGet(Integer|Float|Double|Boolean)(i_|)v(EXT|)' ],
    'impl'    : [
      'if( self->glGet${m2}v(${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'GetPolygonStipple'       : {
    'entries' : [ 'glGetPolygonStipple' ],
    'impl'    : [
      'if( self->glGetPolygonStipple(${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'glGetColorTableOrConvolutionParameterv'       : {
    'entries' : [ 'glGet(ColorTable|Convolution)Parameter(i|f)v' ],
    'impl'    : [
      'if( self->glGet${m1}Parameterv(${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'GetLightOrMaterialv'       : {
    'entries' : [ 'glGet(Light|Material)(f|i|x)v' ],
    'impl'    : [
      'if( self->glGet${m1}v(${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'GetTexEnviv'       : {
    'entries' : [ 'glGet(Multi|)TexEnv(f|i)v(EXT|)' ],
    'impl'    : [
      'if( self->glGet${m1}TexEnvv(${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'GetTexParam' : {
    'entries' : [ 'glGet(Tex|Texture)(Level|)Parameter(f|i)v(EXT|)' ],
    'impl'    : [
      'if( self->glGet${m1}${m2}Parameter(${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'IsEnabled'       : {
    'entries' : [ 'glIsEnabled(i|)' ],
    'impl'    : [
      'GLboolean enabled;',
      'if( self->glIsEnabled${m1}(enabled, ${arg0plus} ) ) {',
      '  return enabled;',
      '}',
    ],
  },
  'Enable'    : {
    'entries' : [ 'gl(Enable|Disable)(i|)' ],
    'impl'    : [
      'if( self->${m1}${m2}(${arg0plus} ) ) {',
      '  return;',
      '}',
    ],
  },
  'TrackDepth' : {
    'entries'    : [ '(glClearDepth|glDepthFunc|glDepthMask)(f|)' ],
    'prefix'     : [ 'self->${m1}( ${arg0plus} );', ],
  },
  'TrackStencil' : {
    'entries'    : [ '(glClearStencil|glStencilFunc|glStencilFuncSeparate|glStencilMask|glStencilMaskSeparate|glStencilOp|glStencilOpSeparate)' ],
    'prefix'     : [ 'self->${m1}( ${arg0plus} );', ],
  },
  'TrackPolygon' : {
    'entries'    : [ '(glCullFace|glFrontFace|glPolygonMode|glPolygonOffset)' ],
    'prefix'     : [ 'self->${m1}( ${arg0plus} );', ],
  },
  'TrackDrawBuffer' : {
    'entries'    : [ 'glDrawBuffer(s|)(ARB|EXT|NV|)' ],
    'prefix'     : [ '  self->glDrawBuffer${m1}( ${arg0plus} );', ],
  },
  'TrackMatrixMode' : {
    'entries'    : [ 'glMatrixMode' ],
    'prefix'     : [ 'self->glMatrixMode( ${arg0plus} );', ],
  },
  'TrackClipPlane' : {
    'entries'    : [ 'glClipPlane' ],
    'prefix'     : [ 'self->glClipPlane( ${arg0plus} );', ],
  },
  'TrackHint' : {
    'entries'    : [ 'glHint' ],
    'prefix'     : [ 'self->glHint( ${arg0plus} );', ],
  },
  'TrackClampColor': {
    'entries'    : [ 'glClampColor' ],
    'prefix'     : [ 'self->glClampColor( ${arg0plus} );',
    ],
  },
  'TrackActiveTexture' : {
    'entries'    : [ 'glActiveTexture(ARB|)' ],
    'prefix'     : [ 'self->glActiveTexture( ${arg0plus} );', ],
  },
  'TrackListBase' : {
    'entries'    : [ 'glListBase' ],
    'prefix'     : [ 'self->glListBase( ${arg0plus} );', ],
  },
  'TrackClearAccum' : {
    'entries'    : [ 'glClearAccum' ],
    'prefix'     : [ 'self->glClearAccum( ${arg0plus} );', ],
  },
  'TrackScissor' : {
    'entries'    : [ 'glScissor(Array|Indexed|)(v|)' ],
    'prefix'     : [ 'self->glScissor${m1}${m2}( ${arg0plus} );', ],
  },
  'TrackViewport' : {
    'entries'    : [ 'glViewport(Array|Indexedf|)(v|)' ],
    'prefix'     : [ 'self->glViewport${m1}${m2}( ${arg0plus} );', ],
  },
  'TrackDepthRange' : {
    'entries'    : [ 'glDepthRange(Array|Indexed|)(f|)(v|)' ],
    'prefix'     : [ 'self->glDepthRange${m1}${m3}( ${arg0plus} );', ],
  },
  'TrackLine' : {
    'entries'    : [ 'glLine(Width|Stipple|)' ],
    'prefix'     : [ 'self->glLine${m1}( ${arg0plus} );', ],
  },
  'TrackSampleCoverage' : {
    'entries'    : [ 'glSampleCoverage' ],
    'prefix'     : [ 'self->glSampleCoverage( ${arg0plus} );', ],
  },
  'TrackMinSampleShading' : {
    'entries'    : [ 'glMinSampleShading' ],
    'prefix'     : [ 'self->glMinSampleShading( ${arg0plus} );', ],
  },
  'TrackMapGrid' : {
    'entries'    : [ 'glMapGrid(1|2)(f|d)' ],
    'prefix'     : [ 'self->glMapGrid${m1}( ${arg0plus} );', ],
  },
  'TrackPointSize' : {
    'entries'    : [ 'glPointSize' ],
    'prefix'     : [ 'self->glPointSize( ${arg0plus} );', ],
  },
  'TrackPointParameter' : {
    'entries'    : [ 'glPointParameter(i|f)(v|)' ],
    'prefix'     : [ 'self->glPointParameter${m2}( ${arg0plus} );', ],
  },
  'TrackTexEnv' : {
    'entries'    : [ 'gl(Multi|)TexEnv(i|f)(v|)(EXT|)' ],
    'prefix'     : [ 'self->gl${m1}TexEnv${m3}( ${arg0plus} );', ],
  },
  'TrackPolygonStipple' : {
    'entries'    : [ 'glPolygonStipple' ],
    'prefix'     : [ 'self->glPolygonStipple( ${arg0plus} );', ],
  },
  'TrackAlphaFunc' : {
    'entries'    : [ 'glAlphaFunc' ],
    'prefix'     : [ 'self->glAlphaFunc( ${arg0plus} );', ],
  },
  'TrackBlend' : {
    'entries'    : [ 'glBlend(Func|Equation|Color)(Separate|)(i|)' ],
    'prefix'     : [ 'self->glBlend${m1}${m2}${m3}( ${arg0plus} );', ],
  },
  'TrackLogicOp' : {
    'entries'    : [ 'glLogicOp' ],
    'prefix'     : [ 'self->glLogicOp( ${arg0plus} );', ],
  },
  'TrackMask' : {
    'entries'    : [ 'gl(Index|Color)Mask(i|)' ],
    'prefix'     : [ 'self->gl${m1}Mask${m2}( ${arg0plus} );', ],
  },
  'TrackClear' : {
    'entries'    : [ 'glClear(Index|Color)' ],
    'prefix'     : [ 'self->glClear${m1}( ${arg0plus} );', ],
  },
  'TrackPixelZoom' : {
    'entries'    : [ 'glPixelZoom' ],
    'prefix'     : [ 'self->glPixelZoom( ${arg0plus} );', ],
  },
  'TrackColorTableParameterv' : {
    'entries'    : [ 'glColorTableParameter(i|f)v' ],
    'prefix'     : [ 'self->glColorTableParameterv( ${arg0plus} );', ],
  },
  'TrackConvolutionParameter' : {
    'entries'    : [ 'glConvolutionParameter(i|f)(v|)' ],
    'prefix'     : [ 'self->glConvolutionParameter${m2}( ${arg0plus} );', ],
  },
  'TrackPixelTransfer' : {
    'entries'    : [ 'glPixelTransfer(i|f)' ],
    'prefix'     : [ 'self->glPixelTransfer( ${arg0plus} );', ],
  },
  'TrackReadBuffer' : {
    'entries'    : [ 'glReadBuffer' ],
    'prefix'     : [ 'self->glReadBuffer( ${arg0plus} );', ],
  },
  'TrackShadeModel' : {
    'entries'    : [ 'glShadeModel' ],
    'prefix'     : [ 'self->glShadeModel( ${arg0plus} );', ],
  },
  'TrackProvokingVertex' : {
    'entries'    : [ 'glProvokingVertex' ],
    'prefix'     : [ 'self->glProvokingVertex( ${arg0plus} );', ],
  },
  'TrackColorMaterial' : {
    'entries'    : [ 'glColorMaterial' ],
    'prefix'     : [ 'self->glColorMaterial( ${arg0plus} );', ],
  },
  'TrackMaterialLight' : {
    'entries'    : [ 'gl(Material|Light)(Model|)(i|f)(v|)' ],
    'prefix'     : [ 'self->gl${m1}${m2}${m4}( ${arg0plus} );', ],
  },
}
