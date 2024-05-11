#
# This script must ONLY be run from the SDK build docker file
#
set -eu
cd ${WORKDIR:?WORKDIR was not set by the caller}
STAGEDIR1=${WORKDIR}/stage1
STAGEDIR2=${WORKDIR}/stage2
FAUST_BIN_DIR=${FAUST_BIN_DIR:?FAUST_BIN_DIR was not set by the caller}
STAGE_FAUST_BIN_DIR=${FAUST_BIN_DIR#/}
FAUST_ARCH_DIR=${FAUST_ARCH_DIR:?FAUST_ARCH_DIR was not set by the caller}
STAGE_FAUST_ARCH_DIR=${FAUST_ARCH_DIR#/}
FAUST_LIB_DIR=${FAUST_LIB_DIR:?FAUST_LIB_DIR was not set by the caller}
STAGE_FAUST_LIB_DIR=${FAUST_LIB_DIR#/}

mkdir -p ${STAGEDIR1}/${STAGE_FAUST_BIN_DIR}
mkdir -p ${STAGEDIR1}${HOME}

cd ${STAGEDIR1}
mkdir -p ${STAGE_FAUST_ARCH_DIR} 
mkdir -p usr/local/lib/python3.5/
mkdir -p usr/local/lib/python3.8/
cp ${WORKDIR}/tests/* .${HOME}
cp -a ${FAUST_BIN_DIR}/faust ${FAUST_BIN_DIR}/*[Ss]tratus* ${STAGE_FAUST_BIN_DIR}
cp -a ${FAUST_ARCH_DIR}/*.lib ${FAUST_ARCH_DIR}/*[Ss]tratus* ${STAGE_FAUST_ARCH_DIR}
cp -a ${FAUST_LIB_DIR}/*[Ff]aust* ${STAGE_FAUST_LIB_DIR}
cp -a /usr/local/lib/python3.7/dist-packages usr/local/lib/python3.5/ 
cp -a /usr/local/lib/python3.7/dist-packages usr/local/lib/python3.8/ 
cp -a ${GLIBC_2_7_DIR} .

mkdir -p ${STAGEDIR2}/local/scripts
tar -czf ${STAGEDIR2}/faust-stratus-sdk-armv7.tgz *

#
# Now we add the bits and bobs for installing that TGZ on the Stratus as well as for a local install
#
cd ${WORKDIR}
cp --preserve=mode ./ci/scripts/install-stratus-faust.sh ${STAGEDIR2}/local/scripts
cp --preserve=mode ./ci/scripts/install-stratus-python.sh ${STAGEDIR2}/local/scripts
cp -r --preserve=mode ./tests ${STAGEDIR2}/local/
cp --preserve=mode -r ./srcs ${STAGEDIR2}/local
mv ${STAGEDIR2}/local/srcs/install/* ${STAGEDIR2}
rmdir ${STAGEDIR2}/local/srcs/install

cd ${STAGEDIR2}
zip -r ${HOME}/faust-stratus.zip *
