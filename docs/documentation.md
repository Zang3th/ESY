# Projektdokumentation

## Timeline

### Netzwerkboot

- TFTP-Server installieren und starten
  - Logging aktivieren
  - Monitoren des tftp-outputs via: tail -f /var/log/syslog | grep tftp
- Netzwerkschnittstelle auf Host-PC statische IP zuweisen
- Originale Bootfiles runterladen und ins tftp Verzeichnis kopieren
- Minicom installieren, konfigurieren und starten
  - sudo minicom -D /dev/ttyUSB0
- Bootloader des Pi ist für Netzwerkboot bereits konfiguriert
- config.txt nach /srv/tftp kopieren
- cmdline.txt nach /srv/tftp kopieren
  - Achtung Datei wurde verändert

### Buildroot (initial)

- Buildroot installieren
- Pi4-Config erstellen
- Userland konfigurieren
  - Identisch zu Aufgabenblatt 5 (inkl. dropbear für SSH)
- Kernelerstellung abwählen

#### Skripte

- post-build.sh erstellen
  - target/interfaces anlegen und auf Target kopieren
  - Relevant für SSH
- post-image.sh erstellen
- Skripte in Buildroot-Config hinterlegen

### Zusätzlicher User

- user_conf-Skript hinzugefügt welches mit Ausführungsrechten nach init.d installiert wird
  - Skript muss speziellen Namen tragen, hier "S99user"

### WLAN

- Buildroot-Konfiguration nach Aufgabenblatt 7
- Skripte und Konfigurationsdateien für WPA, DHCP, Firewall und NAT anlegen und auf Target kopieren
- Alter Kernel ist nicht WLAN-fähig
  - Neubau von Kernel 6.6 mit WLAN-Support (absichtliche Wahl einer älteren Version)
- Falls Fehlermeldung beim Booten auftaucht: brcmfmac: brcmf_set_channel: set chanspec 0x100d fail, reason -52
  - Bekannter Fehler/Bug, bisher ohne Fix: [https://github.com/raspberrypi/linux/issues/6049]

### Gerätetreiber

- exports.sh erstellen und anpassen
- Kernel für Pi 4 bauen (mit WLAN und modules support)
  - make bcm2709_defconfig
  - Kernel-Config aus Moodle als Grundlage
  - time make -j24 zImage dtbs modules
- hello.c crosskompilieren und unter /lib/modules ablegen
- Startskript schreiben, welches Kernelmodul beim booten lädt

#### Entwicklung des LED-Treibers

- Entwicklung auf Host mit Deployment per scp
  - scp led.ko root@192.168.42.69:/lib/modules
  - Logging via: watch -n 1 dmesg
- consumer.h api mit Kernel 6.6 und 6.9 ausprobiert, beide Male kein Erfolg
  - Ansatz über Device-Tree
    - Device Tree Blob Overlay erstellen und GPIO PIN 23 definieren
    - .dts zu .dtbo kompilieren
    - .dtbo nach /srv/tftp/overlays verschieben
    - Device Tree Blob in config.txt eintragen
    - LED-Treiber auf DTBO-API anpassen
