#!/bin/sh

FILE_EXIST="/dmg/audio_msg/playning"

if [ $# -ne 2 ]
  then
    echo "No arguments supplied,invalid number of input argument."
    echo " play_wav.sh namefile.wav duration_in_sec "
    exit -1
fi
if [ -z "$1" ]
  then
    echo "No argument for the namefile.wav supplied,argument is an empty."
    echo "play_wav.sh namefile.wav duration_in_sec "
    exit -2
fi

if [ -z "$2" ]
  then
    echo "No Duration_in_Sec argument supplied, argument is an empty."
    echo "play_wav.sh namefile.wav duration_in_sec "
    exit -3
fi


FILEPLAY=$1
DURATION=$2

if [ -f "$FILEPLAY" ]
then
  rm -f $FILE_EXIST
  sync
  echo 1 > $FILE_EXIST
  sync

	amixer sset Mic 0%
	amixer sset Headphone 100%
  aplay $FILEPLAY -d $DURATION

  rm -f $FILE_EXIST
  sync
else
	echo "File : ${FILEPLAY} not found"
fi

exit 0
