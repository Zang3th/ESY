#!/bin/sh

# MARK A: Originalskript starten
echo "Rufe Default-Skript auf *board/raspberrypi/post-build.sh*"
${TARGET_DIR}/../../board/raspberrypi/post-build.sh $*

# MARK B: Netzwerk
echo "Kopiere Datei *interfaces* nach /etc/network/interfaces"
cp ../target/interfaces ${TARGET_DIR}/etc/network/interfaces
# echo "Kopiere Datei: resolv.conf"
# cp ../target/resolv.conf ${TARGET_DIR}/etc/network/resolv.conf

# MARK C: Zeit setzen

# MARK D: SD-Karte einhängen

# MARK E: Webserver
echo "Kopiere *S51httpd* nach /etc/init.d"
sudo install -m 0755 -o root -g root ../target/S51httpd ${TARGET_DIR}/etc/init.d/S51httpd
echo "Kopiere *httpd.conf* nach /etc/"
sudo install -m 0755 -o root -g root ../webserver/httpd.conf ${TARGET_DIR}/etc/httpd.conf
echo "Kopiere Webserver-Dateien nach /var/www/"
sudo install -m 0777 ../webserver/index.html ${TARGET_DIR}/var/www/index.html
sudo install -m 0777 ../webserver/subindex.html ${TARGET_DIR}/var/www/subindex.html
# mkdir -p ${TARGET_DIR}/var/www/cgi-bin
mkdir -p ${TARGET_DIR}/var/www/log
# sudo install -m 0777 ../webserver/ps.cgi ${TARGET_DIR}/var/www/cgi-bin/ps.cgi
sudo install -m 0777 ../webserver/ps.cgi ${TARGET_DIR}/var/www/ps.cgi

# MARK F: Firewall

# MARK G: Usermanagement