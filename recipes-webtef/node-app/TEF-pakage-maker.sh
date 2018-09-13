#!/bin/bash
#
#   ISTRUZIONI: 
#   eseguire TEF-pakage-maker allo stesso livello di una cartella "TEF"
#
#   .
#   ├── TEF
#   │   ├── build
#   │   ├── heartbeat
#   │   ├── lib
#   │   ├── main.js
#   │   ├── package.json
#   │   ├── tef
#   │   └── uploads
#   └── TEF-pakage-maker.sh
#
#   sarà prodotto un file del tipo:
#   TEF_1.2.8.004.tar.gz

ONBOARD_VERSION=$(cat TEF/tef/version | tr -d ' ' | sed 's/[A-Za-z]*//g')

tar -zcvf TEF_${ONBOARD_VERSION}.tar.gz TEF

exit 0
