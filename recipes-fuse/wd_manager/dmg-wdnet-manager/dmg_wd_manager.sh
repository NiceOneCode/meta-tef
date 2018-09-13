#!/bin/sh

while [ true ]
do
if [ ! -e /dmg/fuse/wd ] ; then
  mkdir -p /dmg/fuse/wd
else
  rm -fr /dmg/fuse/wd/*
fi

/usr/bin/dmg_wd_manager -f -o nonempty -o auto_unmount /dmg/fuse/wd -daemon

# wait zero second and try to restart
sleep 0

done
