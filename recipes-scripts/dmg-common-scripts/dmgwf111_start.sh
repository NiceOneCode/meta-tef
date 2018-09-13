#!/bin/sh

# As client
# sh /usr/bin/dmgwf111_C.sh
# As Access Point

if [ ! -f /home/root/TEF/tef/database/default/armSettingsDefault.ini ]; then
  echo "###################################################################"
  echo "ERROR:"
  echo "file: /home/root/TEF/tef/database/default/armSettingsDefault.ini"
  echo "not found, /home/root/TEF corrupted"
  echo "server can not start"
  echo "restore /home/root/TEF folder by using \"update\" file"
  echo "###################################################################"
  mv /home/root/TEF/main.js /home/root/TEF/main.js-corrupted
fi

if [ ! -f /home/root/TEF/tef/database/local_settings.ini ]; then
  cp /home/root/TEF/tef/database/default/armSettingsDefault.ini    /home/root/TEF/tef/database/local_settings.ini
fi

#setting of "AP_TEF name" by default as "AP_TEF_SX" or by take name in "local_settings.ini"
Nom_Identifiant=$(grep Nom_Identifiant /home/root/TEF/tef/database/local_settings.ini)
Nom_Identifiant=$(echo "${Nom_Identifiant#*\=}")
if [ -z "$Nom_Identifiant" ]; then
  Nom_Identifiant="SX"
fi

#setting of "AP_TEF password" by default as "password" or by take name in "local_settings.ini"
Mot_De_Passe=$(grep Mot_De_Passe /home/root/TEF/tef/database/local_settings.ini)
Mot_De_Passe=$(echo "${Mot_De_Passe#*\=}")
if [ -z "$Mot_De_Passe" ]; then
  Mot_De_Passe="password"
fi

#start ACCESS POINT
sh /usr/bin/dmgwf111.sh "AP_TEF_${Nom_Identifiant}" "$Mot_De_Passe"
