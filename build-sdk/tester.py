import sys, stratus
from stratus import Effect

effect = stratus.Effect("./tester.so")
print("Successfully initialized ", effect.getName())

test_floats = b''
zeroed_floats = b''
doubled_floats = b''
output_floats = b''

#
# The tester has one knob and one 2-state switch, both set to zero
# by default
#
assert effect.knobCount == 1
assert effect.switchCount == 1
assert effect.getKnob(0) == 0
assert effect.getSwitch(0) == 0

#
# So, running the effect on our test buffer SHOULD result
# in a buffer full of floats of zero
#
effect.compute(len(test_floats), test_floats, output_floats)
assert output_floats == zeroed_floats

#
# Set the knob to '1' and we should still get zero
#
effect.compute(len(test_floats), test_floats, output_floats)
assert output_floats == zeroed_floats

#
# Set the switch to '1' and we should now get the same back
#
effect.compute(len(test_floats), test_floats, output_floats)
assert output_floats == test_floats

#
# Set the knob to '2' and we should get doubled floats
#
effect.compute(len(test_floats), test_floats, output_floats)
assert output_floats == doubled_floats

#
# And THAT should be enough to prove that we have got it right!
#
