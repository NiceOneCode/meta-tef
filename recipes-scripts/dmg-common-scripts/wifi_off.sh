#!/bin/bash
echo -n "DMG WIFI ===> OFF " >> /tmp/dmg_node_main.log
iwpriv wlan0 AP_BSS_STOP | tee -a /tmp/dmg_node_main.log
echo  "<=== DONE " >> /tmp/dmg_node_main.log
