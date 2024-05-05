from ctypes import *
from sys import platform, argv

def loadStratus(name):

    shared_lib_path = './'+name+'.so'

    EffectHandle = POINTER(c_char)
    FloatPointer = POINTER(c_float)
    stratus = CDLL(shared_lib_path)
    stratus.create.restype = EffectHandle

    stratus.stratusSetName.argtypes = [EffectHandle, c_char_p]
    stratus.stratusGetName.argtypes = [EffectHandle]
    stratus.stratusGetName.restype = c_char_p
    stratus.stratusGetVersion.argtypes = [EffectHandle]
    stratus.stratusGetVersion.restype = c_char_p
    stratus.stratusGetKnobCount.argtypes = [EffectHandle]
    stratus.stratusGetKnobCount.restype= c_uint
    stratus.stratusSetKnob.argtypes = [EffectHandle, c_uint, c_float]
    stratus.stratusGetKnob.argtypes = [EffectHandle, c_uint]
    stratus.stratusGetKnob.restype = c_float
    stratus.stratusSetSwitch.argtypes = [EffectHandle, c_uint, c_uint]
    stratus.stratusGetSwitch.argtypes = [EffectHandle, c_uint]
    stratus.stratusGetSwitch.restype= c_uint
    stratus.stratusSetStompSwitch.argtypes = [EffectHandle, c_uint]
    stratus.stratusGetStompSwitch.argtypes = [EffectHandle]
    stratus.stratusGetStompSwitch.restype = c_uint
    stratus.stratusStompSwitchPressed.argtypes = [EffectHandle, c_uint, FloatPointer, FloatPointer]
    stratus.stratusCompute.argtypes = [EffectHandle, c_uint, FloatPointer, FloatPointer]
    return stratus

class Stratus:
    def __init__(self,name):
        self.stratus = loadStratus(name)
        self.effect = self.stratus.create()
        self.knobCount = self.stratus.stratusGetKnobCount(self.effect)

    def setName(self,name):
        self.stratus.stratusSetName(self.effect, name)
    def getName(self):
        return self.stratus.stratusGetName(self.effect)
    def getVersion(self):
        return self.stratus.stratusGetVersion(self.effect)
    def getKnobCount(self):
        return self.knobCount
    def setKnob(self,index, value):
        self.stratus.stratusSetKnob(self.effect, index, value)
    def getKnob(self,index):
        return self.stratus.stratusGetKnob(self.effect, index)
    def setSwitch(self,index,value):
        self.stratus.stratusSetSwitch(self.effect, index, value)
    def getSwitch(self,index):
        return self.stratus.stratusGetSwitch(self.effect, index)
    def setStompSwitch(self,value):
        self.stratus.stratusSetStompSwitch(self.effect, value)
    def getStompSwitch(self):
        return self.stratus.stratusGetStompSwitch(self.effect)
    def stompSwitchPressed(self,count, inputs, outputs):
        self.stratus.stratusStompSwitchPressed(self.effect, count, inputs, outputs)
    def compute(self,count, inputs, outputs):
        self.stratus.stratusCompute(self.effect, count, inputs, outputs)

effect = Stratus(argv[1])
print("Successfully initialized ", effect.getName())
knobCount = effect.getKnobCount()

while knobCount > 0:
    knobCount-=1
    knobValue = effect.getKnob(knobCount)
    effect.setKnob(knobCount, knobValue + 0.5)
    assert effect.getKnob(knobCount) == (knobValue + 0.5)

stompSwitch = effect.getStompSwitch()
newStompSwitch = 1 if stompSwitch == 0 else 0
effect.setStompSwitch(newStompSwitch)
assert effect.getStompSwitch() == newStompSwitch

print("Successfully tested ", effect.getName())
