---
title: "Projektaufgabe Embedded Systems SS2024"
author: Tim Krüger, 1197031
date: 13.08.2024
---

\pagebreak

# Allgemeines

## Persönliche Angaben

Name: Tim Krüger

Matrikelnummer: 1197031

Studiengang: Master Informatik

Datum: 13.08.2024

## Eigenständigkeitserklärung

![Eigenständigkeitserklärung](res/erklaerung_krueger.png)

\pagebreak

# Einleitung

## Motivation

## Aufgabenstellung

\pagebreak

# Installation

## Projektstruktur

Als Teil dieses Projekts werden alle notwendigen Dateien (Dokumentation, Configs, Skripte etc.) mitgeliefert. Die Struktur des Dateibaums ist im folgenden dargestellt:

    Dev/ESY/
    |-- buildroot/
    |-- configs/
    |-- devicetree/
    |-- docs/
    |   |-- res/
    |-- kernel/
    |-- modules/
    |-- scripts/
    |-- target/
    `-- txts/

- *Dev/ESY/*: Das Root-Verzeichnis des Projekts
  - Bei einer anderen Verzeichnisstruktur müssen die Skripte *post-build.sh* und *post-image.sh* entsprechend angepasst werden
- *buildroot/*: Das Buildroot-Verzeichnis.
  - Genaue Hinweise folgen in Kapitel 3.3
- *configs/*: Verzeichnis für die Buildroot- und Kernel-Konfigurationsdateien
- *devicetree/*: Verzeichnis für den Device-Tree-Blob zur Ansteuerung der GPIOs
- *docs/*: Dokumentation des Projekts inkl. dem Abbildungsverzeichnis *res/*
- *kernel/*: Bereits vorkompilierte Kernel
  - Genaue Hinweise folgen in Kapitel 3.4
- *modules/*: Der Treiber bzw. das Kernelmodul zur Ansteuerung der LED
  - Genaue Hinweise folgen in Kapitel 3.7
- *scripts/*: Verschiedene Skripte (Teil des Entwicklungsprozesses)
- *target/*: Verschiedenen Skripte und Konfigurationsdateien, welche auf den Raspberry Pi kopiert werden
- *txt/*: Verschiedene Textdateien, hauptsächlich zur initialien TFTP-Konfiguration

## Netzwerkboot

Für einen schnellen Entwicklungsprozess (Iterationsgeschwindigkeit + Deployment) wird Netzwerkboot via TFTP verwendet. Der Raspberry Pi zieht sich hierbei, bei korrekter Konfiguration, alle notwendigen Dateien des Bootprozesses von einem TFTP-Server, welcher auf dem Host-Rechner läuft.

Ein solches Setup kann mit den folgenden Schritten ans Laufen gebracht werden:

- Installation und Starten eines TFTP-Servers auf der Host-Maschine
  - Monitoren des Outputs bspw. via: *tail -f /var/log/syslog | grep tftp*
  - Empfehlung: Extensives Logging aktivieren
- Originale Pi4-Bootfiles herunterladen und ins TFTP-Verzeichnis kopieren
- Bootloader des Raspberry Pi's **muss** für Netzwerkboot angepasst werden
  - Dafür muss der Pi entsprechend geflashed werden
- Anpassung der Netzwerkschnittstellen
  - Zuweisung einer statischen IP ans Ethernet-Interface des Host-PC
  - Zuweisung und Konfiguration des Ethernet-Interface auf dem Pi (erfolgt über die Datei */target/interfaces*)

Es ist außerdem empfehlenswert das serielle Interface des Pi's für Output und Debugging zu verwenden. Die notwendigen Konfigurationsdateien werden bereitgestellt:

- Kopieren der Konfigurationsdateien
  - */txt/cmdline.txt* nach */srv/tftp/*
  - */txt/config.txt* nach */srv/tftp/*
- Installation eines beliebigen Terminalemulators, z.B. *Minicom*
  - Starten bspw. via: *sudo minicom -D /dev/ttyUSB0*

\pagebreak

## Buildroot

Als Systembuilder wird in diesem Projekt *Buildroot* verwendet. *Buildroot* ist im Endeffekt eine Sammlung von Makefiles, welche einem in einem kernelähnlichen Interface verschiedene Konfigurationsoptionen bieten. Buildroot wurde in diesem Projekt ausschließlich zum Bau des Userlands verwendet, der Kernel wurde *per Hand* kompiliert. Die exakten Konfigurationsschritte werden an dieser Stelle ausgelassen, da die Buildroot-Config zur Verfügung gestellt wird.

### Installationshinweise

- Buildroot installieren
  - Git-Repository nach *buildroot/* clonen
- *config/config.buildroot* nach *buildroot/* kopieren
- *make menuconfig* aufrufen und Auswahl speichern
- Durch ein anschließendes Aufrufen von *make* wird das Userland gebaut
- Die Skripte *post-build.sh* und *post-image.sh* klinken sich in diesen Prozess ein und kopieren die notwendigen Dateien an die richtigen Stellen im Image des TFTP-Servers
- **Achtung:** Auf korrekte Pfade achten
  - Die Pfade zu den Skripten können per *make menuconfig* geändert werden
  - Die Pfade innerhalb der Skripte müssen natürlich ebenfalls stimmen (Root-Verzeichnis oben im Skript als Variable hinterlegt)
  - Extensives Logging ist aktiviert und sollte an dieser Stelle schnell Auskunft geben können

## Kernel

Ein bereits crosskompilierter Kernel, welcher WLAN- und Modulefähig ist, findet sich im Verzeichnis *kernel/*. Es handel sich dabei um einen Kernel mit der Version 6.6.4. Dieser Kernel lief in meinen Tests am besten. Speziell die neueren Kernelversionen >= 6.9 haben bei mir Probleme gemacht. Bspw. lief der Arbeitsspeicher auf meiner Maschine beim Kompilieren von Kernel 6.9.1 voll, was schließlich zu einem Crash führte.

### Installationshinweise

Falls Sie einen eigenen Kernel für dieses Projekt kompilieren wollen, stehen alle notwendigen Dateien zur Verfügung.

- Clonen des entsprechenden Kernels von Git
- Hinterlegen der notwendigen Umgebungsvariablen durch: *source /scripts/exports.sh*
- *config/config.linux* ins Kernel-Verzeichnis kopieren
- Kompilieren durch: *time make -j ihr_name dtbs modules*
- Der neue Kernel muss dann noch namentlich im *post-image.sh* Skript hinterlegt werden
- Beim nächsten Aufruf von *make* innerhalb von *Buildroot* wird dieser dann kopiert

## WLAN-AP

Ein Teil der Aufgabenstellung war die Konfiguration des Raspberry Pi's als WLAN Accesspoint. Der Kernel und das Userland wurden dafür entsprechend präperiert. Aufgespannt wird das WLAN *VimIsTheBest*. Skripte und Konfigurationsdateien finden sich im Verzeichnis *target/*. Alle Dateien werden nach *init.d* kopiert und automatisch beim Bootvorgang gestartet.

Die WLAN-AP-Konfiguration umfasst:

- WPA
- DNS und DHCP
- Firewall mit NAT und IP-Forwarding

Unter Umständen taucht beim Booten die Fehlermeldung *brcmfmac: brcmf_set_channel: set chanspec 0x100d fail, reason -52* auf. Diese wird von der WLAN-Hardware ausgelöst.

- Dabei handelt es sich um einen bekannten Bug, bisher ohne richtigen Fix:
  - Siehe z.B. [https://github.com/raspberrypi/linux/issues/6049]
- Dieser betriff sowohl Raspberry Pi's der Version 4 als auch der Version 5
- Er tritt, laut meiner Recherche, seit den Kernelversionen >= 6.3 auf
- Der Bug hat aktuell keinen Effekt auf uns, sollte aber beobachtet werden

\pagebreak

## Hardwareaufbau

![Hardwareaufbau](res/hardware_aufbau.png)

Der Hardware- bzw. Schaltungsaufbau kann *Abbildung 2* entnommen werden. Es handelt sich dabei um eine sehr einfache Schaltung welche Demonstrationszwecken dient. Folgende Aspekte können durch den Aufbau demonstriert werden:

- Logging via serieller Schnittstelle
- Booting via Netzwerk (TFTP)
- Ansteuerung eines GPIO Pin's mittels des Device-Tree's
  - Später: Integration in Kernelmodul und Fernsteuerung per MQTT
- Der Pi ist in meinem Fall noch mit einem optionalen Gehäuse ausgestattet, welches Schutz- und Kühlungszwecken dient

\pagebreak

## Gerätetreiber

Bei dem in diesem Projekt integrierten Gerätetreiber handelt es sich um ein Kernelmodul, welches die Ansteuerung einer LED über den Device-Tree implementiert.

Im folgenden wird kurz auf einige entwicklungstechnische Aspekte eingegangen, bevor Implementierung und Verwendung des Treibers erläutert werden.

### Allgemeines

Der Gerätetreiber *signalru.c* befindet sich im Verzeichnis */modules/signalru/*. Zur Neukompilierung wird ein Makefile bereitgestellt. Zuvor müssen, identisch wie beim Kompilieren des Kernels, die Umgebungsvariablen zum Crosskompilieren geladen werden (via *exports.sh*). Das generierte Kernelmodul wird beim Neuladen der Buildroot-Config (via *make*) automatisch gefetched und nach /lib/modules/ kopiert. Ein Startskript, welches das Kernelmodul automatisch beim Booten lädt, wird ebenfalls bereitgestellt. Bei Umbenennung oder Ergänzung von neuen Kernelmodulen müssen diese Skripte entsprechend angepasst werden.

Wie bereits erwähnt werden die GPIOs in diesem Projekt über den Device-Tree angesprochen. Die zuvor verwendete GPIO-API (*gpio_to_desc* und Kollegen) scheint deprecated zu sein. Auch mit Kernelversion 6.6 funktionierte bei mir nur die Version über den Device-Tree.

Zunächst wird ein Device-Tree-Blob-Overlay erstellt, welches GPIO PIN 23 benennt (*/devicetree/custom_gpio.dts*). Diese Datei wird dann mit dem Device-Tree-Compiler kompiliert (*devicetree/custom_gpio.dtbo*) und nach */srv/tftp/overlays* kopiert. Das Device-Tree-Blob-Overlay muss außerdem noch in die */txts/config.txt* eingetragen werden.

### Implementierung

Die Implementierung des Kernelmoduls bestand aus dem Schreiben der folgenden Funktionen:

- Funktionen für Konfiguration und Speicherfreigabe der GPIO's
- *sig_driver_init* Funktion welche die grundlegende Initialisierung aller benötigten Komponenten beim Laden des Treibers vornimmt
  - Character device
  - Waitqueue
  - Kernelthread
  - ...
- *sig_driver_exit* Funktion welche alles wieder deallokiert und freigibt
- *sig_write* Funktion welche das Schreiben auf die Gerätedatei regelt
  - User-Input zum Setzen der LED-Frequenz
- *sig_read* Funktion welche das Lesen der Gerätedatei regelt
  - Ausgabe der aktuellen LED-Frequenz an den User
- *blink_thread_func* Kernelthread-Funktion welche in Abhängigkeit vom User-Input eine LED, mit einer bestimmten Frequenz, toggelt

Um einen gesicherten asynchronen Zugriff auf die globale Frequenzvariable zu gewährleisten, wurde ein Spinlock verwendet.

Realisiert wird das Blinken bzw. Toggeln der LED durch ein Schlafenlegen des Kernelthreads via *wait_event_interruptible_timeout()*. Der Timeout des Schlafens entspricht der Frequenz des Blinkens, vorausgesetzt diese ist eingeschaltet und soll überhaupt blinken (mehr zur genauen Verwendung bzw. den erlaubten Inputs im nächsten Abschnitt). Falls neuer, gültiger User-Input bereitgestellt wurde, wird die LED entsprechend an- oder ausgeschaltet und die neue Frequenz bzw. Schlafszeit wird berechnet.

*sig_write* erlaubt das An- und Ausschalten der LED, sowie das Setzen einer Blinkfrequenz. User-Input wird dabei als String entgegen genommen und intern konvertiert. Falls die Eingabe valide ist werden eine Zustandvariable und die Frequenz entsprechend neu gesetzt. Erst dann wird der Kernelthread via *wake_up_interruptible()* aufgeweckt.

\pagebreak

### Verwendung

Die Verwendung des Treibers läuft über die bereitgestellte Gerätedatei *led_onoff_ru*. Wie bereits erwähnt können Strings in diese gepiped werden, der Input wird intern kovertiert und validiert.

- Der Wert **0** schaltet die LED aus
- Ein Wert zwischen **1-10** schaltet die LED mit der entsprechenden Frequenz in Hertz an
- Negative Werte und Werte größer **10** werden verworfen

Die Gerätedatei kann außerdem ausgelesen werden. In diesem Fall returned sie die aktuell gesetzte Frequenz.

## MQTT-Konfiguration

# Systemtest

## Testplan

Der Testplan ist relativ simpel. Im Sinne von *Rapid Prototyping* werden die Anforderungen an eine Komponente verschriftlicht (hier Teil der Aufgabenstellung) und dann wird diese implementiert. Im Anschluss wird geprüft ob alle Anforderungen erfüllt wurden und keine ungewünschten Nebeneffekte aufgetreten sind.

## Komponententest

### Boot und Entwicklung

### WLAN

![WLAN-Test](res/wlan_test.png)

\pagebreak

### LED-Treiber

### MQTT

![MQTT-Test](res/mqtt_test.png)

\pagebreak

## Test des Gesamtsystems

![Gerätetest: Setzen der Frequenz](res/device_test_1.png)
![Gerätetest: Erfolg](res/device_test_2.png)

# Zusammenfassung
