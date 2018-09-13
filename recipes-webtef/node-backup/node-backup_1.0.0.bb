SUMMARY = ""
DESCRIPTION = ""

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=2d04528cdadb070b355f9176c1149f90"

SECTION = "base"

RDEPENDS_${PN} += "node-app"

SRC_URI = "file://*"

PR = "r0"

IPK_FOLDER = "${DEPLOY_DIR}/ipk/cortexa7hf-neon"

FILES_${PN}=" /dmg \
	      /dmg/backup \
"

BBFILE_PRIORITY = "1"

do_configure () {
	echo "ipk_folder=$IPK_FOLDER"
	
	for i in `seq 1 1200`;
	do
		sleep 1
		
		if [ ! -d ${IPK_FOLDER} -o ! -e ${IPK_FOLDER}/node-app_*.ipk ]; then
			continue
		fi	

		break
	done


	if [ ! -d ${IPK_FOLDER} -o ! -e ${IPK_FOLDER}/node-app_*.ipk ]; then
		bberror "---------------------------------------------------------------------------"
		bberror ">> 	   Before build this recipe please cook the node-app              <<"
		bberror "---------------------------------------------------------------------------"
		bberror "---------------------------------------------------------------------------"
		bberror "             bitbake -c cleansstate node-app                               "
		bberror "             bitbake node-app               				    "
		bberror "---------------------------------------------------------------------------"
		bberror "---------------------------------------------------------------------------"
		exit 0
	fi

	cp -f ${IPK_FOLDER}/node-app_*.ipk   ${WORKDIR}/
}


do_install () {
	
	install -d ${D}/dmg
	install -d ${D}/dmg/backup
	
	
	if [ -e ${WORKDIR}/node-app*.ipk ]; then
		install -m 0600 ${WORKDIR}/node-app_*.ipk ${D}/dmg/backup
	fi

}
