#!/bin/sh

echo "-----------------------------------------------------------------------"
MAIN_DIR=~/Dev/ESY

# Defaultskript starten
echo "Call default script: ${MAIN_DIR}/buildroot/board/raspberrypi/post-build.sh"
${MAIN_DIR}/buildroot/board/raspberrypi/post-build.sh $*
echo "Finished executing: default script"
echo "-----------------------------------------------------------------------"

# Kopiere Netzwerk/Interfaces Einstellungen
echo "Copy interfaces to ${TARGET_DIR}/etc/network/interfaces"
cp ${MAIN_DIR}/target/interfaces ${TARGET_DIR}/etc/network/interfaces

# Kopiere/Installiere User-Config
echo "Install user config to ${TARGET_DIR}/etc/init.d/user_conf"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/S100user ${TARGET_DIR}/etc/init.d/S100user

# Kopiere DHCP und WLAN-AP Konfiguration
echo "Copy dnsmasq.conf to ${TARGET_DIR}/etc/dnsmasq.conf"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/dnsmasq.conf ${TARGET_DIR}/etc/dnsmasq.conf

echo "Copy wpa_supplicant.conf to ${TARGET_DIR}/etc/wpa_supplicant.conf"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/wpa_supplicant.conf ${TARGET_DIR}/etc/wpa_supplicant.conf

echo "Copy S61wpa to ${TARGET_DIR}/etc/init.d/S61wpa"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/S61wpa ${TARGET_DIR}/etc/init.d/S61wpa

# Kopiere Firewall-Config
echo "Copy S99firewall to ${TARGET_DIR}/etc/init.d/S99firewall"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/S99firewall ${TARGET_DIR}/etc/init.d/S99firewall

# Kopiere sysctl.conf (für IP-Forwarding)
echo "Copy sysctl.conf to ${TARGET_DIR}/etc/sysctl.conf"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/sysctl.conf ${TARGET_DIR}/etc/sysctl.conf

echo "Finished executing: post-build.sh"
echo "-----------------------------------------------------------------------"
