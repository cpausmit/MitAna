import sys
import ROOT

class Configurable(object):
    """
    A wrapper for TObjects with records for all non-constant member functions 
    called during job setup. This information is used in clone() and dumpPython()
    methods to reproduce the configuration of the object.
    """

    class Method(object):
        """
        A wrapper for method class members that calls back the callMethod of
        mother configurable.
        """

        def __init__(self, name, configurable):
            self._name = name
            self._mother = configurable

        def __call__(self, *args):
            return self._mother._callMethod(self._name, args)

    
    class Generator(object):
        """
        Constructor wrapper for Configurables. Can generate objects of
        derived classes of Configurable.
        Also provides access to static member of the Configurable:
         mithep.MyModule.static_var
        """

        def __init__(self, cppcls, clsName, pytype):
            """
            cppcls: C++ class
            clsName: Class name in python code (e.g. mithep.MyModule)
            pytype: Derived class of Configurable
            """

            self._cppcls = cppcls
            self._clsName = clsName
            self._pytype = pytype
    
        def __call__(self, *args, **kwargs):
            """
            Instantiate the configurable.
            """

            return self._pytype(self._cppcls, self._clsName, *args, **kwargs)
    
        def __getattr__(self, name):
            """
            Access static members of the class.
            """

            member = getattr(self._cppcls, name)
            if issubclass(type(member), ROOT.PyRootType):
                # if the member is a class and its meta is PyRootType, return a generator for the class
                return Configurable.Generator(member, self._clsName + '.' + name, Configurable)

            return member


    def __init__(self, cppcls, clsName, *args, **kwargs):
        """
        kwargs allows CMSSW-like initializer
         mod = mithep.MyModule('module', 'my module', MyAttr = value, ..)
        which translates to
         mod = mithep.MyModule('module', 'my module')
         mod.SetMyAttr(value)
        """

        # need to rely on object.__setattr__ since we mess with out setattr below
        object.__setattr__(self, 'const', False) # when True, do not allow non-const attribute calls
        object.__setattr__(self, 'config', [('_Ctor', args, True)]) # attribute name, args, is method

        self._clsName = clsName

        unwrapped = []
        for arg in args:
            if isinstance(arg, Configurable):
                unwrapped.append(arg._cppobj)
            else:
                unwrapped.append(arg)

        self._cppobj = cppcls(*tuple(unwrapped))

        self._configureFromArgs(kwargs)

    def __getattr__(self, name):
        """
        Find the attribute from the core C++ object. If the attribued is a class method,
        return the Method object, which calls back _callMethod.
        """

        try:
            attr = getattr(self._cppobj, name)
        except AttributeError:
            print 'Class ' + self._clsName + ' has no attribute ' + name
            sys.exit(1)

        if type(attr) is ROOT.MethodProxy:
            attr = Configurable.Method(name, self)

        return attr

    def __setattr__(self, name, value):
        """
        Record the configuration provided in the direct form
         obj.attr = value
        """

        if self.const:
            print ' Attempt to set attribute', name, 'on a const object of class', self._clsName
            sys.exit(1)

        if not name.startswith('_'):
            try:
                # for expression of form
                #  obj.attr = value
                # is this an attribute of the C++ object?
                attr = getattr(self._cppobj, name)
                self.config.append((name, value, False))
                return

            except AttributeError:
                # if not, set attribute on the python object
                pass

        object.__setattr__(self, name, value)

    def _configureFromArgs(self, kwargs):
        for key, value in kwargs.items():
            try:
                method = getattr(self, 'Set' + key)
            except AttributeError:
                print 'Error raised in ' + self._clsName + ' constructor argument ' + key + ' = ' + str(value)
                sys.exit(1)

            if type(value) is tuple:
                method(*value)
            else:
                method(value)

    def _callMethod(self, methodName, args):
        """
        Calls the C++ object method after determining its constness.
        Record the arguments for non-const members.
        """

        if type(args) is not tuple:
            args = (args,)

        recordConfig = False
        valueStrings = []
        for val in args:
            # the actual values do not matter here - just used to determine what function is being called
            if type(val) is str:
                valueStrings.append('"abc"')
            elif type(val) is bool:
                valueStrings.append('true')
            elif type(val) is int or type(val) is float:
                valueStrings.append(str(val))
            else:
                # non-simple argument type found - can in principle go further to determine
                # the argument type and pass it to GetMethodWithPrototype, but at this point
                # it's simpler to say that this config line should be recorded.
                recordConfig = True

        if not recordConfig:
            m = self._cppobj.Class().GetMethod(methodName, ','.join(valueStrings), True) # True -> only look for const method
            recordConfig = (not m) # this method is not const

        if self.const and recordConfig:
            print ' Non-const member function', methodName, 'of class', self._clsName, 'was called for a const object.'
            print ' (There is a chance that the function is actually a const function. If this is the case wrap the'
            print ' calling line with [this_object].setConst(False) and [this_object].setConst(True)).'
            sys.exit(1)

        if recordConfig:
            self.config.append((methodName, args, True))

        unwrapped = []
        for arg in args:
            if isinstance(arg, Configurable):
                unwrapped.append(arg._cppobj)
            else:
                unwrapped.append(arg)

        method = getattr(self._cppobj, methodName)
        return method(*tuple(unwrapped))

    def setConst(self, c = True):
        object.__setattr__(self, 'const', c)

    def clone(self, *args, **kwargs):
        """
        Instantiate an object of the same type and call the recorded non-const methods
        in sequence. Does not copy the constness.
        """

        calls = iter(self.config)

        # first config is always Ctor
        if len(args) != 0:
            newObj = type(self)(type(self._cppobj), self._clsName, *args)
            calls.next()
        else:
            newObj = type(self)(type(self._cppobj), self._clsName, *calls.next()[1])

        while True:
            try:
                attrName, arguments, isMethod = calls.next()
                if isMethod:
                    # if method, args is a tuple
                    getattr(newObj, attrName)(*arguments)
                else:
                    # if not method, args is a flat variable
                    setattr(newObj, attrName, arguments)

            except StopIteration:
                break

        newObj._configureFromArgs(kwargs)

        return newObj

    def reset(self, *args, **kwargs):
        """
        Recreate the core object and clear the recorded configurations.
        """

        self._cppobj = type(self._cppobj)(*args)
        self.config = [('_Ctor', args, True)] # attribute name, args, is method

        self._configureFromArgs(kwargs)

    def dumpPython(self, varName = 'v1', withCtor = True, objects = {}):
        """
        Return a python code snippet that constructs this object and calls the recorded
        non-const methods in sequence.
        """

        code = ''

        # first write down any undefined Configurables
        for attrName, args, isMethod in self.config:
            if not isMethod:
                args = (args,)

            for arg in args:
                if not isinstance(arg, Configurable) or arg in objects:
                    continue

                auxName = 'aux' + str(len(objects))
                objects[arg] = auxName
                code += arg.dumpPython(varName = auxName, objects = objects)
                code += '\n'

        for attrName, args, isMethod in self.config:
            if isMethod:
                argList = []
                for arg in args:
                    if isinstance(arg, Configurable):
                        argList.append(objects[arg])
                    elif type(arg) is str:
                        argList.append("'%s'" % arg)
                    elif isinstance(arg, ROOT.ObjectProxy):
                        print 'Error in %s.%s()' % (varName, attrName)
                        print 'Text dump of C++ class in function argument is not supported.'
                        sys.exit(1)
                    else:
                        argList.append(str(arg))
    
                argStr = ', '.join(argList)
                if attrName == '_Ctor':
                    if withCtor:
                        code += '%s = %s(%s)\n' % (varName, self._clsName, argStr)
                else:
                    code += '%s.%s(%s)\n' % (varName, attrName, argStr)

            else:
                # args in this case is a single variable
                code += '%s.%s = %s\n' % (varName, attrName, str(args))

        return code
