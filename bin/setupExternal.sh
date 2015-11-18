#!/bin/bash
#---------------------------------------------------------------------------------------------------------------------
#
# When CMSSW is setup in the condor area the external libraries are also copied but need to be setup with scram so
# they appear in the right places for linking. We can assume that when this script this called CMSSW is already setup.
#
#                                                                                            C.Paus, V0 (Mar 05, 2014)
#                                                                                          Y.Iiyama, V1 (Jul 13, 2015)
#---------------------------------------------------------------------------------------------------------------------

#---------------------
# SET PARAMETERS HERE
#
# PACKAGES TO INSTALL
#PACKAGES="fastjet fastjet-contrib qjets"
PACKAGES="qjets pwhg_cphto_reweight"

# SPECIFIC PARAMETERS
FJ_VERSION="3.1.0-odfocd"
FJCONTRIB_VERSION="1.014-odfocd"
#---------------------

if ! [ "$CMSSW_BASE" ]
then
  echo "CMSSW_BASE not set"
  exit 1
fi

# Utility function to find the installation of the external
find-external() {
  local PACKAGE=$1
  local EXTERNAL

  # look in CMSSW default first
  for EXTERNAL in \
  /cvmfs/cms.cern.ch/$SCRAM_ARCH/external/$PACKAGE \
  /cvmfs/cvmfs.cmsaf.mit.edu/hidsk0001/cmsprod/cms/external/$PACKAGE
  do
    if [ -d $EXTERNAL ]
    then
      echo $EXTERNAL
      exit 0
    fi
  done

  EXTERNAL="/home/$USER/cms/external/$PACKAGE"
  echo " INFO - making directory at: $EXTERNAL"
  echo ""

  mkdir -p $EXTERNAL
  echo $EXTERNAL
}

### Installer functions

# Install fastjet
install-fastjet() {
  # add local fastjet external to scarm config

  local VERSION=$1
  local FASTJET_BASE=$(find-external fastjet/$VERSION)
  local TARGET=$CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/fastjet.xml

  echo ""
  echo " INFO - using fastjet at: $FASTJET_BASE"
  echo ""

  mv $TARGET ${TARGET}-last.$(date +%s) 2> /dev/null

  echo \
'  <tool name="fastjet" version="'$VERSION'">
    <info url="http://www.lpthe.jussieu.fr/~salam/fastjet/"/>
    <lib name="fastjetplugins"/>
    <lib name="fastjettools"/>
    <lib name="siscone"/>
    <lib name="siscone_spherical"/>
    <lib name="fastjet"/>
    <client>
      <environment name="FASTJET_BASE" default="'$FASTJET_BASE'"/>
      <environment name="LIBDIR" default="$FASTJET_BASE/lib"/>
      <environment name="INCLUDE" default="$FASTJET_BASE/include"/>
    </client>
    <runtime name="ROOT_INCLUDE_PATH" value="$INCLUDE" type="path"/>
    <use name="root_cxxdefaults"/>
  </tool>
' > $TARGET

  # commit the scram config changes
  cd $CMSSW_BASE/src
  scram setup fastjet
}

# Install fastjet-contrib
install-fastjet-contrib() {
  # add local fastjet-contrib external to scarm config

  local VERSION=$1
  local FASTJET_CONTRIB_BASE=$(find-external fastjet-contrib/$VERSION)
  local TARGET=$CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/fastjet-contrib.xml

  echo ""
  echo " INFO - using fastjet-contrib at: $FASTJET_CONTRIB_BASE"
  echo ""
  
  mv $TARGET ${TARGET}-last.$(date +%s) 2> /dev/null

  echo \
'  <tool name="fastjet-contrib" version="'$VERSION'">
    <info url="http://fastjet.hepforge.org/contrib/"/>
    <lib name="fastjetcontribfragile"/>
    <client>
      <environment name="FASTJET_CONTRIB_BASE" default="'$FASTJET_CONTRIB_BASE'"/>
      <environment name="LIBDIR" default="$FASTJET_CONTRIB_BASE/lib"/>
      <environment name="INCLUDE" default="$FASTJET_CONTRIB_BASE/include"/>
    </client>
  </tool>
' > $TARGET

  # commit the scram config changes
  cd $CMSSW_BASE/src
  scram setup fastjet-contrib
}

# Install qjets
install-qjets() {
  # add local fastjet-contrib external to scarm config
  # Qjets itself is not versioned, but since it is compiled against fastjet libraries
  # we use the fastjet version to tag the packages

  local VERSION=$1
  local QJETS_BASE=$(find-external Qjets/$VERSION)
  local TARGET=$CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/qjets.xml

  echo ""
  echo " INFO - using qjets at: $QJETS_BASE"
  echo ""

  mv $TARGET ${TARGET}-last.$(date +%s) 2> /dev/null

  echo \
'  <tool name="qjets" version="'$VERSION'">
    <info url="http://jets.physics.harvard.edu/Qjets/html/Welcome.html"/>
    <lib name="qjets"/>
    <client>
      <environment name="QJETS_BASE" default="'$QJETS_BASE'"/>
      <environment name="LIBDIR" default="$QJETS_BASE/lib"/>
      <environment name="INCLUDE" default="$QJETS_BASE"/>
    </client>
  </tool>
' > $TARGET

  # commit the scram config changes
  cd $CMSSW_BASE/src
  scram setup qjets
}

install-pwhg_cphto_reweight() {
  # add local fastjet external to scarm config

  local BASE=$(find-external pwhg_cphto_reweight)
  local TARGET=$CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/pwhg_cphto_reweight.xml

  echo ""
  echo " INFO - using pwhg_cphto_reweight at: $BASE"
  echo ""

  mv $TARGET ${TARGET}-last.$(date +%s) 2> /dev/null

  echo \
'  <tool name="pwhg_cphto_reweight" version="1">
    <lib name="cphto"/>
    <client>
      <environment name="LIBDIR" default="'$BASE'"/>
    </client>
  </tool>
' > $TARGET

  # commit the scram config changes
  cd $CMSSW_BASE/src
  scram setup pwhg_cphto_reweight
}

### Loop over PACKAGES

for PACKAGE in $PACKAGES
do
  case $PACKAGE in
    fastjet)
      install-fastjet $FJ_VERSION
      ;;
    fastjet-contrib)
      install-fastjet-contrib $FJCONTRIB_VERSION
      ;;
    qjets)
      install-qjets $FJ_VERSION
      ;;
    pwhg_cphto_reweight)
      install-pwhg_cphto_reweight
      ;;
    *)
      echo "Unknown external $PACKAGE"
      exit 1
      ;;
  esac
done
