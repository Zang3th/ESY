#!/bin/sh

echo "Started LED control script"

while true; do
    read
    echo "${REPLY}" > /dev/led_onoff_ru
done
