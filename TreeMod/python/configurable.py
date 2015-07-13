import ROOT

class Configurable(object):
    """
    A wrapper for TObjects with a storage for all non-constant member functions 
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


    def __init__(self, cppcls, cppclsName, *args):
        self._clsName = cppclsName
        self._cppobj = cppcls(*args)
        self.config = [('_Ctor', args)]

    def __getattr__(self, name):
        attr = getattr(self._cppobj, name)

        if type(attr) is ROOT.MethodProxy:
            attr = Configurable.Method(name, self)

        setattr(self, name, attr)

        return attr

    def _callMethod(self, methodName, args):
        if type(args) is not tuple:
            args = (args,)

        storeConfig = False
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
                # it's simpler to say that this config line should be stored.
                storeConfig = True

        if not storeConfig:
            m = self._cppobj.Class().GetMethod(methodName, ','.join(valueStrings), True) # True -> only look for const method
            storeConfig = (not m) # this method is not const

        if storeConfig:
            self.config.append((methodName, args))

        unwrapped = []
        for arg in args:
            if isinstance(arg, Configurable):
                unwrapped.append(arg._cppobj)
            else:
                unwrapped.append(arg)

        args = tuple(unwrapped)

        method = getattr(self._cppobj, methodName)
        return method(*args)

    def clone(self):
        calls = iter(self.config)

        newObj = type(self)(*calls.next()[1])

        while True:
            try:
                methodName, args = calls.next()
                getattr(newObj, methodName)(*args)

            except StopIteration:
                break

        return newObj

    def dumpPython(self, varName, objects):

        code = ''

        for methodName, args in self.config:
            argList = []
            for arg in args:
                if isinstance(arg, Configurable):
                    argList.append(objects[arg])
                elif type(arg) is str:
                    argList.append("'%s'" % arg)
                else:
                    argList.append(str(arg))

            argStr = ', '.join(argList)
            if methodName == '_Ctor':
                code += '%s = %s(%s)\n' % (varName, self._clsName, argStr)
            else:
                code += '%s.%s(%s)\n' % (varName, methodName, argStr)

        code += '\n'
        return code
