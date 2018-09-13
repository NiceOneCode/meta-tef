#!/bin/bash +x
FILE_DRIVER_LOG="/tmp/driver.log"

SSIDNAME="AP_TEST"
TYPE_CONFIG="AP"
IP="192.168.0.20"
TYPE_PW="password"

PSK=""


NEW_SSID=$1
NEW_PASS=$2


function chekSSidPAss ()
{
    local LENGTH=0;
    local PASSTEST=$(echo $NEW_PASS | sed 's/ //g')
    
    if [ "$NEW_SSID" !=  "" ]
    then
        SSIDNAME="${NEW_SSID}"
    fi
    
    if [ "$NEW_PASS" !=  "" ]
    then
        LENGTH=$(echo $PASSTEST | wc -c)
        #		echo "LENGTH = ${LENGTH}"
        if [ $LENGTH -gt 8 ];
        then
            TYPE_PW=$PASSTEST
        fi
    fi
    
    echo "SSIDNAME = ${SSIDNAME}"
    echo "PW = ${TYPE_PW}"
    #	echo "WAS = 5acb375a0820ba8ff0b22fb668436bf25a9216b13a82310875b00fd62e92e36d"
    PSK=$( wpa_passphrase ${SSIDNAME} ${TYPE_PW} | grep "	psk=" | sed s/=/\\n/g | sed '/psk/ d')
    echo "PSK = ${PSK}"
    
    return $out
	
    
    # retval=$?
}





function chekmoduleInstalled ()
{
    out=1
    
	#local linescnt=$(dmesg | grep "unifi1: UniFi ready")
    #if [ "$linescnt" !=  "unifi1: UniFi ready" ]
    #then
    #    echo "chekmoduleInstalled result = NOT INSTALLED"
    #    out=1
    #    # Not Installed
    #else
    #    echo "chekmoduleInstalled result = Installed !!! "
    #    out=1
    #    # Installed
    #    
    #fi
    #return $out
    
    if dmesg | grep -q "unifi1: UniFi ready"
    then
        echo "chekmoduleInstalled result = Installed !!! "
        out=0
        # Installed
    else
        echo "chekmoduleInstalled result = NOT INSTALLED"
        out=1
        # Not Installed
    fi
    return $out
    
    # retval=$?
}

function installModule ()
{
    out=1
    
    local linescnt=$(lsmod | grep "unifi_sdio")
    if [[ $linescnt == unifi_sdio* ]]
    then
        echo "unifi_sdio module loded"
    else
        out=$(insmod "/lib/modules/$(uname -r)/extra/unifi_sdio.ko")
    fi
    return $out
}

function interface_enable ()
{
    local linescnt=$(iwconfig | grep "Access Point: Not-Associated")
    if [ "$linescnt" !=  "" ]
    then
        echo "interface_enable result = ASSOCIATED"
        out=0
    else
        echo "interface_enable result = NOT ASSOCIATED !!!"
        out=1
    fi
    return $out
    
    # retval=$?
}


function configure_AP ()
{
    
    ifconfig wlan0 "${IP}"
    udhcpd
    ## -I wlan0 "/etc/udhcpd.conf"
    
    if [ "$TYPE_PW" ==  "" ]
    then
        echo "AP no Password "
        iwpriv wlan0 AP_SET_CFG ASCII_CMD=AP_CFG,SSID="${SSIDNAME}",SEC="open",KEY="",CHANNEL=1,PREAMBLE=1,MAX_SCB=2,END
    else
        echo "AP with Passoword "
        # iwpriv wlan0 AP_SET_CFG ASCII_CMD=AP_CFG,SSID="${SSIDNAME}",SEC="wpa2-psk",KEY="5acb375a0820ba8ff0b22fb668436bf25a9216b13a82310875b00fd62e92e36d",CHANNEL=1,PREAMBLE=1,MAX_SCB=2,END
        iwpriv wlan0 AP_SET_CFG ASCII_CMD=AP_CFG,SSID="${SSIDNAME}",SEC="wpa2-psk",KEY="$PSK",CHANNEL=1,PREAMBLE=1,MAX_SCB=2,END
        
        #
        #		wpa_passphrase "${SSIDNAME}" "${TYPE_PW}"
        #
    fi
    
    #	iwpriv wlan0 AP_BSS_START
}


# function configure_Client ()
# {
#
#    #Wireless Function(reti protette)
#	ifconfig wlan0 up
#	wpa_supplicant -iwlan0 -Dwext -c/etc/wpa_supplicant.conf -B
#	wpa_supplicant -iwlan0 -Dwext -c/etc/wpa_supplicant.conf -B
#	udhcpc -iwlan0
#	ping ip
#
#    #Wireless Function(reti libere)
#	iwconfig wlan0 channel x (x=numero canele)
#	iwconfig wlan0 essid nome_rete
#	udhcpc -iwlan0
#	ping ip
# }

function main()
{
    chekSSidPAss
    chekmoduleInstalled
    retval=$?
    if [ $retval -eq 1 ]
    then
        installModule
        chekmoduleInstalled
        retval=$?
        if [ $retval -eq 1 ]
        then
            exit -10
        fi
    fi
    interface_enable
    retval=$?
    
    
    if [ "$TYPE_CONFIG" != "AP" ]
    then
        echo "TYPE_CONFIG = ${TYPE_CONFIG}"
    else
        echo "CONFIGURE AS AP !!! "
        configure_AP
    fi
    
}

main




exit 0
