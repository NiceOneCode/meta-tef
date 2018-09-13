DESCRIPTION = "Dmg sdk image recipe"

require ../n1c-tef-gx6ul.inc

inherit populate_sdk_base
inherit populate_sdk_${@base_contains('MACHINE', 'dmg6ul', 'dmg-qt5_geam6ul', 'dmg-qt5_icore', d)}


export IMAGE_BASENAME_mx6ul = "dmg-sdk-imx6ul"

SDKIMAGE_FEATURES = "dev-pkgs dbg-pkgs staticdev-pkgs"

IMAGE_INSTALL_append = " \
	glibc-staticdev \
 "

IMAGE_INSTALL_remove ="qt3d nativesdk-qt3d qt3d-native ruby-native"
