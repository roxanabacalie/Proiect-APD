import numpy as np
from scipy.signal import hilbert
signalFile=open("waveData.txt", "r")
signal = signalFile.readlines()
analytic_signal=hilbert(signal) #signal este vectorul aferent semnalului analizat
amplitude_envelope=np.abs(analytic_signal) #amplitude_envelope este anvelopa semnalului
np.savetxt("anvelopa.txt", amplitude_envelope, fmt="%2.0f")

#file = open("anvelopa.txt", "w+")
#content = str(amplitude_envelope)
#file.write(content)
#file.close()