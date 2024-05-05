#
# A script for building a Stratus effect from a Faust DSP file
# If the script is run on the Stratus, the compile has all the
# recommended optimization options. Otherwise we give no
# hardware-specific options.
#
EFFECT=${1:?First argument must be the Faust DSP file path}

cd $(dirname $0)

arch=$(gcc -march=native -Q --help=target|grep '^  -marc' | awk '{print $2}')
if [[ "$arch" == "armv7-a" ]]; then
	# We are on the pedal (or something that looks like it)
	opts="-march=armv7-a -mtune=cortex-a8 -mfloat-abi=hard -mfpu=neon -ftree-vectorize -ffast-math"
else
	# We are off the pedal - so this is probably just a test build
	opts=""
fi

EFFECT_DIR=$(dirname $EFFECT)
EFFECT_STEM=$(basename $EFFECT '.dsp')

faust -a ../stratus.cpp $EFFECT -o /tmp/$EFFECT_STEM.cpp \
&& g++ -fPIC -DCINTERFACE -shared -O3 -g $opts /tmp/$EFFECT_STEM.cpp -o "$EFFECT_DIR/$EFFECT_STEM".so
