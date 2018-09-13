DESCRIPTION = " Dmg fuse driver for devices power management "

LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://${WORKDIR}/src/main.cpp;md5=902d5b44f695b7a2bad28afbe6586ed9"


DEPENDS        += " fuse qtbase "
RDEPENDS_${PN} += " fuse fuse-utils "

inherit qmake5

PR = "r5"

SRC_URI = "file://* \
           file://src/dmg_wd_manager.pro "

S = "${WORKDIR}/src"

EXTRA_QMAKEVARS_PRE = "CONFIG+=release"

FILES_${PN}=" /dmg/fuse/wd \
              /usr/bin/dmg_wd_manager \
              /usr/bin/dmg_wd_manager.sh \
              /etc/init.d/dmg_wd_manager_start.sh \
              /etc/rc2.d/S93dmg_wd_manager_start.sh \
              /etc/rc3.d/S93dmg_wd_manager_start.sh \
              /etc/rc4.d/S93dmg_wd_manager_start.sh \
              /etc/rc5.d/S93dmg_wd_manager_start.sh \
"


do_install_append () {
          install -d ${D}/dmg/fuse/wd
          install -d ${D}/usr/bin

          install -m 0755 ${WORKDIR}/build/dmg_wd_manager ${D}/usr/bin/

          install -m 0755 ${WORKDIR}/dmg_wd_manager.sh ${D}/usr/bin/
          chmod a+x ${D}/usr/bin/dmg_wd_manager.sh

          install -d ${D}${sysconfdir}/init.d

          install -m 0755 ${WORKDIR}/dmg_wd_manager_start.sh ${D}${sysconfdir}/init.d
          chmod a+x ${D}${sysconfdir}/init.d/dmg_wd_manager_start.sh

          update-rc.d -r ${D} dmg_wd_manager_start.sh start 93 2 3 4 5 .
}
