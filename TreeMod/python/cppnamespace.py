import os
import sys
import ROOT
from configurable import Configurable
from sequenceable import Node

class Module(Configurable, Node):
    """
    A wrapper for all Bambu modules inheriting from mithep::BaseMod.
    """

    def __init__(self, cppcls, clsName, *args, **kwargs):
        Configurable.__init__(self, cppcls, clsName, *args, **kwargs)
        Node.__init__(self, self._cppobj, self._cppobj.GetName())

    def connect(self, nextNode):
        """
        Overriding Node.connect which gets called when building a sequence
        """
        Node.connect(self, nextNode)
        self._cppobj.Add(nextNode._cppobj)


class CppNamespace(object):
    """
    PyROOT wrapper for generic C++ namespace.
    """

    loadedLibs = []

    def __init__(self, namespace, namespaceName, superspaces = []):
        self._core = namespace
        self._name = namespaceName
        self._superspaces = superspaces

    def __getattr__(self, name):
        try:
            cls = getattr(self._core, name)
        except:
            # search for the class name in the compiled libraries
            try:
                cppname = ''
                for sup in self._superspaces:
                    cppname += sup + '::'
                cppname += self._name + '::' + name

                mangled = '_ZN'
                for sup in self._superspaces:
                    mangled += str(len(sup)) + sup
                mangled += str(len(self._name)) + self._name
                if '<' in name:
                    # templated class - can only deal with simple templates
                    op = name.find('<')
                    if name.rfind('<') != op:
                        raise Exception()
    
                    cl = name.find('>')
                    mangled += str(op) + 'INS_' + str(cl - op - 1) + name[op + 1:cl] + 'EE'
    
                else:
                    mangled += str(len(name)) + name

                # mangled name now looks like '_ZN6mithep9OutputMod'

                libdirs = os.environ['LD_LIBRARY_PATH'].split(':')
                for libdir in libdirs:
                    for libname in os.listdir(libdir):
                        # only look at libraries that start with libMit or those linked manually
                        if not libname.startswith('libMit') or libname in CppNamespace.loadedLibs:
                            continue
    
                        with open(libdir + '/' + libname, 'rb') as lib:
                            cont = lib.read()
                            if cont.find(mangled) < 0 and cont.find(cppname) < 0:
                                # typedefs are written directly with cppname
                                continue

                            print '(mithep): Auto-loading library', libname
                            ROOT.gSystem.Load(libname)
                            CppNamespace.loadedLibs.append(libname)
                            try:
                                cls = getattr(self._core, name)
                                break
                            except:
                                continue
                    else:
                        continue
    
                    break
                else:
                    raise Exception()

            except:
                print 'No class "' + name + '" found in namespace mithep. Perhaps a missing library?'
                sys.exit(1)

        # class full name with all namespaces
        clsName = ''
        for sup in self._superspaces:
            clsName += sup + '.'
        clsName += self._name + '.' + name

        try:
            tclass = cls.Class()
        except AttributeError:
            tclass = None

        if tclass:
            # this is a TObject
            configurable = Configurable
            if tclass.InheritsFrom(ROOT.mithep.BaseMod.Class()):
                configurable = Module

            ret = Configurable.Generator(cls, clsName, configurable)

        elif issubclass(type(cls), ROOT.PyRootType):
            # for now, all PyRootType that have MethodProxy attributes are treated as Configurable
            # would be nice in future to check for non-const methods (otherwise e.g. TMath is a Configurable)
            for attr in cls.__dict__.values():
                if type(attr) is ROOT.MethodProxy:
                    ret = Configurable.Generator(cls, clsName, Configurable)
                    break

            else:
                # no MethodProxy -> this is a namespace
                ret = CppNamespace(cls, name, self._superspaces + [self._name])

        else:
            # this is a simple variable
            ret = cls

        setattr(self, name, ret)

        return ret

    @staticmethod
    def loadlib(libName):
        ROOT.gSystem.Load(libName)
        CppNamespace.loadedLibs.append(libName)

