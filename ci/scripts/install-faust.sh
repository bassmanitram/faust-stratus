set -eu

EUID=${EUID:-$(id -u)}
#
# This builds Faust with only CPP output and the OSC static library
#
# It is installed in /usr/local
#
# This script can be run seperately to have a local installation of faust
# with the Stratus commponents added
#
cd ${WORKDIR:-$(realpath $(dirname $0))/../..}
FAUST_VERSION=${FAUST_VERSION:-2.72.14}
FAUST_PREFIX=${FAUST_PREFIX:-/usr/local}

#
# Dash doens't need - or even support - the "standard" Bash 'function' keyword
# Bash supports it's absence - so this should be OK in Bash and Dash
#
install_faust() {
	echo "Installing Faust"
	curl -L https://github.com/grame-cncm/faust/releases/download/${FAUST_VERSION}/faust-${FAUST_VERSION}.tar.gz -o faust.tgz
	tar -xzf faust.tgz
	cp -a srcs/faust-build/* faust-${FAUST_VERSION}
	cd faust-${FAUST_VERSION}/build
	make faust PREFIX=${FAUST_PREFIX} BACKENDS=cpp-embeded-only.cmake TARGETS=regular-nohttpd.cmake

	if [ -w "$FAUST_PREFIX" ]; then
		make install
	else
		sudo make install
	fi
}

CONDITIONAL=${1:-false}

if [ "$CONDITIONAL" != "false" ]; then
	if ! which faust; then
		install_faust
	fi
else
	install_faust
fi

cd $WORKDIR
#
# Find the bin and architecture file directories. If already
# set, use those, otherwise discover them. If the version of
# of Faust that has been built is not executable in the environment
# in which this script is run (e.g. from the SDK build docker file)
# then these values MUST be preset by the caller.
#
FAUST_BIN_DIR=${FAUST_BIN_DIR:-$(dirname $(which faust))}
FAUST_ARCH_DIR=${FAUST_ARCH_DIR:-$(faust --archdir)}

echo $FAUST_ARCH_DIR

if [ -w "$FAUST_ARCH_DIR" ]; then
	cp -a ./srcs/faust-stratus/architecture/* "$FAUST_ARCH_DIR"
else
	sudo cp -a ./srcs/faust-stratus/architecture/* "$FAUST_ARCH_DIR"
fi

if [ -w "$FAUST_BIN_DIR" ]; then
	cp -a ./srcs/faust-stratus/bin/* "$FAUST_BIN_DIR"
else
	sudo cp -a ./srcs/faust-stratus/bin/* "$FAUST_BIN_DIR"
fi
