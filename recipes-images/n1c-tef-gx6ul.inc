DESCRIPTION = "DMG images base configs"

LICENSE = "MIT"

inherit core-image

# add remote debugging missing subprocess (https://community.freescale.com/thread/309578)
TOOLCHAIN_HOST_TASK_append = " nativesdk-python-subprocess "

EXTRA_IMAGE_FEATURES = " debug-tweaks ssh-server-openssh tools-debug package-management "

IMAGE_INSTALL_append = " \
	binutils \
	iproute2 \
	mtd-utils \
	mtd-utils-ubifs \
	devmem2 \
	i2c-tools \
	imx-kobs \
	fw-nandautosize \
	minicom \
	ethtool \
	dosfstools \
	e2fsprogs \
	usbutils \
	cpufrequtils \
	geamx6ul-tef-startup \
	netperf \
	wf111-driver \
	wireless-tools \
	wpa-supplicant \
	u-boot-eng-fw-utils \
	util-linux \
"
#ALSA PACKAGES
IMAGE_INSTALL_append = " \
	alsa-lib \
	alsa-oss \
	alsa-plugins \
	alsa-state \
	alsa-tools \
	alsa-utils \
	alsa-utils-scripts \
	flite-alsa \
	imx-alsa-plugins \
"

#DMG PACKAGES
IMAGE_INSTALL_append = " \
	dmg-common-scripts \
	dmg-base-scripts \
	node \
	packagegroup-qt5-qtcreator-debug \
	fuse \
	fuse-utils \
	dmg-wdnet-manager \
	dhcp-server \
	dhcp-client \
	dnsmasq \
	opkg \
	opkg-utils \
  procps \
"
#	iptables
#	conntrack-tools

#Node Exstension
IMAGE_INSTALL_append = " \
    node-modules \
    node-serial \
	node-app \
	node-backup \
"
#
IMAGE_INSTALL_remove ="qt3d nativesdk-qt3d qt3d-native ruby-native"
