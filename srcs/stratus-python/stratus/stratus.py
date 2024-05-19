from ctypes import *
from sys import argv

InstanceHandle = c_void_p
FloatPointer = POINTER(c_float)

class EffectLib:
    def __init__(self,so_file):
        lib = CDLL(so_file)
        self.create = lib.create
        try:
            self.getExtensions = lib.getExtensions
        except:
            self.getExtensions = None

        #
        # Using StratusExtensions
        #
        if self.getExtensions is not None:
            self.stratusGetEffectId = getattr(lib,"_ZN17StratusExtensions11getEffectIdEv")
            self.stratusGetName = getattr(lib,"_ZN17StratusExtensions7getNameEv")
            self.stratusGetVersion = getattr(lib,"_ZN17StratusExtensions10getVersionEv")
            self.stratusGetKnobCount = getattr(lib,"_ZN17StratusExtensions12getKnobCountEv")
            self.stratusGetSwitchCount = getattr(lib,"_ZN17StratusExtensions14getSwitchCountEv")
        else:
            self.stratusGetEffectId = None
            self.stratusGetName = None
            self.stratusGetVersion = None
            self.stratusGetKnobCount = None
            self.stratusGetSwitchCount = None

        #
        # Using the effect
        #
        self.stratusSetKnob = getattr(lib,"_ZN3dsp7setKnobEif")
        self.stratusGetKnob = getattr(lib,"_ZN3dsp7getKnobEi")
        self.stratusSetSwitch = getattr(lib,"_ZN3dsp9setSwitchEiNS_12SWITCH_STATEE")
        self.stratusGetSwitch = getattr(lib,"_ZN3dsp9getSwitchEi")
        self.stratusSetStompSwitch = getattr(lib,"_ZN3dsp14setStompSwitchENS_12SWITCH_STATEE")
        self.stratusGetStompSwitch = getattr(lib,"_ZN3dsp14getStompSwitchEv")
        self.stratusStompSwitchPressed = getattr(lib,"_ZN3dsp18stompSwitchPressedEiPfS0_")
        self.stratusCompute = getattr(lib,"_ZN3dsp7computeEiPfS0_")

        self.create.restype = InstanceHandle
        if self.getExtensions is not None:
            self.getExtensions.argtypes = [ InstanceHandle ]
            self.getExtensions.restype = InstanceHandle

            self.stratusGetEffectId.argtypes = [InstanceHandle]
            self.stratusGetEffectId.restype = c_char_p

            self.stratusGetName.argtypes = [InstanceHandle]
            self.stratusGetName.restype = c_char_p

            self.stratusGetVersion.argtypes = [InstanceHandle]
            self.stratusGetVersion.restype = c_char_p

            self.stratusGetKnobCount.argtypes = [InstanceHandle]
            self.stratusGetKnobCount.restype= c_uint

            self.stratusGetSwitchCount.argtypes = [InstanceHandle]
            self.stratusGetSwitchCount.restype= c_uint

        self.stratusSetKnob.argtypes = [InstanceHandle, c_uint, c_float]

        self.stratusGetKnob.argtypes = [InstanceHandle, c_uint]
        self.stratusGetKnob.restype = c_float

        self.stratusSetSwitch.argtypes = [InstanceHandle, c_uint, c_uint]

        self.stratusGetSwitch.argtypes = [InstanceHandle, c_uint]
        self.stratusGetSwitch.restype= c_uint

        self.stratusSetStompSwitch.argtypes = [InstanceHandle, c_uint]

        self.stratusGetStompSwitch.argtypes = [InstanceHandle]
        self.stratusGetStompSwitch.restype = c_uint

        self.stratusStompSwitchPressed.argtypes = [InstanceHandle, c_uint, FloatPointer, FloatPointer]
        self.stratusCompute.argtypes = [InstanceHandle, c_uint, FloatPointer, FloatPointer]


class Effect:
    def __init__(self,so_file):
        self.effect_lib = EffectLib(so_file)

        self.effect = self.effect_lib.create()
        if self.effect_lib.getExtensions is not None:
            effectExtensions = self.effect_lib.getExtensions(self.effect)
            self.knobCount = self.effect_lib.stratusGetKnobCount(effectExtensions)
            self.switchCount = self.effect_lib.stratusGetSwitchCount(effectExtensions)
            self.version = self.effect_lib.stratusGetVersion(effectExtensions).decode()
            self.name = self.effect_lib.stratusGetName(effectExtensions).decode()
            self.effectId = self.effect_lib.stratusGetEffectId(effectExtensions).decode()
            self.extensionsPresent = True
        else:
            self.extensionsPresent = False
            self.knobCount = -1
            self.switchCount = -1
            self.version = 0
            self.name = "Unknown"
            self.effectId = "Unknown"

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
