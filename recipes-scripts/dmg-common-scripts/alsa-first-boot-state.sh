#!/bin/bash

# this script fix the error:
#
# ALSA: Restoring mixer settings...
# No state is present for card imx6qicoresgtl5
# Found hardware: "imx6q-icore-sgt" "" "" "" ""
# Hardware is initialized using a generic method
# No state is present for card imx6qicoresgtl5


# If is the first boot of the system the alsa state will be saved
# and at the end of this file will be written "#OK" 
# For all nex boot this file is useless
if  [ "$(tail -n 1 /etc/init.d/alsa-first-boot-state.sh)" != '#OK' ]; then
    /usr/sbin/alsactl -f /var/lib/alsa/asound.state store
    echo -e '#OK' >> /etc/init.d/alsa-first-boot-state.sh  
fi

