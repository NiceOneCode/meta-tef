#!/bin/bash
#sleep 5 #sleep to avoid overlapping during startup

mkdir -p /dmg/update
mkdir -p /dmg/backup
mkdir -p /home/root/update-log

#rename the file that comes from the browser with the name and version taken from the properties of the file
PACHAGE_NAME=$(opkg info /dmg/update/update.ipk | grep Package | sed 's/[^ ]* //')
PACHAGE_VERSION=$(opkg info /dmg/update/update.ipk | grep Version | sed 's/[^ ]* //')
mv /dmg/update/update.ipk ${PACHAGE_NAME}_${PACHAGE_VERSION}.ipk


update_package_ipk=($(ls /dmg/update/*.ipk 2>/dev/null))
echo "update_package_ipk: $update_package_ipk"
old_ipk=($(ls /dmg/backup/*.ipk 2>/dev/null))

function check_update_presence() {
	if [[ -e ${update_package_ipk} ]] ; then
		echo "tef-updater--> Update package found in /dmg/update..."

		extension=${update_package_ipk}
		extension=${extension##*.}

		if [ ${extension} == "ipk" ] ; then
			echo "tef-updater--> .ipk file found..."
			return 0
		else
			echo "tef-updater--> The file is not is not an .ipk file"
			echo "tef-updater--> Impossible to update"
			exit 0
		fi

	else
		echo "tef-updater--> No update package found in /dmg/update..."


		echo "tef-updater--> Done"
		exit 0
	fi
}

function progressbar() {
	while [ -e /proc/$PID ]
	do
		sleep 4
		echo -ne "."
	done
	echo ""
}

function is_node_app() {
	test_node_app="node-app"
	if [[ ${update_package_ipk} =~ ${test_node_app} ]] ; then
		return 0	#is node-app
	else
		return 1	#is not node-app
	fi
}

function install_ipk() {
	opkg install ${update_package_ipk}
}

function save_ipk() {
	rm -f /dmg/backup/*.ipk
	mv ${update_package_ipk} /dmg/backup/
}

function restore_backup() {
	opkg remove node-app
	opkg install ${old_ipk}
}

function check_server_status() {
	SERVER_CHECK="bad"
	cd /home/root/TEF
  ( node main.js | tee /dmg/update/node_running.log ) &
  sleep 20
  killall node >/dev/null 2>&1

	SERVER_ACK=$(grep "TEF web server interface started and listening on port 80" /dmg/update/node_running.log)
	echo "first line: $SERVER_ACK"
  if [[ ${SERVER_ACK} == "TEF web server interface started and listening on port 80" ]]; then
		#server ok
		#echo "server ok"
		SERVER_CHECK="ok"
  #else
	#	#server not ok
	#	#echo "server bad"
	#	return 1
  fi
}

################################## MAIN ##################################
function main() {

	is_node_app
	if [ $? -eq 0 ] ; then #if the update file founded is node-app:

	onboard_version=$(cat /home/root/TEF/tef/version | tr -d ' ' )
	echo "tef-updater--> Version installed:${onboard_version}"

	echo "tef-updater--> Installing node-app ipk:"
	install_ipk

	echo -ne "tef-updater--> Checking server status..."
	(check_server_status)&
	#(check_server_status >/dev/null 2>&1)&
	PID=$!
	progressbar

echo "SERVER_CHECK= $SERVER_CHECK"
echo "--------------------------------------------------"
cat /dmg/update/node_running.log
echo "--------------------------------------------------"
		if [[ $SERVER_CHECK == "ok" ]]; then
			echo -ne "tef-updater--> Server ready"
			echo ok > /home/root/TEF/update #"update" contains "ok" if the last upadte as been succesfully installed

			(save_ipk >/dev/null 2>&1)&
  		PID=$!
    	progressbar
		else
			echo "tef-updater--> WARNING: Something went wrong..."
			echo failed > /home/root/TEF/update # "update" contains "failed" if the last upadte as been failed
  		echo -ne "tef-updater--> Restoring backup..."
			restore_backup

		fi

	else #if the update file founded is not node-app package it will be only installed

		echo "tef-updater--> Installing ${update_package_ipk}:"
		install_ipk

	fi

	cd /home/root/update-log
  ls -t *.log | tail -n +6 | xargs rm -rf   #leave only last 3 .log files; do not touch other types of files

	rm -rf /dmg/update/*
	onboard_version=$(cat /home/root/TEF/tef/version | tr -d ' ')
	echo "tef-updater--> TEF version installed:${onboard_version}"
	echo "tef-updater--> Done"
}

#echo "tef-updater--> TEF Updater"

check_update_presence #check presence of "update pachage" in /dmg/update, if true continue else exit

main | tee -a /home/root/update-log/update-${update_package_ipk##*/}-$(date '+%Y-%m-%d_%H:%M').log

exit 0
