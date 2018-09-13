#!/bin/bash
echo -n "DMG LAN ===> ON " >> /tmp/dmg_node_main.log
/sbin/ifconfig eth0 up
echo  "<=== DONE " >> /tmp/dmg_node_main.log
