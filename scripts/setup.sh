#!/bin/bash
# $Id: setup.sh,v 1.88 2013/10/26 17:41:22 bendavid Exp $

if test -z $CMSSW_VERSION; then
    echo "Need cmssw project area setup!";
    exit 1;
fi

if test -z "$CVSROOT"; then
    export CVSROOT=:pserver:anonymous@cmscvs.cern.ch:/cvs_server/repositories/CMSSW
    if test -z "`grep anonymous@cmscvs.cern.ch ~/.cvspass`"; then
        echo "You must have a valid cvs password for user anonymous@cmscvs.cern.ch"
        echo "The password (taken from the workbook) to be used in the next line is 98passwd"
        cvs login 
    fi
fi

majver=`echo $CMSSW_VERSION | cut -d_ -f 2`;
minver=`echo $CMSSW_VERSION | cut -d_ -f 3`;
patver=`echo $CMSSW_VERSION | cut -d_ -f 4`;
majver=`expr $majver \* 1000000`;
minver=`expr $minver \* 1000`;
version=`expr $majver + $minver`;
version=`expr $version + $patver`;

echo
echo "Setup called for $CMSSW_VERSION ($version)";
echo

cd $CMSSW_BASE/src;

if test $version -lt 5004000 -a $version -ge 5003000; then

  # first git clone
  git clone -b hggpaperV6 https://github.com/bendavid/GBRLikelihood.git HiggsAnalysis/GBRLikelihood

  # all the translated cvs one: make sure:
  # export CVSROOT=":ext:${USER}@lxplus5.cern.ch:/afs/cern.ch/user/c/cvscmssw/public/CMSSW"
  cvs co -r V05-08-21    RecoEcal/EgammaCoreTools  
  # not protected pointer of recHits (very often they are not there)
  patch  RecoEcal/EgammaCoreTools/src/EcalClusterLazyTools.cc MitAna/scripts/EcalClusterLazyTools.cc.patch
  cvs co -r EcalLocalFix RecoEgamma/EgammaTools

  # for version 032 (special Mono Release)
  cvs co -r V00-00-13-01 RecoMET/METFilters
  # For CSC Beam Halo filter
  cvs co -r V00-00-08 RecoMET/METAnalyzers

  # For the HBHE filter
  cvs co -r V00-03-23 CommonTools/RecoAlgos

  # Additional packages for the tracking POG filters
  cvs co -r V01-00-11-01 DPGAnalysis/Skims
  cvs co -r V00-11-17    DPGAnalysis/SiStripTools
  cvs co -r V00-00-08    DataFormats/TrackerCommon
  cvs co -r V01-09-05    RecoLocalTracker/SubCollectionProducers

  # seems not to work anymore as addpkg
  ## checkdeps -a
  git cms-checkdeps -a

  ## for version 032 (special Mono Release)
  #git cms-addpkg RecoMET/METFilters
  ## For CSC Beam Halo filter
  #git cms-addpkg RecoMET/METAnalyzers
  #
  ## For the HBHE filter
  #git cms-addpkg CommonTools/RecoAlgos
  #
  ## Additional packages for the tracking POG filters
  #git cms-addpkg DPGAnalysis/Skims
  #git cms-addpkg DPGAnalysis/SiStripTools
  #git cms-addpkg DataFormats/TrackerCommon
  #git cms-addpkg RecoLocalTracker/SubCollectionProducers
  #
  #git cms-checkdeps -a
fi

if test $version -lt 5003000 -a $version -ge 5002000; then
  #support GBRForest for classification, and make compatible with GBRTrain
  addpkg CondFormats/EgammaObjects V00-04-00
  addpkg RecoEcal/EgammaCoreTools  V05-08-20
  addpkg RecoEgamma/EgammaTools    GlobePFDevPatch1
  checkdeps -a
fi

if test $version -lt 4005000 -a $version -ge 4004000; then

  # add packages for clustered PFMET
  cvs co -r AlbertoAndPedro_Sep20_FastJetDevelopments -d RecoParticleFlow/PostProcessing UserCode/RecoParticleFlow/PostProcessing
  cvs co -r v11_09_28 -d CommonTools/ClusteredPFMetProducer UserCode/psilva/ClusteredPFMetProducer

  #patch silly bug...
  patch -p0 -u -N -i MitAna/scripts/ClusteredPFMetProducer.cc.patch -r /dev/null

  #regression and photonfix corrections
  cvs co -r regression_Oct11 HiggsAnalysis/HiggsToGammaGamma

  # RecoTauTag package with bug fix
  addpkg RecoTauTag/RecoTau       V01-02-16
  addpkg RecoTauTag/Configuration V01-02-12

  # RecoTauTag package recommended for 2012
  addpkg DataFormats/TauReco    CMSSW_5_2_4 # yes, this is correct
  addpkg RecoTauTag/TauTagTools CMSSW_5_2_4
  cvs co -r V01-04-17 RecoTauTag/RecoTau
  cvs co -r V01-04-03 RecoTauTag/Configuration


  #fix HitPattern accessor issue in 44x
  addpkg DataFormats/TrackReco
  patch -p0 -u -N -i MitAna/scripts/HitPattern.cc.patch -r /dev/null

  checkdeps -a
fi



case $version in
    (3003006) 
        ;;
    *) 
        #echo "Nothing known about this version, exiting";
        #exit 1;
    ;;
esac

if test -z $MIT_VERS; then
    echo
    echo "Warning: Production setup not executed since \$MIT_VERS is not setup."
    echo
else
    echo
    echo "Entering production setup. Looking for MIT_VERS: $MIT_VERS dependencies."
    echo
fi

echo "Setup done; you probably want to compile your project area now";
