import os
import sys
import ROOT
from configurable import Configurable
from cppnamespace import CppNamespace

ROOT.gROOT.SetBatch(True)
CppNamespace.loadlib('libMitAnaTreeMod.so')

class _Analysis(Configurable):
    """
    PyROOT wrapper for mithep::Analysis
    """

    def __init__(self):
        Configurable.__init__(self, ROOT.mithep.Analysis, 'mithep.Analysis')
        self._sequence = None
        self._outputMods = []
        self.isRealData = False
        self.book = ''
        self.dataset = ''
        self.custom = {}

    def setSequence(self, seq):
        self._sequence = seq

    def AddOutputMod(self, outputMod):
        # need to wrap this function to properly catch the outputMod in the sequence
        self._outputMods.append(outputMod)

    def buildSequence(self):
        for outputMod in self._outputMods:
            self._sequence += outputMod

        modNames = []
        for mod in self._sequence:
            if mod._name in modNames:
                print 'Multiple modules with name ' + mod._name + ' in sequence.'
                sys.exit(1)
                
            modNames.append(mod._name)

        self._sequence.build(nodelist = [])

        for mod in self._sequence.headNodes:
            self.AddSuperModule(mod)

    def reset(self, *args, **kwargs):
        if self._sequence:
            # reset isBuilt status of all modules
            self._sequence.unbuild()

        Configurable.reset(self, *args, **kwargs)

        self._sequence = None
        self._outputMods = []
        self.isRealData = False
        self.custom = {}

    def dumpPython(self, varName = 'analysis', withCtor = True, objects = {}):
        if not self._sequence.isBuilt:
            self.buildSequence()

        code = ''
        modules = {}
        for mod in self._sequence:
            modules[mod] = mod._name

        objects.update(modules)

        # module names are guaranteed to be unique from buildSequence()

        for mod in self._sequence:
            code += mod.dumpPython(varName = mod._name, objects = objects)
            code += '\n'

        for mod in self._sequence:
            for nextNode in mod._nextNodes:
                code += mod._name + '.Add(' + nextNode._name + ')\n'

        code += '\n'

        code += Configurable.dumpPython(self, varName = 'analysis', withCtor = withCtor, objects = modules)

        return code


ROOT.gSystem.Load('libMitAnaTreeMod.so')
mithep = CppNamespace(ROOT.mithep, 'mithep')
mithep.loadedLibs.append('libMitAnaTreeMod.so')

analysis = _Analysis()
