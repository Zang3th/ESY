#!/bin/sh

echo "-----------------------------------------------------------------------"
MAINDIR=~/Dev/ESY

# Defaultskript starten
echo "Call default script: ${MAINDIR}/buildroot/board/raspberrypi/post-build.sh"
${MAINDIR}/buildroot/board/raspberrypi/post-build.sh $*

# Kopiere Netzwerk/Interfaces Einstellungen
echo "Copy ${MAINDIR}/target/interfaces to ${TARGET_DIR}/etc/network/interfaces"
cp ${MAINDIR}/target/interfaces ${TARGET_DIR}/etc/network/interfaces

echo "Finished executing: post-build.sh"
echo "-----------------------------------------------------------------------"
