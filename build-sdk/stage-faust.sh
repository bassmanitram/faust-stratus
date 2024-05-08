set -euo
cd ${WORKDIR}
STAGEDIR=${WORKDIR}/stage
mkdir -p ${STAGEDIR}/usr/local/bin
cd ${STAGEDIR}
mkdir -p usr/local/lib
mkdir -p usr/local/share/faust
cp -a /usr/local/bin/faust usr/local/bin
cp -a /usr/local/lib/libOSCFaust.a usr/local/lib
cp -a /usr/local/share/faust/*.lib usr/local/share/faust
cp ${WORKDIR}/stratus.py usr/local/bin 
cp ${WORKDIR}/faust2stratus usr/local/bin 
cp ${WORKDIR}/stratus.cpp usr/local/share/faust
cp -a ${GLIBC_2_7_DIR} .

tar -czf /root/faust-stratus.tgz *
