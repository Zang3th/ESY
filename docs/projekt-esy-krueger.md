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

Das hier beschriebene Projekt im Rahmen des Moduls "Eingebettete Systeme" bietet die Möglichkeit, praxisnah die Entwicklung eines vollständig angepassten Smarthome-Gateways „von der Pike auf“ zu erlernen.

Das Projekt bildet den Abschluss des Moduls und integriert alle während des Semesters erarbeiteten Komponenten und kennengelernten Technologien in eine eigenständige Praxisarbeit.

## Motivation

Während der Entwicklung wird Wissen über Linux, Buildroot, Netzwerkkonfiguration, das Kommunikationsprotokoll MQTT und viele weitere Technologien vertieft. Die Herausforderung besteht darin, ein vollständiges System auf einem Raspberry Pi zu implementieren, das von Grund auf neu konfiguriert und entwickelt wurde. Dies umfasst sowohl die Konfiguration und Kompilierung eines Linux-Kernels und Userlands als auch die Implementierung spezifischer Treiber und Dienste, die das System funktionsfähig machen.

## Aufgabenstellung

Ziel des Projekts ist die Entwicklung eines eingebetteten, auf Linux basierenden Systems, das auf einem Raspberry Pi 4 läuft und als Smarthome-Gateway agiert. Zu den Kernaufgaben gehören die Implementierung eines Systems, das per TFTP bootet, die Erstellung eines LED-Treibers sowie die Bereitstellung eines MQTT-Brokers, der Daten verschiedener Systemparameter veröffentlicht. Das System spannt zudem ein WLAN auf, das über ein Smartphone zur Steuerung der LED und zur Visualisierung der Systeminformationen genutzt wird. Die Umsetzung dieser Anforderungen wird durch die vorliegende Dokumentation unterstützt.

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

Ein Teil der Aufgabenstellung war die Konfiguration des Raspberry Pi's als WLAN Accesspoint. Der Kernel und das Userland wurden dafür entsprechend präperiert. Aufgespannt wird das WLAN *VimIsTheBest*. Verteilt werden IP-Adressen aus dem Netz 192.168.123.0/24. Skripte und Konfigurationsdateien finden sich im Verzeichnis *target/*. Alle Dateien werden nach *init.d* kopiert und automatisch beim Bootvorgang gestartet.

Die WLAN-AP-Konfiguration umfasst:

- WPA
- DNS und DHCP
- Firewall mit NAT und IP-Forwarding

Unter Umständen taucht beim Booten die Fehlermeldung *brcmfmac: brcmf_set_channel: set chanspec 0x100d fail, reason -52* auf. Diese wird von der WLAN-Hardware ausgelöst.

- Dabei handelt es sich um einen bekannten Bug, bisher ohne richtigen Fix:
  - Siehe z.B. [https://github.com/raspberrypi/linux/issues/6049]
- Der Bug betriff sowohl Raspberry Pi's der Version 4 als auch der Version 5
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

*sig_write* erlaubt das An- und Ausschalten der LED, sowie das Setzen einer Blinkfrequenz. User-Input wird dabei als String entgegen genommen und intern konvertiert. *sig_write* ruft dafür die Funktion *validate_user_input* auf. Falls die Eingabe valide ist werden eine Zustandvariable und die Frequenz entsprechend neu gesetzt. Erst dann wird der Kernelthread via *wake_up_interruptible()* aufgeweckt. Um einen gesicherten asynchronen Zugriff zu gewährleisten, wurde ein Spinlock verwendet.

```c
static void validate_user_input(char* buffer, unsigned long buf_size)
{
    // Convert and validate user input
    [...]

    // Enter critical section: Set global variable to provided user input
    spin_lock(&freq_lock);
    freqKrueger = user_input;
    new_input = true;
    spin_unlock(&freq_lock);

    // Wake up thread
    wake_up_interruptible(&freq_wq);
}
```

\pagebreak

Realisiert wird das Blinken bzw. Toggeln der LED durch ein Schlafenlegen des Kernelthreads via *wait_event_interruptible_timeout()*. Der Timeout des Schlafens entspricht der Frequenz des Blinkens, vorausgesetzt diese ist eingeschaltet und soll überhaupt blinken (mehr zur genauen Verwendung bzw. den erlaubten Inputs im nächsten Abschnitt). Falls neuer, gültiger User-Input bereitgestellt wurde, wird die LED entsprechend an- oder ausgeschaltet und die neue Frequenz bzw. Schlafszeit wird berechnet.

```c
static int blink_thread_func(void* params)
{
    int ret = 0, value = 0;
    long int provided_freq = 0, sleep_msecs = 1000;
    bool led_on = false;

    while(!kthread_should_stop())
    {
        ret = wait_event_interruptible_timeout
              ( freq_wq,
                ({
                   spin_lock(&freq_lock);
                   int cond = (new_input == true);
                   provided_freq = freqKrueger;
                   spin_unlock(&freq_lock);
                   cond;
                }),
                msecs_to_jiffies(sleep_msecs)
              );

        if (ret == 0) // Timeout reached
        {
            if(led_on == true) // If LED is activated, toggle it accordingly
            {
                value ^= 1;
                gpiod_set_value(gpio_desc, value);
            }
        }
        else if (ret == -ERESTARTSYS) // Signal came in
        {
            pr_info("signalru: interrupted by signal ...\n");
        }
        else // Condition is true
        {
            if(provided_freq == 0) // Turn off LED
            {
                led_on = false;
                gpiod_set_value(gpio_desc, 0);
            }
            else // Turn on LED and calculate new frequency aka sleep time
            {
                led_on = true;
                sleep_msecs = 1000 / provided_freq;
            }

            // Reset state (no new user input is yet provided)
            spin_lock(&freq_lock);
            new_input = false;
            spin_unlock(&freq_lock);
        }
    }

    return 0;
}
```

\pagebreak

### Verwendung

Die Verwendung des Treibers läuft über die bereitgestellte Gerätedatei *led_onoff_ru*. Wie bereits erwähnt können Strings in diese gepiped werden, der Input wird intern kovertiert und validiert.

- Der Wert **0** schaltet die LED aus
- Ein Wert zwischen **1-10** schaltet die LED mit der entsprechenden Frequenz in Hertz an
- Negative Werte und Werte größer **10** werden verworfen

Die Gerätedatei kann außerdem ausgelesen werden. In diesem Fall returned sie die aktuell gesetzte Frequenz.

## MQTT-Konfiguration

Ein weiterer Teil des Projekts ist das Betreiben des Pi's als Smarthome-Gateway. Als Kommunikationsprotokoll kommt dabei MQTT zum Einsatz. Als MQTT-Broker wird mosquitto verwendet, welcher bereits in Buildroot integriert ist. Die Konfigurationsschritte sahen im groben wie folgt aus:

- mosquitto in Buildroot aktivieren
- Die default mosquitto.conf in lokales Arbeitsverzeichnis kopieren
- mosquitto.conf editieren
  - Extensives Logging anschalten
  - Anonymen Access zulassen
  - Interface konfigurieren (IP-Adresse, Port, wlan0)
- mosquitto.conf standardmäßig aufs Target kopieren
- Startskript erstellen, welches mosquitto beim Booten automatisch startet
  - **Wichtig:** Erst nach der WPA/wlan0-interface Konfiguration!
  - Loopback-Interface muss ebenfalls aktiviert sein!

Als nächstes musste ein Skript (*target/S97topics*) geschrieben werden, welches in regelmäßigen Abständen (alle 60 Sekunden) verschiedene Topics published (siehe unten für einen Code-Ausschnitt). Alle Rückmeldungen werden dabei verworfen, da Fehlermeldungen separat in eine Loggingdatei geschrieben werden und nicht gepublished werden sollen. Auch dieses Skript wird standardmäßig beim Booten geladen.

Das regelmäßige Publishen **musste** über einen while-true-Loop in Bash gelöst werden, da die internen Parameter *--repeat* und *--repeat-delay* des *mosquitto_pub* Kommandos nur mit den initialen Werten der Skriptvariablen arbeiten. Das Kommando wird sozusagen nicht nochmal "richtig" ausgeführt, sondern es wird nur das Ergebnis des ersten Ausführens erneut gepublished. Diese Art der Implementierung wird bestimmt ihren Sinn haben, scheint mir aber zu kurz gegriffen. Zumindest die Option für ein erneutes "richtiges" Ausführen des Kommandos würde ich begrüßen.

```bash
# S97topics (Ausschnitt)

publish_topics()
{
    while true; do
        mosquitto_pub -h ${BROKER} -d -t ${HOST_TOPIC}  -m ${HOST_MSG}  > /dev/null 2>&1
        mosquitto_pub -h ${BROKER} -d -t ${eth0_TOPIC}  -m ${eth0_MSG}  > /dev/null 2>&1
        mosquitto_pub -h ${BROKER} -d -t ${wlan0_TOPIC} -m ${wlan0_MSG} > /dev/null 2>&1
        mosquitto_pub -h ${BROKER} -d -t ${DATE_TOPIC}  -m ${DATE_MSG}  > /dev/null 2>&1
        mosquitto_pub -h ${BROKER} -d -t ${NAME_TOPIC}  -m ${NAME_MSG}  > /dev/null 2>&1
        mosquitto_pub -h ${BROKER} -d -t ${UP_TOPIC}    -m $(uptime | cut -d' ' -f2) > \
            /dev/null 2>&1
        mosquitto_pub -h ${BROKER} -d -t ${LED_TOPIC}   -m $(cat /dev/led_onoff_ru)  > \
            /dev/null 2>&1
        sleep ${INTERVAL}
    done &
}
```

\pagebreak

Ein Topic welches gepublished aber auch angesteuert werden sollte, war die Blinkfrequenz der LED. Im signalru-Treiber wurden, wie bereits im vorhergegangen Kapitel erwähnt, die dafür notwendigen Funktionen implementiert.

Das Topic-Publishing-Skript subscribed per *mosquitto_sub* aufs Topic "appl/frequency_set" und piped stdin in ein LED-Kontrollskript (*target/ctrl_led_ru.sh*).

```bash
# S97topics (Ausschnitt)

if [ "$1" == "start" ]; then
    publish_topics
    echo "Started topic publishing service"
    (mosquitto_sub -h ${BROKER} -d -t ${LED_TOPIC_CTRL} | ${LED_SCRIPT_PATH}) &
    echo "Piped topic ${LED_TOPIC_CTRL} into script"
fi
```
Dieses LED-Kontrollskript läuft in einem while-true-Loop bis es Input erhält und schreibt diesen dann in die Gerätedatei.

```bash
# ctrl_led_ru.sh

#!/bin/sh

echo "Started LED control script"

while true; do
    read
    echo "${REPLY}" > /dev/led_onoff_ru
done
```

# Tests

## Testplan

In einem *Rapid Prototyping* Setting, wie es im Rahmen dieses Projekts der Fall ist, steht die schnelle Umsetzung und iterative Verbesserung des Systems im Vordergrund. Ein Testing-Ansatz in diesem Kontext muss flexibel und effizient sein, um den Entwicklungsprozess zu unterstützen.

Der Testplan ist relativ simpel. Die Anforderungen an eine Komponente werden schriftlich festgehalten (in diesem Fall als Teil der Aufgabenstellung) und anschließend implementiert. Nach der Implementierung wird geprüft, ob alle Anforderungen erfüllt wurden und ob keine unerwünschten Nebeneffekte aufgetreten sind. Dieser pragmatische Ansatz stellt sicher, dass die Entwicklungszyklen kurz und effizient bleiben, ohne dabei die Qualität des Systems zu vernachlässigen.

**Komponententests**

Diese Tests überprüfen jede einzelne Komponente des Systems separat, um sicherzustellen, dass sie unabhängig und korrekt funktioniert, bevor sie in das Gesamtsystem integriert wird. Da das Projekt verschiedene Komponenten – wie den LED-Treiber, die WLAN-Konfiguration und den MQTT-Broker – umfasst, ist es entscheidend, dass jede dieser Komponenten ihre spezifischen Anforderungen erfüllt. Diese Tests helfen, Fehler und Inkonsistenzen frühzeitig zu identifizieren und zu beheben, bevor die Komponenten miteinander interagieren. Dies reduziert das Risiko, dass Probleme erst in einem späteren Stadium der Entwicklung auftreten, wenn sie schwerer zu beheben sind.

**Systemtest**

Neben den Komponententests wird das System als Ganzes überprüft, um sicherzustellen, dass es in der Praxis den Anforderungen gerecht wird. Dieser vollständige Systemtest umfasst alle Aspekte des Projekts – vom Booten des Systems über TFTP, der Konnektivität des WLANs, bis hin zur Steuerung und Visualisierung über das Smartphone. Durch diese Tests wird sichergestellt, dass das System nicht nur in isolierten Modulen, sondern auch in seiner Gesamtheit stabil und funktional ist.

\pagebreak

## Komponententests

### Boot und Entwicklung

### WLAN

![WLAN-Test](res/wlan_test.png)

\pagebreak

### LED-Treiber

### MQTT

![MQTT-Test](res/mqtt_test.png)

\pagebreak

## Systemtest

![Systemtest: Setzen der Frequenz](res/device_test_1.png)
![Systemtest: Erfolg](res/device_test_2.png)

\pagebreak

# Zusammenfassung

**Aufgabenstellung**

Das Ziel dieses Projekts war die Entwicklung eines eingebetteten Systems auf Basis von Linux, das auf einem Raspberry Pi 4 läuft und als Smarthome-Gateway agiert. Die zentralen Aufgaben umfassten die Implementierung eines Systems, das per TFTP bootet, die Entwicklung eines LED-Treibers zur Steuerung einer LED über GPIO-Pin's, und die Bereitstellung eines MQTT-Brokers, der verschiedene Systemparameter über das Netzwerk veröffentlicht. Darüber hinaus sollte das System ein WLAN-Netzwerk aufspannen, das es ermöglicht, die LED über ein Smartphone zu steuern und Systeminformationen zu visualisieren.

**Inhalte**

Das Projekt wurde Schritt für Schritt umgesetzt, beginnend mit der Konfiguration des Netzwerkboots über TFTP, wodurch der Raspberry Pi die notwendigen Bootdateien von einem TFTP-Server auf dem Host-Rechner bezieht. Als nächstes wurde das Userland mithilfe von Buildroot erstellt, während der Kernel separat kompiliert und angepasst wurde. Ein wichtiger Bestandteil war dort die Konfiguration des Raspberry Pi's als WLAN-Access-Point.

Ein wesentliches Element des Projekts war die Entwicklung eines Gerätetreibers (Kernelmodul) zur Ansteuerung einer LED. Die Integration dieses Treibers ermöglicht es, die LED per MQTT fernzusteuern, wobei die Blinkfrequenz über ein spezifisches MQTT-Topic gesetzt werden kann. MQTT wurde durch die Integration des Mosquitto-Brokers realisiert.

**Ergebnis**

Das System läuft stabil und erfüllt die gestellten Anforderungen. Der Raspberry Pi kann erfolgreich über TFTP booten, das WLAN-Netzwerk aufspannen und die LED über MQTT steuern. Die LED reagiert korrekt auf die vom Smartphone gesendeten Befehle zur Einstellung der Blinkfrequenz, und alle relevanten Systeminformationen werden über das MQTT-Protokoll veröffentlicht und visualisiert. Die Integration der verschiedenen Systemkomponenten verlief erfolgreich, und das System zeigt keine unerwünschten Nebeneffekte.

**Ausblick**

Obwohl das Projekt erfolgreich abgeschlossen wurde, gibt es noch vielzählige Möglichkeiten für Erweiterungen und Optimierungen. Beispielsweise könnten Sensoren oder komplexere Aktoren in das System integriert werden, um die Funktionalität des Smarthome-Gateways zu erweitern. Das Projekt zeigt ja auch nur prototypisch, durch Toggeln eines einzelnen Pin's, die Fernsteuerungmöglichkeiten per MQTT. Zudem könnte die Sicherheit des Systems durch die Implementierung von Schutzmaßnahmen, wie etwa einer Nutzerauthorisierung in der MQTT-Kommunikation, verbessert werden.
