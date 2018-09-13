SUMMARY = "node-serial is a Serial Port binding for Node.js"
SECTION = "nodejs/module"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://README.md;md5=b9a59a0fefe195d10be82cf7f0a99788"

DEPENDS = "node-gyp-native node-modules"

# PV = "1.6.1+gitr${SRCPV}"
PV = "1.6.1"
PR = "002"

RDEPENDS_${PN}-zsh = "zsh"
RDEPENDS_${PN}-bash = "bash"

SRC_URI = "git://github.com/voodootikigod/node-serialport.git;branch=master;protocol=git"
SRCREV = "94b7bdcc087b820e10913c483bab3e2c54d103f2"
SRC_URI += " \
        file://0001-t1.patch \
        file://0002-t2.patch \
        file://0003-t3.patch \
        file://0004-serialport-list-terminal-js.patch \
        file://0007-parsers-js.patch \
        file://0008-serialport-js.patch \
"
#  file://0006-package-json.patch

S = "${WORKDIR}/git"


do_configure() {
    export LD="${CXX}"
    export GYP_DEFINES="sysroot=${STAGING_DIR_HOST}"
    echo  "##### ==> $GYP_DEFINES"
    export NODE_PATH="${STAGING_DIR_HOST}${libdir}/node_modules"
    echo  "##### ==>$NODE_PATH"
    node-pre-gyp --target_arch=${TARGET_ARCH} --target_platform=linux configure
}

do_compile() {
    export LD="${CXX}"
    export GYP_DEFINES="sysroot=${STAGING_DIR_HOST}"
    export NODE_PATH="${STAGING_DIR_HOST}${libdir}/node_modules"
    node-pre-gyp --target_arch=${TARGET_ARCH} --target_platform=linux build
}

do_install() {

    npm install --no-cache

    install -d ${D}/usr/
    install -d ${D}${libdir}
    install -d ${D}${libdir}/node_modules
    install -d ${D}${libdir}/node_modules/serialport/
    install -d ${D}${libdir}/node_modules/serialport/build
    install -d ${D}${libdir}/node_modules/serialport/bin
    install -d ${S}/build/Release/


    install -m 0664 ${S}/bin/serialport-list.js      ${D}${libdir}/node_modules/serialport/bin/
    install -m 0664 ${S}/bin/serialport-terminal.js  ${D}${libdir}/node_modules/serialport/bin/
    install -m 0664 ${S}/package.json               ${D}${libdir}/node_modules/serialport/
    install -m 0664 ${S}/parsers.js                 ${D}${libdir}/node_modules/serialport/
    install -m 0664 ${S}/serialport.js              ${D}${libdir}/node_modules/serialport/

    #problematic line if the cp lines are uncommented the bitbake displayed to add RDEPEND /bin/bash and /bin/zsh
    cp -r  ${S}/build/*         ${D}${libdir}/node_modules/serialport/build/
    # cp -r  ${S}/node_modules/*  ${D}${libdir}/node_modules
}


FILES_${PN} += "${libdir}/node_modules \
        ${libdir}/node_modules/serialport \
        ${libdir}/node_modules/serialport/build \
"

FILES_${PN}-dbg += "${libdir}/node_modules/.debug \
        ${libdir}/node_modules/serialport/.debug \
        ${libdir}/node_modules/serialport/build/.debug \
        ${libdir/node_modules/serialport/build/Release/.debug \
"
#workaround to avoid QA's problems, must be fixed in the second round
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"
