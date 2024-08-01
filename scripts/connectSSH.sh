#!/bin/sh

ssh-keygen -f "/home/zang3th/.ssh/known_hosts" -R "192.168.42.69"
ssh Krueger@192.168.42.69
