#!/usr/bin/env python2.6
# Copyright 2014 iminer. All Rights Reserved.
# Author:Zhouxiaoliang
# script to generate callback_spec.h
# usage: ./gen_callback_spec.py [max_num] > callback_spec.h

import sys

def Usage():
  return './gen_callback_spec.py [max_num(>=2)] > callback_spec.h'


def PrintHeader():
  print '// Copyright 2010 Yunrang Inc. All Rights Reserved.'
  print '// This is generated by script, DO NOT edit it.\n'
  print '#ifndef BASE_CALLBACK_SPEC_H_'
  print '#define BASE_CALLBACK_SPEC_H_'
  print '\n#include "base/basictypes.h"\n'
  print 'namespace base {\n'
  pass


def PrintTail():
  print '}  // namespace base\n'
  print '#endif  // BASE_CALLBACK_SPEC_H_'


def GetArgList(has_class, has_type, has_obj, is_member, m, n):
  assert has_class or has_type or has_obj
  if n == 0:
    return ''
  template = has_class and 'class ' or ''
  template += has_type and 'Arg%d' or ''
  if has_obj:
    if len(template) > 0:
      template += ' '
    template += 'arg%d'
    if is_member:
      template += '_'
  if has_type and has_obj:
    return ', '.join([template % (x, x) for x in range(m+1, n+1)])
  return ', '.join([template % x for x in range(m+1, n+1)])


def Append(has_class, header, string):
  if len(string) == 0:
    return header
  tmp_str = string
  if has_class:
    if (string == 'del'):
      tmp_str = 'bool ' + string
    else:
      tmp_str = 'class ' + string
  if len(header) > 0:
    return header + ', ' + tmp_str
  return tmp_str


def GetClassTemplateHeader(has_class, has_del, has_result, has_obj, n):
  header = ''
  if has_del:
    header = Append(has_class, header, 'del')
  if has_result:
    header = Append(has_class, header, 'R')
  if has_obj:
    header = Append(has_class, header, 'T')
  header = Append(False, header, GetArgList(True, True, False, False, 0, n))
  if len(header) == 0:
    return ''
  return header


def GetClosureName(has_template, n):
  if (n > 0):
    template = has_template and '<%s>' % GetArgList(False, True, False, False, 0, n) or ''
    return 'Callback%d%s' % (n, template)
  return 'Closure'


def GetResultCallbackName(has_template, n):
  template = (has_template and
              '<R, %s>' % GetArgList(False, True, False, False, 0, n) or '')
  if (n > 0):
    return 'ResultCallback%d%s' % (n, template)
  return has_template and 'ResultCallback<R>' or 'ResultCallback'


def PrintClosureAndRC(n):
  def PrintClass(is_rc, name, n):
    r = is_rc and 'R' or 'void'
    if is_rc or n > 0:
      print ('template <%s>' %
             GetClassTemplateHeader(True, False, is_rc, False, n))
    print 'class %s {' % name
    print ' public:'
    print '  %s() {}' % name
    print '  virtual ~%s() {}' % name
    print '  virtual %s Run(%s) = 0;' % (r, GetArgList(False, True, False, False, 0, n))
    print ' private:'
    print '  DISALLOW_COPY_AND_ASSIGN(%s);' % name
    print '};\n'
  PrintClass(False, GetClosureName(False, n), n)
  PrintClass(True, GetResultCallbackName(False, n), n)


def GetClassName(is_member, is_const, m, n):
  if is_member:
    return (is_const and '_ConstMemberResultCallback_%d_%d' % (m, n) or
            '_MemberResultCallback_%d_%d' % (m, n))
  return '_FunctionResultCallback_%d_%d' % (m, n)


def PrintCallback(is_member, is_const, is_void, m, n):
  name = GetClassName(is_member, is_const, m, n)
  parent = (is_void and
            GetClosureName(True, n-m) or GetResultCallbackName(True, n-m))
  r = is_void and 'void' or 'R'
  fr = not is_void and 'R result = ' or ''
  end = not is_void and '\n    return result;' or ''
  end_const = (is_const and ' const' or '')
  before_const = (is_const and 'const ' or '')
  ct = ''
  if is_void:
    ct = is_member and '<del, void, T' or '<del, void'
    ct += n > 0 and ', ' + GetArgList(False, True, False, False, 0, n) or ''
    ct += '>'
  sup_arg = m > 0 and ', ' + GetArgList(False, True, True, False, n-m, n) or ''
  init_arg = (m > 0 and ', '.join([''] +
              ['arg%d_(arg%d)'% (x, x) for x in range(n-m+1, n+1)]) or '')
  mem_arg = GetArgList(False, False, True, True, n-m, n)
  if n-m > 0 and m > 0:
    mem_arg = ', ' + mem_arg
  member = m > 0 and '\n'.join([''] +
      ['  Arg%d arg%d_;' % (x,x) for x in range(n-m+1, n+1)] + ['']) or ''
  sig_name = is_member and 'MemberSignature' or 'FunctionSignature'
  method = is_member and 'member' or 'function'
  obj = is_member and '\n  %sT* object_;' % before_const or ''
  call = is_member and 'object_->' or ''
  obj_head = is_member and 'T::' or ''
  obj_arg = is_member and 'T* object, ' or ''
  obj_init = is_member and 'object_(object), ' or ''

  print 'template <%s>' % GetClassTemplateHeader(True, True, not is_void, is_member, n)
  print 'class %s%s : public %s {' % (name, ct, parent)
  print ' public:'
  print '  typedef %s base;' % parent
  print('  typedef %s (%s*%s)(%s)%s;' %
        (r, obj_head, sig_name, GetArgList(False, True, False, False, 0, n), end_const))
  print('  inline %s(%s%s%s %s%s)' %
        (name, before_const, obj_arg, sig_name, method, sup_arg))
  print '      : %s%s_(%s)%s {}\n' % (obj_init, method, method, init_arg)
  print '  virtual %s Run(%s) {' % (r, GetArgList(False, True, True, False, 0, n-m))
  print('    %s(%s*%s_)(%s%s);' %
        (fr, call, method, GetArgList(False, False, True, False, 0, n-m), mem_arg))
  print '    if (del) {'
  print '      //  zero out the pointer to ensure segfault if used again'
  print '      %s_ = NULL;' % method
  print '      delete this;\n    }%s\n  }\n private:%s' % (end, obj)
  print '  %s %s_;%s' % (sig_name, method, member)
  print '  DISALLOW_COPY_AND_ASSIGN(%s);\n};\n' % name


def GetNewCallbackTemplate(is_member, n):
  result = ''
  if is_member:
    result = Append(True, result, 'T1')
    result = Append(True, result, 'T2')
  result = Append(True, result, 'R')
  if n > 0:
    result += ', ' + GetArgList(True, True, False, False, 0, n)
  return result


def PrintNewCallback(is_member, is_const, self_delete, m, n):
  method = self_delete and 'NewOneTimeCallback' or 'NewPermanentCallback'
  name = GetClassName(is_member, is_const, m, n)
  obj_name = is_member and 'member' or 'function'
  template = self_delete and 'true' or 'false'
  template += ', R'
  template += is_member and ', T1' or ''
  template += n > 0 and ', ' + GetArgList(False, True, False, False, 0, n) or ''
  arg = is_member and is_const and 'const ' or ''
  arg += is_member and 'T1* obj, ' or ''
  obj = is_member and 'T2::' or ''
  end_const = is_member and is_const and ' const' or ''
  in_arg = m > 0 and ', ' + GetArgList(False, True, True, False, n-m, n) or ''
  para = is_member and 'obj, ' or ''
  para += is_member and 'member' or 'function'
  para += m > 0 and ', ' + GetArgList(False, False, True, False, n-m, n) or ''
  print 'template <%s>' % GetNewCallbackTemplate(is_member, n)
  print 'inline typename %s<%s>::base*' % (name, template)
  print '%s(%sR (%s*%s)(%s)%s%s) {' % (method, arg, obj, obj_name,
      GetArgList(False, True, False, False, 0, n), end_const, in_arg)
  print '  return new %s<%s>(%s);\n}\n' % (name, template, para)


def PrintN(n):
  def PrintM(m, n):
    PrintCallback(True, True, False, m, n)
    PrintCallback(True, True, True, m, n)
    PrintNewCallback(True, True, True, m, n)
    PrintNewCallback(True, True, False, m, n)
    PrintCallback(True, False, False, m, n)
    PrintCallback(True, False, True, m, n)
    PrintNewCallback(True, False, True, m, n)
    PrintNewCallback(True, False, False, m, n)
    PrintCallback(False, False, False, m, n)
    PrintCallback(False, False, True, m, n)
    PrintNewCallback(False, False, True, m, n)
    PrintNewCallback(False, False, False, m, n)

  PrintClosureAndRC(n)
  for m in range(n+1):
    PrintM(m, n)
  pass


def main(args):
  if len(args) != 2:
    Usage()
    return
  n = int(args[1])
  assert n >= 2, Usage()

  PrintHeader()
  for k in range(n):
    PrintN(k)
  PrintTail()
  pass

if __name__ == '__main__':
  main(sys.argv)
