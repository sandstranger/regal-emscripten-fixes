#!/usr/bin/python -B

from string       import Template, upper, replace
from ApiUtil      import outputCode
from ApiUtil      import typeIsVoid
from ApiUtil      import typeIsVoidPointer
from ApiUtil      import toLong
from ApiUtil      import hexValue
from ApiCodeGen   import *
from ApiRegal     import logFunction
from Emu          import emuFindEntry, emuCodeGen

from RegalContext     import emuRegal
from RegalContextInfo import cond
from RegalSystem      import regalSys

publicHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#include <GL/Regal.h>

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include <stddef.h>

typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#include <windows.h>

${API_TYPEDEF}

#ifdef __cplusplus
extern "C" {
#endif

${API_ENUM}

${API_FUNC_DECLARE}

#ifdef __cplusplus
}
#endif

#endif /* __${HEADER_NAME}_H__ */

''')

def generateWglHeader(api, args):

  apiTypedef     = apiTypedefCode( api, args )
  apiEnum        = apiEnumCode(api, args)                 # CodeGen for API enums
  apiFuncDeclare = apiFuncDeclareCode( api, args )        # CodeGen for API functions

  # Output

  substitute = {}
  substitute['LICENSE']          = args.license
  substitute['AUTOGENERATED']    = args.generated
  substitute['COPYRIGHT']        = args.copyright
  substitute['HEADER_NAME']      = "REGAL_WGL"
  substitute['REGAL_SYS']        = regalSys
  substitute['API_TYPEDEF']      = apiTypedef
  substitute['API_ENUM']         = apiEnum
  substitute['API_FUNC_DECLARE'] = apiFuncDeclare

  outputCode( '%s/RegalWGL.h' % args.incdir, publicHeaderTemplate.substitute(substitute))

def apiTypedefCode( api, args ):

  def printTypedef( name, type ):
    if re.search( '\(\s*\*\s*\)', type ):
      return 'typedef %s;' % ( re.sub( '\(\s*\*\s*\)', '(*%s)' % name, type ) )
    else:
      return'typedef %s %s;' % ( type, name )

  code = ''
  code += '\n'
  if api.name in cond:
    code += '#if %s\n' % cond[api.name]

  if api.name == 'wgl':
    code += '#ifdef  REGAL_SYS_WGL_DECLARE_WGL\n'
    code += '#ifndef _WINDEF_\n'

  for typedef in api.typedefs:

    if api.name == 'wgl' and typedef.name=='GLYPHMETRICSFLOAT':
      code += '#endif\n'
      code += '#ifndef _WINGDI_\n'
    if api.name == 'wgl' and typedef.name=='HPBUFFERARB':
      code += '#endif\n'
      code += '#endif // REGAL_SYS_WGL_DECLARE_WGL\n'

    if isinstance(typedef.type, str) or isinstance(typedef.type, unicode):
      code += printTypedef( typedef.name, typedef.type ) + '\n'
    else:
      type = {}
      mapping = { 'wgl' : 'REGAL_SYS_WGL', '' : '' }
      for i in typedef.type:
        if i in mapping:
          type[mapping[i]] = printTypedef( typedef.name, typedef.type[i] )
      code += '\n'.join(wrapIf(type,None)) + '\n'

  if api.name in cond:
    code += '#endif // %s\n' % cond[api.name]
  code += '\n'

  return code

# CodeGen for custom API definitions.

def apiEnumCode( api, args ):

  code = ''
  code += '\n'
  if api.name in cond:
    code += '#if %s\n' % cond[api.name]
  code += '#if REGAL_SYS_WGL_DECLARE_WGL\n'
  for enum in api.enums:
    if enum.name == 'defines':
      pass
    else:
      code += '\ntypedef enum _%s {\n' % enum.name
      for enumerant in enum.enumerants:
        code += '  %s = %s,\n' % ( enumerant.name, enumerant.value )
      code += '} %s;\n\n' % enum.name
  code += '#endif // REGAL_SYS_WGL_DECLARE_WGL\n'
  if api.name in cond:
    code += '#endif // %s\n' % cond[api.name]
  code += '\n'

  return code

def apiVerInt( ver ):
  va = [ int( x ) for x in ver.split('.') ]
  while len( va ) < 3:
    va.append(0)
  return '%d%02d%02d' % tuple(va)

def apiFuncDecl( function ):
  name   = function.name
  params = paramsDefaultCode(function.parameters, True)
  rType  = typeCode(function.ret.type)
  funcDecl = ''
  guard = []
  if len(function.version):
    guard.append( 'GL_VER >= %s' % apiVerInt( function.version ) )
  if getattr(function,'esVersions', None) and len(function.esVersions):
    guard.extend( [ 'ES_VER == %s' % ( apiVerInt('%s' % x) ) for x in function.esVersions ] )
  if len(guard):
    funcDecl += '#if GL_VER_ALL || ' + ' || '.join( guard ) + '\n'
  funcDecl += 'REGAL_DECL %sREGAL_CALL %s(%s);\n' % (rType, name, params)
  if len(guard):
    funcDecl += '#endif\n'
  return funcDecl;

# CodeGen for API function declaration.

def apiFuncDeclareCode(api, args):
  code = ''


  d = [] # defines
  e = [] # enums
  t = [] # function pointer typedefs
  m = [] # mangled names for REGAL_NAMESPACE support
  f = [] # gl names

  for enum in api.enums:
    if enum.name == 'defines':
      for enumerant in enum.enumerants:

        # Ignore enums that match category, a workaround
        # for EGL_VERSION_1_3 and EGL_VERSION_1_4

        if enumerant.name==enumerant.category:
          continue

        value = toLong(enumerant.value)
        if value==None:
          value = enumerant.value

        # Don't bother with decimal for 0-10
        if isinstance(value, long) and value>=10:
          e.append((enumerant.category, '#define %s %s /* %s */'%(enumerant.name,hexValue(value),value)))
        else:
          e.append((enumerant.category, '#define %s %s'%(enumerant.name,hexValue(value))))

  for function in api.functions:

    name   = function.name
    params = paramsDefaultCode(function.parameters, True)
    rparams = paramsDefaultCode(function.parameters, True, "void *_context")
    rType  = typeCode(function.ret.type)
    category  = getattr(function, 'category', None)
    version   = getattr(function, 'version', None)

    if category:
      category = category.replace('_DEPRECATED', '')
    elif version:
      category = version.replace('.', '_')
      category = 'GL_VERSION_' + category

    t.append((category,funcProtoCode(function, version, 'REGAL_CALL', True)))
    m.append((category,'#define %-35s r%-35s' % (name, name) ))
    f.append((category, apiFuncDecl( function ) ))

  def cmpEnum(a,b):
    if a[0]==b[0]:
      aValue = a[1].split(' ')[2]
      bValue = b[1].split(' ')[2]
      if aValue==bValue:
        return cmp(a[1].split(' ')[1], b[1].split(' ')[1])
      else:
        return cmp(aValue,bValue)
    return cmp(a[0],b[0])

  def enumIfDef(category):
    return '#ifndef REGAL_NO_ENUM_%s'%(upper(category).replace(' ','_'))

  def typedefIfDef(category):
    return '#ifndef REGAL_NO_TYPEDEF_%s'%(upper(category).replace(' ','_'))

  def namespaceIfDef(category):
    return '#ifndef REGAL_NO_NAMESPACE_%s'%(upper(category).replace(' ','_'))

  def declarationIfDef(category):
    return '#ifndef REGAL_NO_DECLARATION_%s'%(upper(category).replace(' ','_'))

  categories = set()

  if api.name=='egl':
    categories.add('EGL_VERSION_1_3')
    categories.add('EGL_VERSION_1_4')

  categories.update([ i[0] for i in e ])
  categories.update([ i[0] for i in t ])
  categories.update([ i[0] for i in m ])
  categories.update([ i[0] for i in f ])

  for i in categories:
    if len(i):
      cat = upper(i).replace(' ','_')

      d.append((i,'#if (defined(%s) || defined(REGAL_NO_ENUM) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_ENUM_%s)'%(cat,cat,cat)))
      d.append((i,'#define REGAL_NO_ENUM_%s'%(cat)))
      d.append((i,'#endif'))
      d.append((i,''))

      d.append((i,'#if (defined(%s) || defined(REGAL_NO_TYPEDEF) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_TYPEDEF_%s)'%(cat,cat,cat)))
      d.append((i,'#define REGAL_NO_TYPEDEF_%s'%(cat)))
      d.append((i,'#endif'))
      d.append((i,''))

      d.append((i,'#if (defined(%s) || !defined(REGAL_NAMESPACE) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_NAMESPACE_%s)'%(cat,cat,cat)))
      d.append((i,'#define REGAL_NO_NAMESPACE_%s'%(cat)))
      d.append((i,'#endif'))
      d.append((i,''))

      d.append((i,'#if (defined(%s) || !defined(REGAL_PLUGIN_MODE) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_PLUGIN_%s)'%(cat,cat,cat)))
      d.append((i,'#define REGAL_NO_PLUGIN_%s'%(cat)))
      d.append((i,'#endif'))
      d.append((i,''))

      d.append((i,'#if (defined(%s) || defined(REGAL_NO_DECLARATION) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_DECLARATION_%s)'%(cat,cat,cat)))
      d.append((i,'#define REGAL_NO_DECLARATION_%s'%(cat)))
      d.append((i,'#endif'))
      d.append((i,''))

      d.append((i,'#ifndef %s'%(i)))
      d.append((i,'#define %s 1'%(i)))
      d.append((i,'#endif'))
      d.append((i,''))

  e.sort(cmpEnum)
  e = alignDefineCategory(e)
  e = ifCategory(e,enumIfDef)
  e = spaceCategory(e)

  t.sort()
  t = ifCategory(t,typedefIfDef)
  t = spaceCategory(t)

  m.sort()
  m = ifCategory(m,namespaceIfDef)
  m = spaceCategory(m)

  f.sort()
  f = ifCategory(f,declarationIfDef)
  f = spaceCategory(f)

  d.extend(e)
  d.extend(t)
  d.extend(m)
  d.extend(f)

  tmp = listToString(unfoldCategory(d,'/**\n ** %s\n **/',lambda x,y: cmp(x[0], y[0])))

  if api.name == 'wgl':
    tmp = wrapIf('REGAL_SYS_WGL_DECLARE_WGL',tmp)
  if api.name in cond:
    tmp = wrapIf(cond[api.name], tmp)

  code += '%s\n'%(tmp)

  return code
