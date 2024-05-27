set -eu

#
# This builds Faust with only CPP output and the OSC static library
#
# It is installed in /usr/local
#
cd ${WORKDIR:-$(realpath $(dirname $0))/../..}
FAUST_VERSION=${FAUST_VERSION:-2.72.14}
FAUST_PREFIX=${FAUST_PREFIX:-/usr/local}

echo "Installing Faust"
curl -L https://github.com/grame-cncm/faust/releases/download/${FAUST_VERSION}/faust-${FAUST_VERSION}.tar.gz -o faust.tgz
tar -xzf faust.tgz
cp -a srcs/faust-build/* faust-${FAUST_VERSION}
cd faust-${FAUST_VERSION}/build
make faust PREFIX=${FAUST_PREFIX} BACKENDS=cpp-embedded-only.cmake TARGETS=regular-nohttpd.cmake
make install
