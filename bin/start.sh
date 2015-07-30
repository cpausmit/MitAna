#!/bin/bash

# Need this wrapper since system default python is 2.6 while 2.7 is used in CMSSW

BOOK=$1
DATASET=$2
FILESET=$3
JSON=$4

if ! [ -d /cvmfs/cms.cern.ch ] || ! [ -d /cvmfs/cvmfs.cmsaf.mit.edu ]
then
  echo "CVMFS not visible from "$(hostname)
  exit 1
fi

source /cvmfs/cms.cern.ch/cmsset_default.sh
source env.sh

env

scram project CMSSW $CMSSW_RELEASE
tar xzf *.lib.tar.gz -C $CMSSW_RELEASE
tar xzf *.inc.tar.gz -C $CMSSW_RELEASE
tar xzf *.MitAna-bin.tar.gz -C $CMSSW_RELEASE

export MIT_CATALOG=catalog

echo $HOSTNAME

eval $(cd $CMSSW_RELEASE; scram runtime -sh)

ls -l

if [ "$JSON" ] && [ "$JSON" != "~" ]
then
  JSONARG="--goodlumi $JSON"
fi

echo "./analysis.py sequence.py --flat --book=$BOOK --dataset=$DATASET --fileset=$FILESET --output=${FILESET}.root --nentries=-1 $JSONARG"
echo ""
./analysis.py sequence.py --flat --book=$BOOK --dataset=$DATASET --fileset=$FILESET --output=${FILESET}.root --nentries=-1 $JSONARG
