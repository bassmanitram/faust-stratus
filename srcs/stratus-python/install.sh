set -eu
#
# A script to install the stratus.py class as a standard Python package
#
# See build/tester.py for usage
#
cd $(realpath $(dirname $0))

echo "Installing Python class for Stratus Faust effects libraries"
#
# How to hack python install when you can't rely on pip or even setuptools being present
#
python_path=( $(python3 -c 'import sys
print(" ".join(sys.path))') )

for ((i=${#python_path[@]}-1; i>=0; i--)); do
	if [[ -d "${python_path[i]}" ]]; then
		PYTHON_DIR="${python_path[i]}"
		break
	fi
done

[[ "$PYTHON_DIR" ]] || { echo "Could not establish install location for Stratus Python components"; exit 1; }

if [[ -w "$PYTHON_DIR" ]]; then
	cp -r stratus "$PYTHON_DIR"
elif which sudo > /dev/null; then
	sudo cp -r stratus "$PYTHON_DIR"
else
	echo "Could not install for Stratus Python components into $PYTHON_DIR. Permission denied."
	exit 1
fi
echo "Stratus Python components installed into $PYTHON_DIR"
