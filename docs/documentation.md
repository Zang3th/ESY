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

#### Skripte

- post-build.sh erstellen
  - target/interfaces anlegen und auf Target kopieren
  - Relevant für SSH
- post-image.sh erstellen
- Skripte in Buildroot-Config hinterlegen

### Zusätzlicher User

- user_conf-Skript hinzugefügt welches mit Ausführungsrechten nach init.d installiert wird
  - Funktioniert aktuell aber noch nicht (wird nicht ausgeführt?)
  - Werden automatisch alle Skripte aus init.d ausgeführt?
  - Wenn ja, muss das Format der Datei falsch sein!

### WLAN

- Buildroot-Konfiguration nach Aufgabenblatt 7
- Skripte und Konfigurationsdateien für WPA, DHCP, Firewall und NAT anlegen und auf Target kopieren
- Alter Kernel ist doch nicht WLAN-fähig + Config ist verloren gegangen
  - Neubau von Kernel 6.6 (absichtliche Wahl einer älteren Version)
