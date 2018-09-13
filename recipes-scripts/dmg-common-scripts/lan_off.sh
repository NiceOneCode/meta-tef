#!/bin/bash
echo -n "DMG LAN ===> OFF " >> /tmp/dmg_node_main.log
/sbin/ifconfig eth0 down
echo  "<=== DONE " >> /tmp/dmg_node_main.log
