#!/bin/sh

PATH_DEV_DMG="/dev/dmg"


echo "Creating DMG symlinks... "

mkdir -p $PATH_DEV_DMG
mkdir -p /dmg/update
mkdir -p /dmg/audio_msg


###############################################################################
# Symlink dispositivo
###############################################################################

  # Alsa Control
  ln -s /usr/bin/play_wav.sh                        $PATH_DEV_DMG/play_wav.sh
  ln -s /usr/bin/rec_wav.sh                         $PATH_DEV_DMG/rec_wav.sh
  # Web control
  ln -s /usr/bin/web_on.sh                          $PATH_DEV_DMG/web_on.sh
  ln -s /usr/bin/web_off.sh                         $PATH_DEV_DMG/web_off.sh

  #Gpio control
  ln -s /sys/class/gpio/arm_active/value            $PATH_DEV_DMG/out_arm_active
  ln -s /sys/class/gpio/arm_enable/value            $PATH_DEV_DMG/in_arm_enable
  ln -s /sys/class/gpio/arm_standby/value           $PATH_DEV_DMG/in_arm_standby

  #serial_port
  ln -s /dev/ttymxc7                                $PATH_DEV_DMG/ttyMSP
