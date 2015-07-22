#!/bin/bash
#----------------------------------------------------------------------------------------------
# Setup the MitAna package adjusting things that are needed for it to compile and run properly.
#
#                                                                   Apr 17, 2015 - V1 Y. Iiyama
#----------------------------------------------------------------------------------------------

echo "*********************"
echo " MitAna/bin/setup.sh"
echo "*********************"

$CMSSW_BASE/src/MitAna/bin/setupExternal.sh

# Generate ROOT dictionaries for classes defined in this module
$CMSSW_BASE/src/MitCommon/bin/genDict.sh MitAna/{Catalog,DataCont,DataTree,DataUtil,PhysicsMod,PhysicsUtils,TAM,TreeFiller,TreeMod,Utils,Validation}
