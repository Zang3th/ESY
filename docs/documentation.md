# Projektdokumentation

## Timeline

### Netzwerkboot

- TFTP-Server installieren und starten
  - Logging aktivieren
- Netzwerkschnittstelle auf Host-PC statische IP zuweisen
- Originale Bootfiles runterladen und ins tftp Verzeichnis kopieren
- Minicom installieren, konfigurieren und starten
- Bootloader des Pi ist für Netzwerkboot bereits konfiguriert
- config.txt nach /srv/tftp kopieren
- cmdline.txt nach /srv/tftp kopieren
  - Achtung Datei wurde verändert

### Buildroot

- Buildroot installieren
- Pi4-Config erstellen
- Userland konfigurieren
  - Identisch zu Aufgabenblatt 5
  - Kernelheader 6.6 (nur relevant für Buildroot-Kernel)

#### Skripte

- post-build.sh erstellen
  - target/interfaces anlegen und auf target kopieren
- post-image.sh erstellen
- Skripte in Buildroot-Config hinterlegen
