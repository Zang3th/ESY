#!/bin/sh

echo "-----------------------------------------------------------------------"
MAINDIR=~/Dev/ESY

# Defaultskript starten
echo "Call default script: ${MAINDIR}/buildroot/board/raspberrypi/post-image.sh"
${MAINDIR}/buildroot/board/raspberrypi/post-image.sh $*

# Kernel und Initramfs kopieren
echo "Copy kernel and userland"
sudo cp ${MAINDIR}/kernel/zImage /srv/tftp/kernel7l.img
sudo cp ${MAINDIR}/buildroot/output/images/rootfs.cpio /srv/tftp/rootfs.cpio

echo "Finished executing: post-image.sh"
echo "-----------------------------------------------------------------------"
