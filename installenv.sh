#!/bin/bash

# Display the current time
echo "[$(date)] Starting system update and upgrade..."

# Update and upgrade the system
apt update && apt -y upgrade
echo "[$(date)] System update and upgrade completed."

# Install common tools and libraries
echo "[$(date)] Starting to install common tools and libraries..."
apt -y install mc screen htop mono-complete exim4 libpcap-dev curl wget ipset net-tools tzdata ntpdate mariadb-server mariadb-client p7zip p7zip-full
echo "[$(date)] Common tools and libraries installation completed."

# Install development tools and dependency libraries
echo "[$(date)] Starting to install development tools and dependency libraries..."
apt -y install make gcc g++ libssl-dev libcrypto++-dev libpcre3 libpcre3-dev libtesseract-dev libx11-dev gcc-multilib libc6-dev build-essential libtemplate-plugin-xml-perl libxml2-dev libstdc++6 libmariadb-dev-compat libmariadb-dev
echo "[$(date)] Development tools and dependency libraries installation completed."

# Add i386 architecture support
echo "[$(date)] Starting to add i386 architecture support..."
dpkg --add-architecture i386
echo "[$(date)] i386 architecture support added."

# Install 32-bit libraries (i386)
echo "[$(date)] Starting to install 32-bit libraries (i386)..."
apt -y install libssl-dev:i386 libpcre3:i386 libxml2:i386 libmariadb-dev-compat:i386 libmariadb-dev:i386 libdb++-dev:i386 libdb-dev:i386 libdb5.3:i386 libdb5.3++:i386 libdb5.3++-dev:i386 libdb5.3-dbg:i386 libdb5.3-dev:i386
echo "[$(date)] 32-bit libraries installation completed."

# Install 64-bit libraries (x86_64)
echo "[$(date)] Starting to install 64-bit libraries (x86_64)..."
apt -y install libssl-dev libpcre3 libxml2 libmariadb-dev-compat libmariadb-dev libdb++-dev libdb-dev libdb5.3 libdb5.3++ libdb5.3++-dev libdb5.3-dbg libdb5.3-dev
echo "[$(date)] 64-bit libraries installation completed."

# Configure timezone data
echo "[$(date)] Starting to configure timezone data..."
dpkg-reconfigure tzdata
echo "[$(date)] Timezone data configuration completed."

# Automatically remove unnecessary packages
echo "[$(date)] Starting to automatically remove unnecessary packages..."
apt autoremove
echo "[$(date)] Unnecessary packages removed."

# Completion
echo "[$(date)] Script execution completed. All operations were successfully completed."
