DESCRIPTION = "Copies Node Application files to the image"
LICENSE = "CLOSED"


SRC_URI = "file://TEF_${PV}.tar.gz"
INHIBIT_PACKAGE_STRIP = "1"

do_install() {
  install -d ${D}/home/
  install -d ${D}/home/root/

  cp -rf ${WORKDIR}/TEF ${D}/home/root

  rm -rf ${D}/home/root/TEF/node_modules
  rm -rf ${D}/home/root/TEF/tef/node_modules

  ln -sf /usr/lib/node_modules ${D}/home/root/TEF
  ln -sf /usr/lib/node_modules ${D}/home/root/TEF/tef

}
FILES_${PN} += "/home/root/TEF"
