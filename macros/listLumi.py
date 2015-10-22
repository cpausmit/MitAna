from MitAna.TreeMod.bambu import mithep, analysis

listMod = mithep.RunLumiListMod()

# RunLumiSelectionMod will be applied automatically from the command-line option to analysis.py / runOnDatasets.py

analysis.setSequence(listMod)

analysis.SetUseReadCache(False)
