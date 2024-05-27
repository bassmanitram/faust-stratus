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

mkdir -p ${STAGEDIR1}/fs/${STAGE_FAUST_BIN_DIR}
mkdir -p ${STAGEDIR1}/fs/${STAGE_FAUST_LIB_DIR}
mkdir -p ${STAGEDIR1}/fs/${STAGE_FAUST_ARCH_DIR} 

cd ${STAGEDIR1}/fs
cp -a ${FAUST_BIN_DIR}/faust ${FAUST_BIN_DIR}/faustpath ${STAGE_FAUST_BIN_DIR}
cp -a ${FAUST_ARCH_DIR}/*.lib ${STAGE_FAUST_ARCH_DIR}
cp -a ${FAUST_LIB_DIR}/*[Ff]aust* ${STAGE_FAUST_LIB_DIR}
cp -a ${GLIBC_2_7_DIR} .

cd ${STAGEDIR1}
cp -r --preserve=mode ${WORKDIR}/srcs/stratus-python .
cp -r --preserve=mode ${WORKDIR}/srcs/faust-stratus .
rm ./faust-stratus/bin/stratus-build-effect
cp -r --preserve=mode ${WORKDIR}/tests .

mkdir -p ${STAGEDIR2}/local
tar -czf ${STAGEDIR2}/faust-stratus-sdk-armv7.tgz *

#
# Now we add the bits and bobs for installing that TGZ on the Stratus as well as for a local install
#
cd ${WORKDIR}
mv srcs/stratus-python ${STAGEDIR2}/local
mv srcs/faust-stratus ${STAGEDIR2}/local
mv ./tests ${STAGEDIR2}/local
mv srcs/install/* ${STAGEDIR2}

cd ${STAGEDIR2}
zip -r ${HOME}/faust-stratus.zip *
