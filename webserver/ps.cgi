#!/bin/bash

echo "Content-type: text/html"
echo ""

echo "<html><head><title>Process List</title></head><body>"
echo "<h1>Process List</h1>"
echo "<pre>"

# Ausgabe der laufenden Prozesse
ps

echo "</pre>"
echo "</body></html>"