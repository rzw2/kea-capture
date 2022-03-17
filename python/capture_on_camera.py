# This is a hacky way to get on camera python working, as OpenMP linking isn't working
import numpy as np

import ctypes
from ctypes.util import find_library
tmp_lib = find_library("gomp")
if tmp_lib != None:
    ctypes.CDLL(tmp_lib, mode=ctypes.RTLD_GLOBAL)
    import chronoptics.tof as tof


csf_out = "capture_python.csf"
ndrop = 20
ncap = 10

proc = tof.ProcessingConfig()

cam = tof.EmbeddedKeaCamera(proc)
config = cam.getCameraConfig()

tof.selectStreams(cam, [tof.FrameType.RADIAL, tof.FrameType.INTENSITY])

writer = tof.createCsfWriterCamera(csf_out, cam)

cam.start()
for n in range(0, ndrop):
    frames = cam.getFrames()

for n in range(0, ncap):
    frames = cam.getFrames()
    for frame in frames:
        writer.writeFrame(frame)

cam.stop()

