import os
import sys
import ROOT
from sequenceable import Node
from configurable import Configurable

ROOT.gROOT.SetBatch(True)
ROOT.gSystem.Load('libMitAnaTreeMod.so')

class Module(Configurable, Node):
    """
    A wrapper for all Bambu modules inheriting from mithep::BaseMod.
    """

    def __init__(self, cppcls, cppclsName, *args):
        Configurable.__init__(self, cppcls, cppclsName, *args)
        Node.__init__(self, self._cppobj, self._cppobj.GetName())


class _Generator(object):
    """
    Constructor wrapper for C++ objects to allow CMSSW-like initializer
     mod = mithep.Module('module', 'my module', Attr1 = value, ..)
    which translates to
     mod = mithep.Module('module', 'my module')
     mod.SetAttr1(value)
    """

    def __init__(self, cls, clsName):
        self._cls = cls
        self._clsName = clsName

    def __call__(self, *args, **kwargs):
        try:
            tclass = self._cls.Class()
        except AttributeError:
            tclass = None

        if tclass:
            if tclass.InheritsFrom(ROOT.mithep.BaseMod.Class()):
                obj = Module(self._cls, self._clsName, *args)
            else:
                obj = Configurable(self._cls, self._clsName, *args)
        else:
            obj = self._cls(*args)

        for key, value in kwargs.items():
            try:
                method = getattr(obj, 'Set' + key)
            except AttributeError:
                print 'class ' + self._clsName + ' has no attribute ' + key + ' (i.e. has no method Set' + key + ').'
                sys.exit(1)

            if type(value) is tuple:
                method(*value)
            else:
                method(value)

        return obj

    def __getattr__(self, name):
        """
        Access static members of the class
        """
        return getattr(self._cls, name)


class _mithep(object):
    """
    PyROOT wrapper for mithep namespace
    """

    def __init__(self):
        self._core = ROOT.mithep

    def __getattr__(self, name):
        try:
            cls = getattr(self._core, name)
        except:
            # search in the standard library directory and find a possible match
            try:
                loaded = map(os.path.basename, ROOT.gSystem.GetLibraries('libMit', '', False).split())
    
                mangled = '_ZN6mithep'
                if '<' in name:
                    # templated class - can only deal with simple templates
                    op = name.find('<')
                    if name.rfind('<') != op:
                        raise Exception()
    
                    cl = name.find('>')
                    mangled += str(op) + 'INS_' + str(cl - op - 1) + name[op + 1:cl] + 'EE'
    
                else:
                    mangled += str(len(name)) + name

                libdirs = os.environ['LD_LIBRARY_PATH'].split(':')
                for libdir in libdirs:
                    for libname in os.listdir(libdir):
                        if not libname.startswith('libMit') or libname in loaded:
                            continue
    
                        with open(libdir + '/' + libname, 'rb') as lib:
                            cont = lib.read()
                            if cont.find(mangled) < 0 and cont.find('mithep::' + name) < 0:
                                # second condition: typedef names are not mangled
                                continue
                      
                            print '(mithep): Auto-loading library', libname
                            ROOT.gSystem.Load(libname)
                            loaded.append(libname)
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

        gen = _Generator(cls, 'mithep.' + name)

        setattr(self, name, gen)

        return gen

    def loadlib(self, package, module):
        ROOT.gSystem.Load('lib' + package + module + '.so')


class _Analysis(object):
    """
    PyROOT wrapper for mithep::Analysis
    """

    def __init__(self):
        self._core = ROOT.mithep.Analysis()
        self._core.SetKeepHierarchy(False)
        self._sequence = None

    def __getattr__(self, name):
        attr = getattr(self._core, name)
        setattr(self, name, attr)
        return attr

    def setSequence(self, seq):
        self._sequence = seq

    def buildSequence(self):
        headNodes = self._sequence.build()

        for mod in headNodes:
            self._core.AddSuperModule(mod)

    def dumpPython(self):
        iMod = 0
        auxObjects = {}

        code = ''

        for mod in self._sequence:
            for methodName, args in mod.config:
                for arg in args:
                    if not isinstance(arg, Configurable):
                        continue
                    if arg in auxObjects:
                        continue

                    auxName = 'aux' + str(len(auxObjects))
                    code += arg.dumpPython(auxName, auxObjects)
                    auxObjects[arg] = auxName

            modName = 'mod' + str(iMod)
            code += mod.dumpPython(modName, auxObjects)
            iMod += 1

        return code


mithep = _mithep()
analysis = _Analysis()
