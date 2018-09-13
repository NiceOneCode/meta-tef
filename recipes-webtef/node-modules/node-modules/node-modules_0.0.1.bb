DESCRIPTION = "Copies Node Modules files to the image"
LICENSE = "CLOSED"

PV = '0.0.1'
PR = "001"

SRC_URI = "file://node_modules.tar.gz"
INHIBIT_PACKAGE_STRIP = "1"

do_install() {
  install -d ${D}/usr/
  install -d ${D}/usr/lib/
  install -d ${D}/usr/lib/node_modules/

  install -d ${D}/home/
  install -d ${D}/home/root/
  install -d ${D}/home/root/TEF
  install -d ${D}/home/root/TEF/tef


  cp -rf ${WORKDIR}/node_modules ${D}/usr/lib

  rm -rf ${D}/home/root/TEF/node_modules
  rm -rf ${D}/home/root/TEF/tef/node_modules

  ln -sf /usr/lib/node_modules ${D}/home/root/TEF
  ln -sf /usr/lib/node_modules ${D}/home/root/TEF/tef

}


FILES_${PN} += " \
                 /usr/lib \
                 /home/root/TEF \
                 /home/root/TEF/ \
                 /home/root/TEF/tef \
                 "
