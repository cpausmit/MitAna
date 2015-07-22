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

            return getattr(self._cppcls, name)


    def __init__(self, cppcls, clsName, *args, **kwargs):
        """
        kwargs allows CMSSW-like initializer
         mod = mithep.MyModule('module', 'my module', MyAttr = value, ..)
        which translates to
         mod = mithep.MyModule('module', 'my module')
         mod.SetMyAttr(value)
        """

        self._cppobj = cppcls(*args)
        self._clsName = clsName
        self.config = [('_Ctor', args, True)] # attribute name, args, is method

        self._configureFromArgs(kwargs)

    def __getattr__(self, name):
        """
        Find the attribute from the core C++ object. If the attribued is a class method,
        return the Method object, which calls back _callMethod.
        """

        try:
            attr = getattr(self._cppobj, name)
        except AttributeError:
            print 'Class ' + self._clsName + ' has no attribute ' + key
            raise

        if type(attr) is ROOT.MethodProxy:
            attr = Configurable.Method(name, self)

        return attr

    def __setattr__(self, name, value):
        """
        Record the configuration provided in the direct form
         obj.attr = value
        """

        if not name.startswith('_'):
            try:
                attr = getattr(self._cppobj, name)
                self.config.append((name, value, False))
                return

            except AttributeError:
                pass

        object.__setattr__(self, name, value)

    def _configureFromArgs(self, kwargs):
        for key, value in kwargs.items():
            try:
                method = getattr(self, 'Set' + key)
            except AttributeError:
                print 'Error raised in ' + self._clsName + ' constructor argument ' + key + ' = ' + str(value)
                raise

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

        if recordConfig:
            self.config.append((methodName, args, True))

        unwrapped = []
        for arg in args:
            if isinstance(arg, Configurable):
                unwrapped.append(arg._cppobj)
            else:
                unwrapped.append(arg)

        args = tuple(unwrapped)

        method = getattr(self._cppobj, methodName)
        return method(*args)

    def clone(self, *args, **kwargs):
        """
        Instantiate an object of the same type and call the recorded non-const methods
        in sequence.
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

    def dumpPython(self, varName = 'v1', objects = {}):
        """
        Return a python code snippet that constructs this object and calls the recorded
        non-const methods in sequence.
        """

        code = ''

        for attrName, args, isMethod in self.config:
            if isMethod:
                argList = []
                for arg in args:
                    if isinstance(arg, Configurable):
                        argList.append(objects[arg])
                    elif type(arg) is str:
                        argList.append("'%s'" % arg)
                    else:
                        argList.append(str(arg))
    
                argStr = ', '.join(argList)
                if attrName == '_Ctor':
                    code += '%s = %s(%s)\n' % (varName, self._clsName, argStr)
                else:
                    code += '%s.%s(%s)\n' % (varName, attrName, argStr)

            else:
                code += '%s.%s = %s\n' % (varName, attrName, str(args))

        return code
