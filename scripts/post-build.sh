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
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/S99user ${TARGET_DIR}/etc/init.d/S99user

# Kopiere DHCP und WLAN-AP Konfiguration
echo "Copy dnsmasq.conf to ${TARGET_DIR}/etc/dnsmasq.conf"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/dnsmasq.conf ${TARGET_DIR}/etc/dnsmasq.conf

echo "Copy wpa_supplicant.conf to ${TARGET_DIR}/etc/wpa_supplicant.conf"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/wpa_supplicant.conf ${TARGET_DIR}/etc/wpa_supplicant.conf

echo "Copy S97wpa to ${TARGET_DIR}/etc/init.d/S97wpa"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/S97wpa ${TARGET_DIR}/etc/init.d/S97wpa

# Kopiere Firewall-Config
echo "Copy S98firewall to ${TARGET_DIR}/etc/init.d/S98firewall"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/S98firewall ${TARGET_DIR}/etc/init.d/S98firewall

# Kopiere sysctl.conf (für IP-Forwarding)
echo "Copy sysctl.conf to ${TARGET_DIR}/etc/sysctl.conf"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/sysctl.conf ${TARGET_DIR}/etc/sysctl.conf

# Kopiere Modul-Konfig
echo "Copy S05modules to ${TARGET_DIR}/etc/init.d/S05modules"
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/S05modules ${TARGET_DIR}/etc/init.d/S05modules

# Kopiere Kernel-Modul
KERNEL_MOD="signalru.ko"
echo "Copy ${KERNEL_MOD} to ${TARGET_DIR}/lib/modules"
mkdir -p ${TARGET_DIR}/lib/modules
cp ${MAIN_DIR}/modules/signalru/${KERNEL_MOD} ${TARGET_DIR}/lib/modules/${KERNEL_MOD}

echo "Finished executing: post-build.sh"
echo "-----------------------------------------------------------------------"
