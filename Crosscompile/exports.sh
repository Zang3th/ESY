echo "Preparing environment for this shell"
HOMEDIR=`echo ~`

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-
export SYSROOT=${HOMEDIR}/Dev/ESY/buildroot/output/host/usr/arm-buildroot-linux-uclibcgnueabihf
export PATH="${PATH}:${HOMEDIR}:/Dev/ESY/buildroot/output/host/usr/bin/"
export KDIR=${HOMEDIR}/Dev/ESY/Kernel/linux_server/linux/
export KERNEL=kernel7l