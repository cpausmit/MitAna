#!/bin/bash
#---------------------------------------------------------------------------------------------------
# Request one file to be downloaded using SmartCache queues or through local download with xrdcp. 
#
#                                                                           C.Paus (V0 Apr xx, 2014)
#---------------------------------------------------------------------------------------------------
h=`basename $0`
# start waiting time now
startTime=$(date +%s)

file="$1"

if [ -e "$file" ]
then
  echo " $h - Found file: $file. EXIT!"
  exit 0
fi

localopt=""
if [ "$2" = "-L" ]
then
  localopt="link"
elif [ "$2" = "-C" ]
then
  localopt="copy"
fi

filename=`basename $file`
dirName=`dirname $file`

dataset=`basename $dirName`
dirName=`dirname $dirName`

version=`basename $dirName`
dirName=`dirname $dirName`

book=`basename $dirName`

lfdir=/store/user/paus/$book/$version/$dataset
lfn=$lfdir/$filename

cache=/mnt/hadoop/cms$lfn

if [ -e "/usr/local/DynamicData/SmartCache/setup.sh" ]
then
  if ( [ "$localopt" ] && [ ! -e $cache ] ) || [[ $file =~ ^/mnt/hadoop/cms ]]
  then
    # first condition = Copy from hadoop cache requested but the cache does not exist
    # -> for this job will use xrdcp but at the same time request a cache download
    # second condition = Hadoop cache requested. Submit a download request regardless of
    # the cache existence. Download will not happen if the cache exists.

    # do not ask me why I do this! PYTHON MADNESS
    user=$USER; unset -v `env | sed -e 's/=.*//'`; export USER=$user; export PATH=/bin:/usr/bin
    source /home/$user/.bashrc
    # worst patch in a long time #
  
    source /usr/local/DynamicData/SmartCache/setup.sh
    export PATH="${PATH}:$SMARTCACHE_DIR/Client"
  
    echo " Making request:"
    echo " -> addDownloadRequest.py --file=$filename --dataset=$dataset --book=$book/$version"
    addDownloadRequest.py --file=$filename --dataset=$dataset --book=$book/$version
    rc="$?"

    if [ "$localopt" != "copy" ]
    then
      exit $rc
    fi
  fi
fi

if ! [[ $file =~ ^\./ ]]
then
  echo " $h - SmartCache not available or not requested, but the destination is not local. EXIT!"
  exit 1
fi

destdir=.$lfdir

mkdir -p $destdir

if [ "$localopt" = "link" ]
then
  if [ -e $cache ]
  then
    ln -s $cache $file
    exit $?
  else
    echo " $h - Link to cache requested, but the cache does not exist. Proceeding with download."
  fi
elif [ "$localopt" = "copy" ] && [ -e $cache ]
then
  hdfs dfs -get $cache $file &
  exit 0
fi

server="xrootd.cmsaf.mit.edu"
echo " $h - SmartCache not available or not requested.. trying xrootd cp (xrdcp)."
echo " -> xrdcp from $server to ./store/user/paus/$book/$version/$dataset/$filename"
( xrdcp -s root://${server}/$lfn $file.xrdcp && mv $file.xrdcp $file ) &

exit 0
