#!/bin/bash

# Originalscript starten
echo "${TARGET_DIR}/../../board/raspberrypi/post-image.sh $*"
${TARGET_DIR}/../../board/raspberrypi/post-image.sh $*

# Kernel und Initramfs kopieren
echo "Copy kernel and userland"
sudo cp ../Lernmaterialien/zImage /srv/tftp/kernel7l.img
sudo cp output/images/rootfs.cpio /srv/tftp/rootfs.cpio
echo "Done"