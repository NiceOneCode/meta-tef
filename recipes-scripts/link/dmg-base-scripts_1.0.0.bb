DESCRIPTION = "DMG configuration scripts for DINER base image"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=2d04528cdadb070b355f9176c1149f90"

SECTION = "base"

PR = "r3"


#####################################
# dmg_base_scripts
# 2016-10-22
# versione iniziale
#
####################################

#####################################
# dmg_base_scripts
# 2017-01-25
# introdotti link WEB ON/OFF
# introdotti link Play/Rec Wave
#
####################################

SRC_URI = "file://*"

do_install () {
	install -d ${D}${sysconfdir}/
	install -d ${D}${sysconfdir}/init.d/
	install -d ${D}${sysconfdir}/profile.d/

	install -m 0755 ${WORKDIR}/dmg_common_symlinks.sh         ${D}${sysconfdir}/init.d/

	update-rc.d -r ${D} dmg_common_symlinks.sh                start 10 2 3 4 5 .
}
