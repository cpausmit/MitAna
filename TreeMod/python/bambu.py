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

    def connect(self, nextNode):
        Node.connect(self, nextNode)
        self._cppobj.Add(nextNode._cppobj)


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


class _Namespace(object):
    """
    PyROOT wrapper for generic C++ namespace.
    """

    def __init__(self, namespace, namespaceName, superspaces = []):
        self._core = namespace
        self._name = namespaceName
        self._superspaces = superspaces
        self.loadedLibs = []

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
                        if not libname.startswith('libMit') or libname in self.loadedLibs:
                            continue
    
                        with open(libdir + '/' + libname, 'rb') as lib:
                            cont = lib.read()
                            if cont.find(mangled) < 0 and cont.find(cppname) < 0:
                                # typedefs are written directly with cppname
                                continue

                            print '(mithep): Auto-loading library', libname
                            ROOT.gSystem.Load(libname)
                            self.loadedLibs.append(libname)
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

        if hasattr(cls, 'Class'):
            # this is a TObject

            clsName = ''
            for sup in self._superspaces:
                clsName += sup + '.'
            clsName += self._name + '.' + name

            ret = _Generator(cls, clsName)
        elif issubclass(type(cls), ROOT.PyRootType):
            # this is a namespace
            ret = _Namespace(cls, name, self._superspaces + [self._name])
        else:
            # this is a simple variable
            ret = cls

        setattr(self, name, ret)

        return ret

    def loadlib(self, libName):
        ROOT.gSystem.Load(libName)
        self.loadedLibs.append(libName)


class _Analysis(object):
    """
    PyROOT wrapper for mithep::Analysis
    """

    def __init__(self):
        self._core = ROOT.mithep.Analysis()
        self._sequence = None
        self._keepHierarchy = False

    def __getattr__(self, name):
        attr = getattr(self._core, name)
        setattr(self, name, attr)
        return attr

    def SetKeepHierarchy(self, keep):
        self._keepHierarchy = keep

    def setSequence(self, seq):
        self._sequence = seq

    def buildSequence(self):
        self._sequence.build()

        for mod in self._sequence.headNodes:
            self._core.AddSuperModule(mod._cppobj)

        self._core.SetKeepHierarchy(self._keepHierarchy)

    def dumpPython(self):
        iMod = 0
        auxObjects = {}

        code = 'import ROOT\n'
        code += 'ROOT.gROOT.SetBatch(True)\n'
        for libName in mithep.loadedLibs:
            code += 'ROOT.gSystem.Load(\'' + libName + '\')\n'

        code += '\n'

        modNames = {}
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
            modNames[mod] = modName
            code += mod.dumpPython(modName, auxObjects)
            iMod += 1

        if not self._sequence.isBuilt:
            self._sequence.build()

        for mod in self._sequence:
            for nextNode in mod.nextNodes:
                code += modNames[mod] + '.Add(' + modNames[nextNode] + ')\n'

        code += '\n'

        code += 'analysis = mithep.Analysis()\n'
        code += 'analysis.SetKeepHierarchy(' + str(self._keepHierarchy) + ')\n'
        for mod in self._sequence.headNodes:
            code += 'analysis.AddSuperModule(' + modNames[mod] + ')\n'

        return code


ROOT.gSystem.Load('libMitAnaTreeMod.so')
mithep = _Namespace(ROOT.mithep, 'mithep')
mithep.loadedLibs.append('libMitAnaTreeMod.so')

analysis = _Analysis()
