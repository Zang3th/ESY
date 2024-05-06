echo "Preparing environment for this shell"
HOMEDIR=$(echo ~)

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-
export PATH="${PATH}:${HOMEDIR}/Dev/ESY/buildroot/output/host/usr/bin"
export KERNEL=kernel7l