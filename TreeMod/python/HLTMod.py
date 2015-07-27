from MitAna.TreeMod.bambu import mithep, analysis

hltMod = mithep.HLTMod(
    BitsName = mithep.Names.gkHltBitBrn,
    AbortIfNotAccepted = analysis.isRealData,
    PrintTable = False
)
