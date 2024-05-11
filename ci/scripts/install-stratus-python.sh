set -eu
#
# A script to install the stratus.py class as a standard Python package
#
# See build/tester.py for usage
#
cd ${WORKDIR:-$(realpath $(dirname $0))/..}

echo "Installing Python class for Stratus Faust effects libraries"
python3 -m pip install ./srcs/stratus-python
