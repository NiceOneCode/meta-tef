#!/bin/bash
echo "DMG WIFI ===> ON " >> /tmp/dmg_node_main.log
iwpriv wlan0 AP_BSS_START
echo  " <=== DONE " >> /tmp/dmg_node_main.log
