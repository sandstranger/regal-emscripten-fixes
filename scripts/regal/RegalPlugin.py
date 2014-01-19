#!/usr/bin/python -B

#
# Plugin interface for Regal
#
# RegalPlugin.cpp provides the functions for
# plugin layers to call back into the Regal
# emulation or dispatch stack.
#

from string import Template, upper, replace

from ApiUtil import outputCode
from ApiUtil import typeIsVoid

from ApiCodeGen   import *

from RegalContextInfo import cond

# Code generation for RegalPlugin.cpp

pluginHeaderTemplate = Template('''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#define REGAL_PLUGIN_MODE 1

#include "RegalUtil.h"

#if REGAL_PLUGIN

REGAL_GLOBAL_BEGIN

#include "RegalContext.h"
#include "RegalLookup.h"
#include "RegalDispatch.h"

extern "C" {

${PLUGIN_SOURCE}

}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Plugin {

${PLUGIN_SOURCE2}

}

REGAL_NAMESPACE_END

REGAL_GLOBAL_BEGIN

extern "C" {

${PLUGIN_SOURCE3}

}

REGAL_GLOBAL_END

#endif // REGAL_PLUGIN

''')


def generatePluginSource(apis, args):

  code = ''
  for api in apis:

    tmp = []
    for function in api.functions:

      if getattr(function,'regalOnly',False):
        continue

      name       = function.name
      params     = paramsDefaultCode(function.parameters, True, "void *_context")
      callParams = paramsNameCode(function.parameters, "reinterpret_cast< ::REGAL_NAMESPACE_INTERNAL::RegalContext * >(_context)")
      rType      = typeCode(function.ret.type)
      rTypes     = rType.strip()
      category   = getattr(function, 'category', None)
      version    = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      c = ''
      c += '%sREGAL_CALL plugin_%s(%s) \n{\n' % (rType, name, params)

      if function.needsContext:
        c += '  ::REGAL_NAMESPACE_INTERNAL::Thread::ThreadLocal &_instance = ::REGAL_NAMESPACE_INTERNAL::Thread::ThreadLocal::instance();\n'
        c += '  //UNUSED_PARAMETER( _instance );\n'
        c += '  //::REGAL_NAMESPACE_INTERNAL::Dispatch::GL *_next = _instance.nextDispatchTable;\n'
        c += '  ::REGAL_NAMESPACE_INTERNAL::Dispatch::GL *_next = &_instance.currentContext->dispatchGL;\n'
      else:
        c += '  //::REGAL_NAMESPACE_INTERNAL::DispatchGlobal *_next = _instance.nextDispatchTableGlobal;\n'
        c += '  ::REGAL_NAMESPACE_INTERNAL::Dispatch::Global *_next = &::REGAL_NAMESPACE_INTERNAL::dispatchGlobal;\n'
      c += '  RegalAssert(_next);\n'
      if not typeIsVoid(rType):
        c += '  return '
      else:
        c += '  '
      c += '_next->%s(%s);\n}\n' % ( name, callParams )

      tmp.append( (category, indent(c,'  ') ) )

    tmp = listToString(unfoldCategory(tmp,'  /* %s */'))

    if api.name in cond:
      tmp = wrapIf(cond[api.name], tmp)

    code += tmp

  # lookup by name

  code2 = ''
  size = {}
  for i in apis:
    tmp = []

    names = []
    for j in i.functions:
      if getattr(j,'regalOnly',False):
        continue
      names.append(j.name)

    size[i.name] = len(names)
    tmp.extend(pointerLookupByNameCode([ (j,'plugin_%s'%j) for j in names ],("lookup_%s_Name"%i.name,"lookup_%s_Value"%i.name),valueCast = '(void *)(%s)'))

    tmp = '\n'.join(tmp)
    if i.name in cond:
      tmp = wrapIf(cond[i.name], tmp)

    code2 += '\n' + indent(tmp,'  ')

  # glGetProcAddress

  code3 = '''
  using namespace ::REGAL_NAMESPACE_INTERNAL::Lookup;
  using namespace ::REGAL_NAMESPACE_INTERNAL::Plugin;

  void * REGAL_CALL
  plugin_glGetProcAddress(const char *name)
  {
    const char **res;
'''

  for i in apis:
    tmp =  '    res = (const char **) std::bsearch(&name, %s, %d, sizeof(const char *), NameCmp);\n'%('lookup_%s_Name'%i.name,size[i.name]-1)
    tmp += '    if (res) return const_cast<void *>(%s[(size_t) (res - %s)]);\n'%('lookup_%s_Value'%i.name,'lookup_%s_Name'%i.name)
    if i.name in cond:
      tmp = wrapIf(cond[i.name], tmp)
    code3 += '\n' + tmp

  code3 += '''
    return NULL;
  }
'''

  # Output

  substitute = {}

  substitute['LICENSE']         = args.license
  substitute['AUTOGENERATED']   = args.generated
  substitute['PLUGIN_SOURCE']   = code
  substitute['PLUGIN_SOURCE2']  = code2
  substitute['PLUGIN_SOURCE3']  = code3

  outputCode( '%s/RegalPlugin.cpp' % args.srcdir, pluginHeaderTemplate.substitute(substitute))

