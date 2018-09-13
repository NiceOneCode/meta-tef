#!/bin/bash

IPTABLES=/usr/sbin/iptables

# Trusted public network (da cui si permettono collegamenti SSH)
TRUSTEDL="192.168.77.151/32"
TRUSTEDW="192.168.0.151/32"
TRUSTEDX="192.168.77.101/32"

### ENABLE DEBUG
# set -x

### SETTING IPFORWARDING ### Abilita il forwarding di pacchetti non locali - FONDAMENTALE
echo "1" > /proc/sys/net/ipv4/ip_forward

### DISABLE RESPOND TO BROADCAST ### Non risponde ai ping inviati al browadcast della subnet
#/bin/echo "1" > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

### ENABLE BAD ERROR MESSAGE PROTECTION ### Ignora finti messaggi di errore ICMP
#/bin/echo "1" > /proc/sys/net/ipv4/icmp_ignore_bogus_error_responses

### DISABLE ICMP REDIRECT ACCEPTANCE ### Non accetta pacchetti ICMP di route redirection
#/bin/echo "0" > /proc/sys/net/ipv4/conf/all/accept_redirects

### SETTING ANTISPOOFING PROTECTION ###
#/bin/echo "1" > /proc/sys/net/ipv4/conf/all/rp_filter

### DON'T RESPOND TO BROADCAST PINGS ###
#/bin/echo "1" > /proc/sys/net/ipv4/conf/all/log_martian

### FLUSHING CHAIN ### Azzera e pulisce ogni regola esistente
$IPTABLES -F
$IPTABLES -F -t nat
$IPTABLES -X
$IPTABLES -Z

#capiteve portal
$IPTABLES -t nat -A PREROUTING -p tcp --dport 80 -i wlan0 -j DNAT --to 192.168.0.20:3000
$IPTABLES -t nat -A PREROUTING -p tcp --dport 443 -i wlan0 -j DNAT --to 192.168.0.20:3000
#$IPTABLES -t nat -A PREROUTING -p tcp --dport 80 -i wlan0 -j REDIRECT --to 3000
$IPTABLES -t nat -A POSTROUTING -j MASQUERADE

#consente il traffico in loopback
$IPTABLES -A INPUT -i lo -j ACCEPT
$IPTABLES -A OUTPUT -o lo -j ACCEPT

# SSH Regole per permettere l'accesso al server SSH locale da un IP esterno fidato precedentemente indicato
$IPTABLES -A INPUT -i eth0 -s $TRUSTEDL -p TCP --dport 22 -j ACCEPT
$IPTABLES -A INPUT -i eth0 -s $TRUSTEDX -p TCP --dport 22 -j ACCEPT
$IPTABLES -A INPUT -s $TRUSTEDW -p TCP --dport 22 -j ACCEPT
$IPTABLES -A INPUT -p tcp -s 0.0.0.0/0 --dport 22 -j DROP

# Abilita l'accesso alla sottorete  192.168.0.XXX
$IPTABLES -A INPUT -s 192.168.0.0/255.255.255.0 -j ACCEPT
$IPTABLES -A INPUT -s 192.168.0.0/255.255.255.0 -j ACCEPT

# ABlita l'accesso solo per le porte 80 e 443
$IPTABLES -A INPUT   -p TCP -m multiport --dports 80,443 -j ACCEPT
$IPTABLES -A OUTPUT  -p TCP -m multiport --sports 80,443 -j ACCEPT


### DEFAULT CHAIN ### Imposta le policy di default
$IPTABLES -P INPUT DROP
$IPTABLES -P FORWARD DROP
#$IPTABLES -P OUTPUT DROP
#$IPTABLES -t nat -P POSTROUTING DROP
#$IPTABLES -t nat -P PREROUTING DROP
