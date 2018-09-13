DESCRIPTION = "DMG common startup scripts"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=2d04528cdadb070b355f9176c1149f90"

SECTION = "base"

PR = "r13"

SRC_URI = "file://*"

RDEPENDS_${PN} = "bc bash"

BBFILE_PRIORITY = "1"


FILES_${PN}=" /dmg \
              /dmg/etc \
              /dmg/conf \
              /etc \
              /etc/udhcpd.conf\
              /usr \
              /usr/bin/ \
              /usr/bin/dmgwf111.sh \
              /usr/bin/dmgwf111_C.sh \
              /usr/bin/dmg_node_main.sh \
              /usr/bin/wifi_on.sh \
              /usr/bin/wifi_off.sh \
              /usr/bin/lan_on.sh \
              /usr/bin/lan_off.sh \
              /usr/bin/daemon_controller.sh \
              /etc/init.d/dmgwf111_start.sh \
              /etc/init.d/dmg_node_main_start.sh \
              /etc/init.d/dmg-tef-updater.sh \
              /etc/init.d/alsa-first-boot-state.sh \
              /etc/rc5.d/S90dmg-tef-updater.sh \
              /etc/rc5.d/S90alsa-first-boot-state.sh \
              /etc/rc5.d/S96dmgwf111_start.sh \
              /etc/rc5.d/S97dmg_node_main_start.sh \
              /etc/bin/play_wav.sh \
              /etc/bin/rec_wav.sh \
              /etc/bin/web_on.sh \
              /etc/bin/web_off.sh \
"


#
#              /etc/rc5.d/S98iptable_captive_portal.sh
#              /etc/init.d/iptable_captive_portal.sh
#

do_install[nostamp]="1"

do_install () {
      install -d ${D}/usr
      install -d ${D}/usr/bin

      install -d ${D}/dmg
      install -d ${D}/dmg/conf
      install -d ${D}/dmg/etc

      install -m 0755 ${WORKDIR}/dmgwf111.sh                    ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/dmgwf111_C.sh                  ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/dmg_node_main.sh               ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/daemon_controller.sh           ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/udhcpd.conf                    ${D}/dmg/conf/

      install -d ${D}/etc
      install -m 0755 ${WORKDIR}/udhcpd.conf                    ${D}/etc/

      install -d ${D}${sysconfdir}/init.d

      install -m 0755 ${WORKDIR}/dmg-tef-updater.sh              ${D}${sysconfdir}/init.d
      chmod a+x ${D}${sysconfdir}/init.d/dmg-tef-updater.sh

      update-rc.d -r ${D} dmg-tef-updater.sh start 90 5 .

      install -m 0755 ${WORKDIR}/alsa-first-boot-state.sh              ${D}${sysconfdir}/init.d
      chmod a+x ${D}${sysconfdir}/init.d/alsa-first-boot-state.sh

      update-rc.d -r ${D} alsa-first-boot-state.sh start 90 5 .

      install -m 0755 ${WORKDIR}/dmgwf111_start.sh              ${D}${sysconfdir}/init.d
      chmod a+x ${D}${sysconfdir}/init.d/dmgwf111_start.sh

      update-rc.d -r ${D} dmgwf111_start.sh start 96 5 .

      install -m 0755 ${WORKDIR}/dmg_node_main_start.sh              ${D}${sysconfdir}/init.d
      chmod a+x ${D}${sysconfdir}/init.d/dmg_node_main_start.sh

      update-rc.d -r ${D} dmg_node_main_start.sh start 97 5 .

#      install -m 0755 ${WORKDIR}/iptable_captive_portal.sh              ${D}${sysconfdir}/init.d
#      chmod a+x ${D}${sysconfdir}/init.d/iptable_captive_portal.sh
#
#      update-rc.d -r ${D} iptable_captive_portal.sh start 98 5 .


      install -m 0755 ${WORKDIR}/play_wav.sh                    ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/rec_wav.sh                     ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/web_on.sh                      ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/web_off.sh                     ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/wifi_on.sh                     ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/wifi_off.sh                    ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/lan_on.sh                      ${D}/usr/bin/
      install -m 0755 ${WORKDIR}/lan_off.sh                     ${D}/usr/bin/
}
