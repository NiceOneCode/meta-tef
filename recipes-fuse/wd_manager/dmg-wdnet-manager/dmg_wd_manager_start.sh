#!/bin/sh

sh /usr/bin/daemon_controller.sh "start" "dmg_wd_manager"

PATH_DEV_DMG="/dev/dmg"


echo "Creating DMG WD symlinks... "

mkdir -p $PATH_DEV_DMG

###############################################################################
# Symlink dispositivo
###############################################################################

  # WD Control
  ln -s /dmg/fuse/wd/wd_start                        $PATH_DEV_DMG/wd_start
  ln -s /dmg/fuse/wd/wd_stop                         $PATH_DEV_DMG/wd_stop
  ln -s /dmg/fuse/wd/wd_alive                        $PATH_DEV_DMG/wd_alive
  ln -s /dmg/fuse/wd/wd_countdown                    $PATH_DEV_DMG/wd_countdown
