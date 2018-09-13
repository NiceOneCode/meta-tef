#!/bin/sh

WEBMAIN="/home/root/TEF/main.js"

if [ -f "$WEBMAIN" ]
then
    node "${WEBMAIN}" &
else
	echo "File : ${WEBMAIN} not found"
fi

exit 0

