set -eu
python3 -c "try:
    __import__('imp').find_module('pip')
    exit(0)
except ImportError:
    exit(1)
" || { echo "Cannot install python tools: pip not found in target system"; exit; }
python3 -m pip install chaos-stratus
echo "Stratus Python components installed"
