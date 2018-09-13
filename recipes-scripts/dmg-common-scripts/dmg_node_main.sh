#!/bin/bash

IN_ARM_STANDBY_GPIO="/dev/dmg/in_arm_standby"
OUT_ARM_ACTIVE_GPIO="/dev/dmg/out_arm_active"
IN_ARM_STANDBY_GPIO_ISUP=""
PID=0

function in_arm__standby_waiting()
{
	#wait signal from in_arm_standby with debouncing control
	echo "dmg_node_main: in_arm_standby_waiting"
	while true;
	IN_ARM_STANDBY_GPIO_ISUP=0
	do
		if [[ $(cat $IN_ARM_STANDBY_GPIO) == 0 ]]; then
			sleep	0
			sleep	0
			sleep	0
			if [[ $(cat $IN_ARM_STANDBY_GPIO) == 0 ]]; then
				sleep	0
				sleep	0
				sleep	0
				if [[ $(cat $IN_ARM_STANDBY_GPIO) == 0 ]]; then
					IN_ARM_STANDBY_GPIO_ISUP=1
					echo "dmg_node_main: IN_ARM_STANDBY_GPIO_ISUP = 0"
					return 0
				fi
			fi
		fi
		sleep 1
	done
}

function node_death_waiting()
{
	echo "dmg_node_main: node is working"
	#while ps -p $PID > /dev/null
	while [ -e /proc/$PID ]
	do
		sleep 1
	done
	return 0
}

function main()
{
	local wdtest

	DATE=$(date +"%Y"-"%m"-"%d") #date print for log file

	(sh /usr/bin/lan_on.sh)&
	sleep 3
	echo "dmg_node_main: lan_on"
	# echo 1 > $OUT_ARM_ACTIVE_GPIO
	sleep 1

	if [ -f /home/root/TEF/main.js ]; then

		echo "--------------> dmg_node_main: START SERVER <--------------"
		(node /home/root/TEF/main.js)&
		PID=$!

		echo "dmg_node_main: WAITING WIFI"
		#while true;  Si può fare ma si deve cambiare il comportamento del MSP
		#do
			in_arm__standby_waiting
			if [[ $IN_ARM_STANDBY_GPIO_ISUP == 1 ]];
			then
				echo "1" > /dev/dmg/wd_alive
				sleep 1
    		wdtest=$(cat /dev/dmg/wd_start)
    		if [ "$wdtest" == " is Disable" ];
    		then
						(sh /usr/bin/lan_on.sh)&
				fi
				(sh /usr/bin/wifi_on.sh)&
				echo "dmg_node_main: wifi_on"
				sleep 29
			fi
    	sleep 1
		#done

	fi
}


main | tee -a /tmp/dmg_node_main.log
exit 0
