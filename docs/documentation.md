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

### Buildroot

- Buildroot installieren
- Pi4-Config erstellen
- Userland konfigurieren
  - Identisch zu Aufgabenblatt 5 (inkl. dropbear für SSH)
  - Kernelheader 6.6 (nur relevant für Buildroot-Kernel)

#### Skripte

- post-build.sh erstellen
  - target/interfaces anlegen und auf target kopieren
  - Relevant für SSH
- post-image.sh erstellen
- Skripte in Buildroot-Config hinterlegen

### Zusätzlicher User

- user_conf-Skript hinzugefügt welches mit Ausführungsrechten nach init.d installiert wird
  - Funktioniert aktuell aber noch nicht (wird nicht ausgeführt?)
  - Werden automatisch alle Skripte aus init.d ausgeführt?
  - Wenn ja, muss das Format der Datei falsch sein!
