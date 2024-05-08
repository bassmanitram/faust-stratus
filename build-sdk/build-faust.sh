set -euo

cd ${WORKDIR}

curl -L https://github.com/grame-cncm/faust/releases/download/${FAUST_VERSION}/faust-${FAUST_VERSION}.tar.gz -o faust.tgz
tar -xzf faust.tgz
cp -a build/* faust-${FAUST_VERSION}/build/
cd faust-${FAUST_VERSION}/build
make faust BACKENDS=cpp-embeded-only.cmake TARGETS=regular-nohttpd.cmake
make install
