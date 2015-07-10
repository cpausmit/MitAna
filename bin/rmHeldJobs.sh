#!/bin/bash
#===================================================================================================
# Analyze all heald jobs by spitting out the error file
#===================================================================================================
#
function usage {
  echo ""
  echo " usage:  "`basename $0`" [ <nJobs> = all ]"
  echo ""
}

if [ "$1" == "" ]
then
  clusterIds=`condor_q -l -constraint "HoldReasonCode==13"|grep ^ClusterId|cut -d ' ' -f3`
else
  NJOBS="$2"
  clusterIds=`condor_q -l -constraint "HoldReasonCode==13"|grep ^ClusterId|cut -d ' ' -f3|head -$NJOBS`
fi

for clusterId in $clusterIds
do

  echo " remove job with Id: $clusterId"
  condor_rm $clusterId

done