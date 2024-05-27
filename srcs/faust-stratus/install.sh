set -eu
#
# Install the Stratus-related files into their proper Faust locations
#

cd $(realpath $(dirname $0))
#
# Find the bin and architecture file directories. If already
# set, use those, otherwise discover them. If the version of
# of Faust that has been built is not executable in the environment
# in which this script is run (e.g. from the SDK build docker file)
# then these values MUST be preset by the caller.
#
FAUST_BIN_DIR=${FAUST_BIN_DIR:-$(dirname $(which faust))}
FAUST_ARCH_DIR=${FAUST_ARCH_DIR:-$(faust --archdir)}

echo "Installing Faust components to support the Chaos Stratus"

if [ -w "$FAUST_ARCH_DIR" ]; then
	rm -f "$FAUST_ARCH_DIR/stratus.cpp"
	cp -a ./architecture/* "$FAUST_ARCH_DIR"
else
	sudo rm -f "$FAUST_ARCH_DIR/stratus.cpp"
	sudo cp -a ./architecture/* "$FAUST_ARCH_DIR"
fi

if [ -w "$FAUST_BIN_DIR" ]; then
	cp -a ./bin/* "$FAUST_BIN_DIR"
else
	sudo cp -a ./bin/* "$FAUST_BIN_DIR"
fi
