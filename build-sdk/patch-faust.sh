##
## THIS is the awkward bit - well, the fix to the awkwardness...
##
## Basically we load up the glibc-related libraries that we need for the
## busster-compiled faust to work on Debian 9 ARM, and we patch the faust
## binary to load them from there instead of from the standard system location
##
set -euo
cd ${WORKDIR}
curl -L https://github.com/NixOS/patchelf/releases/download/${PATCHELF_VERSION}/patchelf-${PATCHELF_VERSION}-x86_64.tar.gz -o patchelf.tgz
cd /
tar -xzf ${WORKDIR}/patchelf.tgz
mkdir ${GLIBC_2_7_DIR}
cd ${GLIBC_2_7_DIR}
tar -xzf ${WORKDIR}/glib-2_7-armv7.tgz
cd /usr/local/bin
patchelf --set-interpreter ${GLIBC_2_7_DIR}/ld-linux-armhf.so.3 faust
patchelf --set-rpath ${GLIBC_2_7_DIR} faust