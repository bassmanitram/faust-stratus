from ctypes import *
from sys import argv

EffectHandle = c_void_p
FloatPointer = POINTER(c_float)


def loadEffect(so_file):

    effect_lib = CDLL(so_file)
    effect_lib.create.restype = EffectHandle

    effect_lib.stratusGetEffectId.argtypes = [EffectHandle]
    effect_lib.stratusGetEffectId.restype = c_char_p

    effect_lib.stratusGetName.argtypes = [EffectHandle]
    effect_lib.stratusGetName.restype = c_char_p

    effect_lib.stratusGetVersion.argtypes = [EffectHandle]
    effect_lib.stratusGetVersion.restype = c_char_p

    effect_lib.stratusGetKnobCount.argtypes = [EffectHandle]
    effect_lib.stratusGetKnobCount.restype= c_uint

    effect_lib.stratusGetSwitchCount.argtypes = [EffectHandle]
    effect_lib.stratusGetSwitchCount.restype= c_uint

    effect_lib.stratusSetKnob.argtypes = [EffectHandle, c_uint, c_float]

    effect_lib.stratusGetKnob.argtypes = [EffectHandle, c_uint]
    effect_lib.stratusGetKnob.restype = c_float

    effect_lib.stratusSetSwitch.argtypes = [EffectHandle, c_uint, c_uint]

    effect_lib.stratusGetSwitch.argtypes = [EffectHandle, c_uint]
    effect_lib.stratusGetSwitch.restype= c_uint

    effect_lib.stratusSetStompSwitch.argtypes = [EffectHandle, c_uint]

    effect_lib.stratusGetStompSwitch.argtypes = [EffectHandle]
    effect_lib.stratusGetStompSwitch.restype = c_uint

    effect_lib.stratusStompSwitchPressed.argtypes = [EffectHandle, c_uint, FloatPointer, FloatPointer]
    effect_lib.stratusCompute.argtypes = [EffectHandle, c_uint, FloatPointer, FloatPointer]
    return effect_lib

class Effect:
    def __init__(self,so_file):
        self.effect_lib = loadEffect(so_file)
        self.effect = self.effect_lib.create()
        self.knobCount = self.effect_lib.stratusGetKnobCount(self.effect)
        self.switchCount = self.effect_lib.stratusGetSwitchCount(self.effect)

        self.version = self.effect_lib.stratusGetVersion(self.effect).decode()
        self.name = self.effect_lib.stratusGetName(self.effect).decode()
        self.effectId = self.effect_lib.stratusGetEffectId(self.effect).decode()

    def setKnob(self,index, value):
        self.effect_lib.stratusSetKnob(self.effect, index, value)
    def getKnob(self,index):
        return self.effect_lib.stratusGetKnob(self.effect, index)
    def setSwitch(self,index,value):
        self.effect_lib.stratusSetSwitch(self.effect, index, value)
    def getSwitch(self,index):
        return self.effect_lib.stratusGetSwitch(self.effect, index)
    def setStompSwitch(self,value):
        self.effect_lib.stratusSetStompSwitch(self.effect, value)
    def getStompSwitch(self):
        return self.effect_lib.stratusGetStompSwitch(self.effect)
    def stompSwitchPressed(self,inputs):
        count = len(inputs)
        input_floats = (c_float * count)(*inputs)
        output_floats = (c_float * count)(0.0)
        self.effect_lib.stratusStompSwitchPressed(self.effect, count, input_floats, output_floats)
        return [output_float for output_float in output_floats]
    def compute(self,inputs):
        count = len(inputs)
        input_floats = (c_float * count)(*inputs)
        output_floats = (c_float * count)(*inputs)
        self.effect_lib.stratusCompute(self.effect, count, input_floats, output_floats)
        return [output_float for output_float in output_floats]
    def stompSwitchPressedBuf(self,inputs):
        if len(inputs) % 4 != 0:
            raise ValueError("inputs must be a buffer of 4-byte floating point values")
        outputs = bytearray(len(inputs))
        count = len(inputs)//4
        input_floats = (c_float * count)(*inputs)
        output_floats = (c_float * count)(*inputs)

        self.effect_lib.stratusStompSwitchPressed(self.effect, c_uint(count), input_floats, output_floats)
        return outputs
    def computeBuf(self,inputs):
        if len(inputs) % 4 != 0:
            raise ValueError("inputs must be a buffer of 4-byte floating point values")
        outputs = bytearray(len(inputs))
        count = len(inputs)//4
        Buffer = c_float * count
        input_floats = Buffer.from_buffer(inputs)
        output_floats = Buffer.from_buffer(outputs)
        self.effect_lib.stratusCompute(self.effect, c_uint(count), input_floats, output_floats)
        return outputs
