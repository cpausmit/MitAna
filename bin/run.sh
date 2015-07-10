#!/bin/bash
#---------------------------------------------------------------------------------------------------
# Execute one job (works interactively and when executed in condor)
#---------------------------------------------------------------------------------------------------
  runMacro=$1
catalogDir=$2
      book=$3
   dataset=$4
      skim=$5
   fileset=$6
outputName=$7
 outputDir=$8
 runTypeId=$9
   nEvents=-1
if [ ".${10}" != "." ]
then
  nEvents=${10}
fi

h=`basename $0`
echo "Script:    $h"
echo "Arguments: $*"

# some basic printing
echo " ";
echo "${h}: Show who and where we are";
echo " start time    : "`date`
echo " user executing: "`id`;
echo " running on    : "`hostname`;
echo " executing in  : "`pwd`;
echo " submitted from: $HOSTNAME";
echo " ";
env

initialSeconds=`date +"%s"`

# initialize the CMSSW environment
echo " "; echo "${h}: Initialize CMSSW (in $CMSSW_BASE)"; echo " "

# make sure to setup what we need locally if we are not in the original area (when in condor)
workDir=`pwd`
ls -alhrt
if [ "$workDir" != "/home/$USER/cms/root" ]
then
  export VER=`echo CMSSW_*[0-9].tgz |cut -d. -f1`
  source /cvmfs/cms.cern.ch/cmsset_default.sh
  scram project CMSSW ${VER}

  echo " untarring: ${VER}.tgz"
  tar fzx ${VER}.tgz
  echo " untarring: ${VER}-src.tgz"
  tar fzx ${VER}-src.tgz
  ls -lhrt ${VER}/

  echo " setting up CMSSW environment"
  cd ${VER}/src
  eval `scram runtime -sh`
  cd - >& /dev/null

  echo " setting up MIT_PROD environment"
  source ./setup.sh
  env | grep MIT_
  env | grep EXTERNAL

  if [ $EXTERNAL != "/cvmfs/cvmfs.cmsaf.mit.edu/hidsk0001/cmsprod/cms/external" ]
  then
    echo "  untarring: external.tgz"
    tar fzx external.tgz
    $CMSSW_BASE/src/MitAna/bin/setupExternal.sh
    export EXTERNAL=./external
  fi

  echo "  untarring: json.tgz"
  tar fzx json.tgz

  echo "  untarring: catalog.tgz"
  tar fzx catalog.tgz

  echo "  found MitPhysics/data at: $MIT_DATA"
else
  echo " Everything is ready already. Let's go!"
fi
ls -lhrt $CMSSW_BASE/src/MitPhysics

# take care of the certificate
if [ -e "./x509up_u`id -u`" ]
then
  export X509_USER_PROXY="./x509up_u`id -u`"
fi
echo " INFO -- using the x509 ticket: $X509_USER_PROXY"

# get ready to run
echo " "; echo "${h}: Starting root job now"; echo " ";
echo \
  root -b -l -q -n rootlogon.C \
  ./${runMacro}+\(\"$fileset\",\"$skim\",\"$dataset\",\"$book\",\"$catalogDir\"\,\"$outputName\",$nEvents\)

  root -b -l -q -n rootlogon.C \
  ./${runMacro}+\(\"$fileset\",\"$skim\",\"$dataset\",\"$book\",\"$catalogDir\"\,\"$outputName\",$nEvents\)

# get the return code from the root job
status=`echo $?`
echo "${h}: Status - $status"

finalSeconds=`date +"%s"`
let duration=($finalSeconds-$initialSeconds)/60
echo " end time      : "`date`
echo " duration      : $duration min"

exit $status
