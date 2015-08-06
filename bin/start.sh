#!/bin/bash

# Need this wrapper since system default python is 2.6 while 2.7 is used in CMSSW

BOOK=$1
DATASET=$2
FILESET=$3
NENTRIES=$4 # usually empty; set in pilot jobs

if ! [ -d /cvmfs/cms.cern.ch ] || ! [ -d /cvmfs/cvmfs.cmsaf.mit.edu ]
then
  echo "CVMFS not visible from "$(hostname)
  exit 1
fi

if [ -e x509up ]
then
  export X509_USER_PROXY=x509up
fi

source /cvmfs/cms.cern.ch/cmsset_default.sh
source env.sh

scram project -n $CMSSW_NAME CMSSW $CMSSW_RELEASE
tar xzf $CMSSW_NAME.tar.gz -C $CMSSW_RELEASE

echo $HOSTNAME

eval $(cd $CMSSW_NAME; scram runtime -sh)

$CMSSW_NAME/src/MitAna/bin/setupExternal.sh

env

ls -l
ls -l $CMSSW_NAME/lib/$SCRAM_ARCH

python run.py $FILESET $NENTRIES

ls -l
