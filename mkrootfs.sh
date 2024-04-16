#!/bin/bash

MAINDIR=~/Dev/ESY

cd $MAINDIR/Userland

# Dateisystem anlegen

# Dateisystem formatieren

# Dateisystem einhängen (mounten)
sudo mount -o loop rootfs.img loop

# Verzeichnisstruktur im eingehängten Dateisystem (Image) anlegen

# Busybox generieren
make -C ./busybox-1.36.1 menuconfig

# Bauen und installieren
make -C ./busybox-1.36.1 && make -C ./busybox-1.36.1 install

# Busyboxdateien ins Image installieren (kopieren)
sudo rsync -a busybox-1.36.1/_install/ loop

# Gerätedateien im Image anlegen

# Applikationsdateien kopieren (Webserver)

# Zugriffsrechte anpassen

# MARK A

# MARK B

# MARK C

# MARD D

# Image (Dateisystem) wieder aushaengen
sudo umount loop

cd ..

./start_el.sh

