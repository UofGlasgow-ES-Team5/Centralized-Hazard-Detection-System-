#!/bin/bash

# Check if the script is run as root
if [ "$(id -u)" -ne 0 ]; then
    echo "Please run as root (sudo)"
    exit 1
fi

# Check if the required argument is provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <SSID> <PASSWORD>"
    exit 1
fi

SSID=$1
PASSWORD=$2

# Path to the wpa_supplicant configuration file
WPA_CONF="/etc/wpa_supplicant/wpa_supplicant.conf"

# Check if the network entry already exists in the configuration file
if grep -q "ssid=\"$SSID\"" "$WPA_CONF"; then
    echo "Network entry already exists in $WPA_CONF"
    exit 1
fi

# Add the new network entry to the wpa_supplicant configuration file
echo -e "\nnetwork={" >> "$WPA_CONF"
echo -e "    ssid=\"$SSID\"" >> "$WPA_CONF"
echo -e "    psk=\"$PASSWORD\"" >> "$WPA_CONF"
echo "}" >> "$WPA_CONF"

# Restart the networking service to apply changes
systemctl restart networking

echo "WiFi configuration updated successfully for SSID: $SSID"