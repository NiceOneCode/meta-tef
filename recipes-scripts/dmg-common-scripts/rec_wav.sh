#!/bin/sh

if [ $# -ne 1 ]
  then
    echo "No arguments supplied,invalid number of input argument."
    exit -1
fi
if [ -z "$1" ]
  then
    echo "No argument supplied,argument is an empty."
    exit -2
fi

FILEDEST=$1

if [ -f "$FILEDEST" ]
then
	rm $FILEDEST
fi
amixer cset numid=2 15
amixer cset numid=9 1
amixer sset Mic 100%
arecord -d 10 -f cd -t wav $FILEDEST

exit -0
