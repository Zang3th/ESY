#!/bin/sh

echo "-----------------------------------------------------------------------"
MAIN_DIR=~/Dev/ESY

# Defaultskript starten
echo "Call default script: ${MAIN_DIR}/buildroot/board/raspberrypi/post-image.sh"
${MAIN_DIR}/buildroot/board/raspberrypi/post-image.sh $*
echo "Finished executing: default script"
echo "-----------------------------------------------------------------------"

# Kernel und Initramfs kopieren
echo "Copy kernel and userland"
sudo cp ${MAIN_DIR}/kernel/zImage /srv/tftp/kernel7l.img
sudo cp ${MAIN_DIR}/buildroot/output/images/rootfs.cpio /srv/tftp/rootfs.cpio

echo "Finished executing: post-image.sh"
echo "-----------------------------------------------------------------------"
