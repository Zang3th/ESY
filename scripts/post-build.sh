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
sudo install -m 0755 -o root -g root ${MAIN_DIR}/target/user_conf ${TARGET_DIR}/etc/init.d/user_conf

echo "Finished executing: post-build.sh"
echo "-----------------------------------------------------------------------"
