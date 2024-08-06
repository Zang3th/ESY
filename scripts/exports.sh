echo "Preparing environment for this shell"
HOMEDIR=$(echo ~)

export ARCH=arm
export CROSS_COMPILE=arm-linux-
export PATH="${PATH}:${HOMEDIR}/Dev/ESY/buildroot/output/host/usr/bin/"
# export SYSROOT=${HOMEDIR}/Dev/ESY/buildroot/output/host/usr/arm-buildroot-linux-uclibcgnueabihf
export KERNELDIR=${HOMEDIR}/Dev/ESY/kernel/linux
export KERNEL=kernel7l
