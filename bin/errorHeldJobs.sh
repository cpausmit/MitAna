#!/bin/bash
#===================================================================================================
# Analyze all heald jobs by spitting out the error file
#===================================================================================================
#
function usage {
  echo ""
  echo " usage:  "`basename $0`" <nLines>  [ <nJobs> = all ]"
  echo ""
}

if [ "$1" == "" ]
then
  echo ""
  echo " Please specify at least on parameter."
  echo ""
  usage
  exit 1
fi
NLINES="$1"

if [ "$2" == "" ]
then
  errorFiles=`condor_q -l -constraint "HoldReasonCode==13" | grep ^Err|cut -d \" -f2`
else
  NJOBS="$2"
  errorFiles=`condor_q -l -constraint "HoldReasonCode==13" | grep ^Err|cut -d \" -f2 | head -$NJOBS`
fi

for errorFile in $errorFiles
do

  outputFile=`echo $errorFile | sed "s@.err@.out@"`

  node=`grep 'running on' $outputFile`

  echo ""
  echo "=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="
  echo "ERROR FILE: $errorFile"
  echo " $node"
  echo "=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="
  if [ -z "$NLINES" ]
  then
    cat $errorFile
  else
    head -$NLINES $errorFile
  fi

done