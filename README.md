# Embedded Systems (Master Computer Science)

This project is the final assignment of the "Embedded Systems" module, focused on developing a fully customized smart home gateway using a Raspberry Pi 4.

The gateway is built from scratch, rolling a customized kernel and userland (using Buildroot), network configuration, MQTT, and more.

Because this project is the result of an university assignment, parts of it (especially the documentation) may be in German.

## Project Overview

The main goal is to create an embedded Linux-based system that functions as a smart home gateway. Key features include:

- Network Booting: The system boots via TFTP.
- LED Driver: A custom kernel module to control an LED through GPIO pins, with frequency adjustable via MQTT.
- MQTT Broker: Publishes various system parameters, enabling smart home integration.
- WLAN Access Point: The Raspberry Pi is configured as a Wi-Fi access point for remote control and monitoring.
