import sys, stratus
from stratus import Effect
from ctypes import *

effect = Effect(sys.argv[1])

test_floats =    [1.0,2.1,3.2,4.3,5.4,6.5,7.6,8.7,9.8]
zeroed_floats =  [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
doubled_floats = [2.0,4.2,6.4,8.6,10.8,13.0,15.2,17.4,19.6]

#
# The tester has one knob and one 2-state switch, 
# both should be set to zero by default
#
assert effect.knobCount == 1
assert effect.switchCount == 1
assert effect.getKnob(0) == 0
assert effect.getSwitch(0) == 0

#
# So, running the effect on our test buffer should result
# in a buffer full of floats of zero
#
output_floats = effect.compute(test_floats)
assert output_floats == zeroed_floats

#
# Set the knob to '1' and we should still get zero
#
effect.setKnob(0, 1)
output_floats = effect.compute(test_floats)
assert output_floats == zeroed_floats

#
# Set the switch to '1' and we should now get the same back
#
effect.setSwitch(0, 1)
output_floats = effect.compute(test_floats)
assert test_floats == test_floats

#
# Set the knob to '2' and we should get doubled floats (rounded :))
#
effect.setKnob(0, 2)
output_floats = [round(output_float, 1) for output_float in effect.compute(test_floats)]
assert output_floats == doubled_floats

#
# And THAT should be enough to prove that we have got it right!
#
print("All tests successful")
