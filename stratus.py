from ctypes import *
from sys import argv

def loadEffect(name):

    shared_lib_path = name

    EffectHandle = POINTER(c_char)
    FloatPointer = POINTER(c_float)
    effect_lib = CDLL(shared_lib_path)
    effect_lib.create.restype = EffectHandle

    effect_lib.stratusSetName.argtypes = [EffectHandle, c_char_p]
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
    def __init__(self,name):
        self.effect_lib = loadEffect(name)
        self.effect = self.effect_lib.create()
        self.knobCount = self.effect_lib.stratusGetKnobCount(self.effect)
        self.switchCount = self.effect_lib.stratusGetSwitchCount(self.effect)

    def setName(self,name):
        self.effect_lib.stratusSetName(self.effect, name)
    def getName(self):
        return self.effect_lib.stratusGetName(self.effect)
    def getVersion(self):
        return self.effect_lib.stratusGetVersion(self.effect)
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
    def stompSwitchPressed(self,count, inputs, outputs):
        self.effect_lib.stratusStompSwitchPressed(self.effect, count, inputs, outputs)
    def compute(self,count, inputs, outputs):
        self.effect_lib.stratusCompute(self.effect, count, inputs, outputs)
