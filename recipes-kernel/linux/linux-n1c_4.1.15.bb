


require recipes-kernel/linux/linux-imx.inc
require recipes-kernel/linux/linux-dtb.inc

DEPENDS += "lzop-native bc-native"

LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"


export KERNEL_N1C_VERSION = "00.01.001"
export DEVICETREE_VERSION = "dmg-imx6d-00.01.001"

LOCALVERSION = "-${KERNEL_N1C_VERSION}"


# Put a local version until we have a true SRCREV to point to
SRCREV = "9e518484640e08a468dd050cc7de1641323e12ca"
SRCBRANCH = "master"

SRC_URI = " \
			git://github.com/NiceOneCode/linux-n1c.git;protocol=git;branch=master \
           	file://0001-add-geamx6ul-tef-dts.patch \
           	file://defconfig-${KERNEL_N1C_VERSION}"

S = "${WORKDIR}/git"

# We need to pass it as param since kernel might support more then one
# machine, with different entry points
#KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

KERNEL_EXTRA_ARGS_mx6 += "LOADADDR=0x10008000"

KERNEL_EXTRA_ARGS_mx6ul += "LOADADDR=0x80008000"

COMPATIBLE_MACHINE = "(mx6|mx6ul)"

LINUX_VERSION_EXTENSION="-n1c"

### Probably this is not important ...


addtask kernel_config before do_preconfigure after do_patch

do_kernel_config() {

    if [ -f "${WORKDIR}/defconfig" ]
    then
        rm -fr ${WORKDIR}/defconfig
    fi

    cp -fr ${WORKDIR}/defconfig-${KERNEL_N1C_VERSION} ${WORKDIR}/defconfig

}


# Function introduce to remove into the sting version
# the extra code append by default

kernel_conf_variable() {
        CONF_SED_SCRIPT="$CONF_SED_SCRIPT /CONFIG_$1[ =]/d;"
        if test "$2" = "n"
        then
                echo "# CONFIG_$1 is not set" >> ${B}/.config
        else
                echo "CONFIG_$1=$2" >> ${B}/.config
        fi
}


addtask kernel_compile before do_compile after do_configure

do_kernel_compile() {
       kernel_conf_variable LOCALVERSION "\"${LOCALVERSION}\""
       kernel_conf_variable LOCALVERSION_AUTO n
       if [ -e ${WORKDIR}/git/.scmversion ];
        then
				rm -f ${WORKDIR}/git/.scmversion
       fi
       touch ${WORKDIR}/git/.scmversion

cat <<END >${WORKDIR}/git/arch/arm/boot/dts/dmg-dtversion.h

#ifndef __DTS_VERSION_H
#define __DTS_VERSION_H

#define DMG_DT_VERSION     "${DEVICETREE_VERSION}"

#endif /* __DTS_VERSION_H */
END


}
