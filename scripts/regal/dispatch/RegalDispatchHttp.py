#!/usr/bin/python -B

from string import Template, upper, replace

from ApiCodeGen   import *
from ApiUtil      import outputCode
from ApiUtil      import typeIsVoid
from ApiUtil      import typeIsVoidPointer
from ApiRegal     import logFunction

from Dispatch     import dispatchGenCode

from RegalContextInfo import cond

from RegalDispatchShared import apiDispatchFuncInitCode
from RegalDispatchShared import apiDispatchGlobalFuncInitCode
from RegalDispatchShared import apiDispatchLayerConstructor

formulae = {
  'bindtexture' : {
    'entries' : [ 'glBindTexture(EXT|)' ],
    'pre' : [
      'if( ${arg1} != 0 ) {',
      '  HttpTextureInfo & hti = self->texture[ ${arg1} ];',
      '  RegalAssert( hti.name == 0 || hti.name == ${arg1} );',
      '  if( hti.name == 0 ) {',
      '    hti.name = ${arg1};',
      '    hti.target = ${arg0};',
      '  }',
      '}',
    ],
  },
  'bindmultitexture' : {
    'entries' : [ 'glBindMultiTextureEXT' ],
    'pre' : [
      'if( ${arg2} != 0 ) {',
      '  HttpTextureInfo & hti = self->texture[ ${arg2} ];',
      '  RegalAssert( hti.name == 0 || hti.name == ${arg2} );',
      '  if( hti.name == 0 ) {',
      '    hti.name = ${arg2};',
      '    hti.target = ${arg1};',
      '  }',
      '}',
    ],
  },
  'bindtextures' : {
    'entries' : [ 'glBindTextures' ],
    'pre' : [
      'if( ${arg2} != NULL ) {',
      '  for( int i = 0; i < ${arg1}; i++ ) {',
      '    if( ${arg2}[i] != 0 ) {',
      '      HttpTextureInfo & hti = self->texture[ ${arg2}[i] ];',
      '      RegalAssert( hti.name == 0 || hti.name == ${arg2}[i] );',
      '      if( hti.name == 0 ) {',
      '        hti.name = ${arg2}[i];',
      '        hti.target = ${arg0};',
      '      }',
      '    }',
      '  }',
      '}',
    ],
  },

  'deletetextures' : {
    'entries' : [ 'glDeleteTextures(EXT|)' ],
    'pre' : [
      'for( int i = 0; i < ${arg0}; i++ ) {',
      '  self->texture.erase( ${arg1}[i] );',
      '}',
    ],
  },

  'createshader' : {
    'entries' : [ 'glCreateShader(ObjectARB|)' ],
    'post' : [
      'self->shader.insert( ret );',
    ],
  },

  'deleteshader' : {
    'entries' : [ 'glDeleteShader(ObjectARB|)' ],
    'post' : [
      'self->shader.erase( ${arg0} );',
    ],
  },

  'createprogram' : {
    'entries' : [ 'glCreateProgram(ObjectARB|)' ],
    'post' : [
      'self->program.insert( ret );',
    ],
  },

  'deleteprogram' : {
    'entries' : [ 'glDeleteProgram(ObjectARB|)' ],
    'post' : [
      'self->program.erase( ${arg0} );',
    ],
  },


  'bindfbo' : {
    'entries' : [ 'glBindFramebuffer(ARB|)' ],
    'pre' : [
      'self->fbo[ ${arg1} ] = HttpFboInfo( ${arg1} );',
      'self->count.fbo++;',
      'if( self->runState == RS_NextFbo ) {',
      '  self->runState = RS_Pause;',
      '}',
    ],
    'post' : [
      'self->count.lastFbo = self->count.call;',
    ],
  },

  'deletefbos' : {
    'entries' : [ 'glDeleteFramebufferss(EXT|)' ],
    'pre' : [
      'for( int i = 0; i < ${arg0}; i++ ) {',
      '  self->fbo.erase( ${arg1}[i] );',
      '}',
    ],
  },

  'pushdebuggroup' : {
    'entries' : [ 'glPushDebugGroup', 'glPushGroupMarkerEXT' ],
    'pre' : [
      'self->count.group++;',
      'if( self->runState == RS_NextGroup ) {',
      '  self->runState = RS_Pause;',
      '}',
      'self->debugGroupStackDepth++;',
    ],
    'post' : [
      'self->count.lastGroup = self->count.call;',
    ],
  },

  'popdebuggroup' : {
    'entries' : [ 'glPopDebugGroup', 'glPopGroupMarkerEXT' ],
    'post' : [
      'if( self->runState == RS_StepOutOfGroup || self->runState == RS_NextGroup ) {',
      '  self->runState = RS_Pause;',
      '}',
      'if( self->runState == RS_StepOverGroup && self->debugGroupStackDepth == self->stepOverGroupDepth ) {',
      '  self->runState = RS_Pause;',
      '}',
      'self->debugGroupStackDepth--;',
    ],
  },

  'draw' : {
    'entries' : [ 'gl(Multi|)Draw(Arrays|Elements).*', ],
    'pre' : [
      'self->count.draw++;',
      'if( self->runState == RS_NextDraw ) {',
      '  self->runState = RS_Pause;',
      '}',
    ],
    'post' : [
      'self->count.lastDraw = self->count.call;',
    ],
  },

  'begin' : {
    'entries' : [ 'glBegin' ],
    'pre' : [
      'self->count.draw++;',
      'self->inBeginEnd++;',
      'if( self->runState == RS_NextDraw ) {',
      '  self->runState = RS_Pause;',
      '}',
    ],
    'post' : [
      'self->count.lastDraw = self->count.call;',
    ],
  },

  'end' : {
    'entries' : [ 'glEnd' ],
    'post' : [
      'self->inBeginEnd--;',
      'self->YieldToHttpServer( false /* no log update */ );'
    ],
  },

  'swap' : {
    'entries' : [ '(glX|wgl|egl|)SwapBuffers', 'CGLFlushDrawable' ],
    'pre' : [
      'self->count.frame++;',
      'switch( self->runState ) {',
      '   case RS_Run:',
      '     break;',
      '   default:',
      '    self->runState = RS_Pause;',
      '}',
    ],
    'post' : [
      'self->count.lastFrame = self->count.call;',
    ],
  },

}

# CodeGen for dispatch table init.

dispatchHttpTemplate = Template('''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if ! REGAL_NO_HTTP

REGAL_GLOBAL_BEGIN

#include "RegalLog.h"
#include "Http.h"
#include "RegalPush.h"
#include "RegalToken.h"
#include "RegalHelper.h"
#include "RegalContext.h"
#include "RegalDispatch.h"

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

${CONSTRUCTOR}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

static Dispatch::Global nextGlobal;

void InitDispatchHttp( Layer * layer, Dispatch::GL & );
void InitDispatchHttp( Layer * layer, Dispatch::Global & );

bool Http::Initialize( const std::string & instanceInfo ) {
  ResetInterception();
  return true;
}

void Http::ResetInterception() {
  RegalContext * ctx = GetContext();
  next = ctx->dispatchGL;
  InitDispatchHttp( this, ctx->dispatchGL );

  gl.Initialize( &next );

  nextGlobal = dispatchGlobal;
  InitDispatchHttp( this, dispatchGlobal );
}

${API_FUNC_DEFINE}

void InitDispatchHttp( Layer *layer, Dispatch::GL &tbl)
{
${API_GL_DISPATCH_INIT}
}

${API_GLOBAL_DISPATCH_INIT}

REGAL_NAMESPACE_END

#endif
''')


def generateDispatchHttp(apis, args):

  # CodeGen for API functions.

  code = ''
  categoryPrev = None

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:

      if getattr(function,'regalOnly',False)==True:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True, "Layer *_layer")
      callParams = ''
      if function.needsContext:
        callParams = paramsNameCode(function.parameters, "self->next")
      else:
        callParams = paramsNameCode(function.parameters, "dispatchGlobal")

      rType  = typeCode(function.ret.type)
      rTypes    = rType.strip()
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '// %s\n\n' % category

      categoryPrev = category

      code += 'static %sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'http_', name, params)
      code += '  Http * self = static_cast<Http *>(_layer);\n'

      generated = dispatchGenCode( function, formulae )

      retInit = ''
      if not typeIsVoid(rType):
        if rTypes in api.defaults:
          retInit += '%s' % ( api.defaults[rTypes] )
        else:
          if rType[-1]=='*' or typeIsVoidPointer(rType):
            retInit += 'NULL'
          else:
            retInit += '(%s) 0' % ( rTypes )

      if not typeIsVoid(rType):
        code += '    %s ret = %s;\n' % (rType, retInit)

      if function.name == "glBegin" or function.name == "glEnd":
        code += '  self->callString = %s\n' % logFunction( function, 'print_string' )
      else:
        code += '  if( self->inBeginEnd == 0 ) {\n'
        code += '    self->callString = %s\n' % logFunction( function, 'print_string' )
        code += '  }\n'

      if generated and 'pre' in generated:
        for i in generated['pre']:
          code += '  %s\n' % i

      code +=   '  if( self->runState == RS_Next ) {\n'
      code +=   '    self->runState = RS_Pause;\n'
      code +=   '  }\n'
      if function.name == "glBegin" or function.name == "glEnd":
        code += '  self->YieldToHttpServer();\n'
      else:
        code += '  self->YieldToHttpServer( self->inBeginEnd == 0 );\n'

      

      code += '  '

      if not typeIsVoid(rType):
        code += 'ret = '
      code += 'R%s(%s);\n' % ( name, callParams )

      
      if generated and 'post' in generated:
        for i in generated['post']:
          code += '  %s\n' % i

      if not typeIsVoid(rType):
        code += '  return ret;\n'
      code += '}\n\n'

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  # Output

  substitute = {}
  substitute['LICENSE']         = args.license
  substitute['AUTOGENERATED']   = args.generated
  substitute['COPYRIGHT']       = args.copyright
  substitute['CONSTRUCTOR']     = apiDispatchLayerConstructor( "Http" )
  substitute['API_FUNC_DEFINE'] = code
  substitute['API_GL_DISPATCH_INIT']     = apiDispatchFuncInitCode( apis, args, 'http' )
  substitute['API_GLOBAL_DISPATCH_INIT'] = apiDispatchGlobalFuncInitCode( apis, args, 'http' )

  outputCode( '%s/layer/http/HttpProcs.cpp' % args.srcdir, dispatchHttpTemplate.substitute(substitute))
