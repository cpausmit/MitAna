import sys
import os
import ROOT

fileset = sys.argv[1]
sources = sys.argv[2:]

if not os.path.exists(fileset + '.root') or os.path.getsize(fileset + '.root') == 0:
    sys.exit(1)

target = ROOT.TFile.Open(fileset + '.root', 'update')

for sname in sources:
    source = ROOT.TFile.Open(sname)
    
    trees = set()
    
    for key in source.GetListOfKeys():
        obj = key.ReadObj()
        if obj.InheritsFrom(ROOT.TTree.Class()):
            trees.add(obj.GetName())
            continue

        target.cd()
        newobj = obj.Clone()
        newobj.Write()
    
    # clone only the last version
    for tree in trees:
        target.cd()
        clonetree = source.Get(tree).CloneTree(-1, 'fast')
        clonetree.Write()
