#!/bin/bash

# Need this wrapper since system default python is 2.6 while 2.7 is used in CMSSW

BOOK=$1
DATASET=$2
FILESET=$3

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

scram project CMSSW $CMSSW_RELEASE
tar xzf *.lib.tar.gz -C $CMSSW_RELEASE
tar xzf *.inc.tar.gz -C $CMSSW_RELEASE
tar xzf *.MitAna-bin.tar.gz -C $CMSSW_RELEASE

export MIT_CATALOG=catalog

echo $HOSTNAME

eval $(cd $CMSSW_RELEASE; scram runtime -sh)

env

ls -lR

echo "python $BOOK/$DATASET/run.py $FILESET"
python $BOOK/$DATASET/run.py $FILESET
