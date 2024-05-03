#!/bin/sh

# MARK A: Originalskript starten
echo "${TARGET_DIR}/../../board/raspberrypi/post-build.sh $*"
${TARGET_DIR}/../../board/raspberrypi/post-build.sh $*

# MARK B: Zeit setzen

# MARK C: SD-Karte einhängen

# MARK D: Webserver

# MARK E: Firewall

# MARK F: Usermanagement