#!/usr/bin/python -B

from string import Template, upper, replace

from ApiUtil import outputCode
from ApiUtil import typeIsVoid
from ApiUtil import typeIsVoidPointer

from ApiCodeGen import *

from RegalContextInfo import cond as condDefault

############################################################################

loaderSourceTemplate = Template('''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

${IFDEF}REGAL_GLOBAL_BEGIN

#include <string>
using namespace std;

#include "RegalLog.h"
#include "RegalBreak.h"
#include "RegalPush.h"
#include "RegalToken.h"
#include "RegalHelper.h"
#include "RegalPrivate.h"
#include "RegalContext.h"
#include "RegalDispatcherGL.h"
#include "RegalDispatcherGlobal.h"
${LOCAL_INCLUDE}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

${LOCAL_CODE}

${API_DISPATCH_FUNC_DEFINE}

  template <typename T>
  void _getProcAddress( T & func, T funcRegal, const char *name)
  {
    GetProcAddress(func, name);
    RegalAssert(func!=funcRegal);
    if (func==funcRegal)
      func = NULL;
  }

  void InitDispatchGLLoader( Dispatch::GL & dt )
  {
${API_DISPATCH_FUNC_INIT}
  }

${API_DISPATCH_GLOBAL_FUNC_INIT}

REGAL_NAMESPACE_END

${ENDIF}''')

# CodeGen for missing dispatch functions

def apiMissingFuncDefineCode(apis, args):

  code = ''
  categoryPrev = None

  cond = condDefault

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:

      if getattr(function,'regalOnly',False)==True:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType     = typeCode(function.ret.type)
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

      code += '  static %sREGAL_CALL missing_%s(%s) \n  {\n' % (rType, name, params)
      for param in function.parameters:
        code += '    UNUSED_PARAMETER(%s);\n' % param.name
      code += '    Warning( "%s", " not available." );\n' % name

      if not typeIsVoid(rType):
        if rTypes in api.defaults:
          code += '    return %s;\n' % ( api.defaults[rTypes] )
        else:
          if rType[-1]=='*' or typeIsVoidPointer(rType):
            code += '    return NULL;\n'
          else:
            code += '    return (%s) 0;\n' % ( rTypes )

      code += '  }\n\n'

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  return code

def apiDispatchFuncInitCode(apis, args, dispatchName, exclude=[], filter = lambda x : True, cond = None):

  if not cond:
    cond = condDefault

  categoryPrev = None
  code = ''

  for api in apis:

    code += '\n'

    for function in api.functions:

      if not function.needsContext:
        continue

      if not filter(function):
        continue

      if getattr(function,'regalOnly',False)==True:
        continue

      if function.name in exclude or function.category in exclude:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
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
        code += '    // %s\n\n' % category

      categoryPrev = category

      # Get a reference to the appropriate dispatch table and attempt GetProcAddress

      code += '    _getProcAddress( dt.%s, %s, "%s" );\n'%(name,name,name)
      code += '    if( dt.%s == NULL ) {\n' % name
      code += '      dt.%s = missing_%s;\n' % (name,name)
      code += '    }\n'

    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  return code

def apiDispatchGlobalFuncInitCode(apis, args, dispatchName, exclude=[], filter = lambda x : True, cond = None):

  if not cond:
    cond = condDefault

  categoryPrev = None
  code = ' void InitDispatchGlobalLoader(Dispatch::Global &dt) {\n'

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '  #if %s\n' % cond[api.name]

    for function in api.functions:

      if function.needsContext:
        continue

      if not filter(function):
        continue

      if getattr(function,'regalOnly',False)==True:
        continue

      if function.name in exclude or function.category in exclude:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
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
        code += '    // %s\n\n' % category

      categoryPrev = category


      code += '    _getProcAddress( dt.%s, %s, "%s" );\n'%(name,name,name)
      code += '    if( dt.%s == NULL ) {\n' % name
      code += '      dt.%s = missing_%s;\n' % (name,name)
      code += '    }\n'

    if api.name in cond:
      code += '  #endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  code += '}\n'

  return code



def generateLoaderSource(apis, args):

  funcDefine   = apiMissingFuncDefineCode(apis, args)
  funcInit   = apiDispatchFuncInitCode( apis, args, None )
  globalFuncInit   = apiDispatchGlobalFuncInitCode( apis, args, None )

  # Output

  substitute = {}

  substitute['LICENSE']         = args.license
  substitute['AUTOGENERATED']   = args.generated
  substitute['COPYRIGHT']       = args.copyright
  substitute['DISPATCH_NAME']   = 'Loader'
  substitute['LOCAL_CODE']      = ''
  substitute['LOCAL_INCLUDE']   = ''
  substitute['API_DISPATCH_FUNC_DEFINE'] = funcDefine
  substitute['API_DISPATCH_FUNC_INIT'] = funcInit
  substitute['API_DISPATCH_GLOBAL_FUNC_INIT'] = globalFuncInit
  substitute['IFDEF'] = '#if REGAL_DRIVER && REGAL_LOADER\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchLoader.cpp' % args.srcdir, loaderSourceTemplate.substitute(substitute))
