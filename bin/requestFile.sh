#!/bin/bash
#---------------------------------------------------------------------------------------------------
# Request one file to be downloaded using SmartCache queues or through local download with xrdcp. 
# Follow the comment lines for different cases:
# Case 1. $file = /mnt/hadoop/cms/store/...
# Case 2. $file = ./store/... + local option (-L or -C)
# Case 3. $file = ./store/...
#                                                                           C.Paus (V0 Apr xx, 2014)
#                                                                         Y.Iiyama (V1 Aug 15, 2015)
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

# Status:
# Case 1: Cache does not exist
# Case 2 & 3: Local copy does not exist

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

if [ "$localopt" = "link" ] && [ ! -e $cache ]
then
  echo " $h - Link to cache requested, but the cache does not exist. Proceeding with download."
  localopt=copy
fi

if ( [ $file = $cache ] || [ "$localopt" ] ) && [ ! -e $cache ]
then
  # case 1 or 2 and cache does not exist
  if [ -e "/usr/local/DynamicData/SmartCache/setup.sh" ]
  then
    # do not ask me why I do this! PYTHON MADNESS
    env | sed 's/^.*/export &/' > _requestFile_sh_env
    user=$(id -un); unset -v `env | sed -e 's/=.*//'`; export USER=$user; export PATH=/bin:/usr/bin
    source /home/$user/.bashrc
    # worst patch in a long time #
  
    source /usr/local/DynamicData/SmartCache/setup.sh
    export PATH="${PATH}:$SMARTCACHE_DIR/Client"
  
    echo " Making request:"
    echo " -> addDownloadRequest.py --file=$filename --dataset=$dataset --book=$book/$version"
    addDownloadRequest.py --file=$filename --dataset=$dataset --book=$book/$version
    rc="$?"
  
    if [ "$localopt" ]
    then
      echo " Download request made. Now obtaining a local copy for this job.."
    else
      rm _requestFile_sh_env
      exit $rc
    fi
  else
    echo " $h - Cache $cache requested but SmartCache is not available. EXIT!"
    exit 1
  fi
fi

# Status:
# Case 1: Closed. Caller should wait for the cache to arrive.
# Case 2 & 3: Local copy does not exist.

if ! [[ $file =~ ^\./ ]]
then
  echo " $h - SmartCache not requested, but the destination is not local. EXIT!"
  exit 1
fi

if [ -e _requestFile_sh_env ]
then
  # recover the environment variables that were unset
  unset -v `env | sed -e 's/=.*//'`
  
  source _requestFile_sh_env
  rm _requestFile_sh_env
fi

destdir=.$lfdir

mkdir -p $destdir

if [ "$localopt" ] && [ -e $cache ]
then
  echo " $h - Using cache $cache"
  if [ "$localopt" = "link" ]
  then
    ln -s $cache $file
    exit $?
  elif [ "$localopt" = "copy" ]
  then
    hdfspath=$(echo $cache | sed 's#/mnt/hadoop##')
    if [[ $(hostname) =~ .*cmsaf.mit.edu ]]
    then
      cmd="hadoop dfs -get"
    else
      cmd="hdfs dfs -fs hdfs://t3serv002.mit.edu:9000 -get"
    fi
    ( itry=0; while [ $itry -lt 10 ] && [ ! -e $file ]; do $cmd $hdfspath $file; itry=$(($itry+1)); done ) &
#    ( cp $cache ${file}.copy && mv ${file}.copy $file ) &
    exit 0
  fi
fi

# Case 2: Neither local copy nor cache exists.
# Case 3: Local copy does not exist.

server="xrootd.cmsaf.mit.edu"
echo " $h - xrdcp from $server to ./store/user/paus/$book/$version/$dataset/$filename"
( for try in {1..20}; do echo "xrdcp attempt $try"; \
    xrdcp -s root://${server}/$lfn $file.xrdcp; stat=$?; \
    if [ $stat -eq 0 ]; then mv $file.xrdcp $file && break; else rm -f $file.xrdcp; fi; \
    done ) &
# retry 20: ~< 1 minute per try, Cacher wait timeout is 20 minutes -> trying more than 20 times is useless

exit 0
