#
# A script for building a Stratus effect from a Faust DSP file
# If the script is run on the Stratus, the compile has all the
# recommended optimization options and no C interface. Otherwise 
# we give no hardware-specific options, and we include the C 
# interface
#
EFFECT=${1:?First argument must be the Faust DSP file path}

EFFECT=$(realpath $EFFECT)

cd $(realpath $(dirname $0))

arch=$(gcc -march=native -Q --help=target|grep '^  -marc' | awk '{print $2}')
if [[ "$arch" == "armv7-a" ]]; then
	# We are on the pedal (or something that looks like it)
	# Max optimization and no C interface
	opts="-march=armv7-a -mtune=cortex-a8 -mfloat-abi=hard -mfpu=neon -ftree-vectorize -ffast-math"
else
	# We are off the pedal - so this is probably a test build so we include the C interface
	opts="-DCINTERFACE"
fi

EFFECT_DIR="$(dirname "$EFFECT")"
EFFECT_STEM="$(basename "$EFFECT" ".dsp")"
EFFECT_CPP="$EFFECT_DIR/$EFFECT_STEM.cpp"
EFFECT_SO="$EFFECT_DIR/$EFFECT_STEM.so"

faust -a ../stratus.cpp "$EFFECT" -o "$EFFECT_CPP" \
&& g++ -fPIC -shared -O3 -g $opts "$EFFECT_CPP" -o "$EFFECT_SO"
