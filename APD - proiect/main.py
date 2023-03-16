import numpy as np
from scipy.io import wavfile

samplerate, data=wavfile.read('21.wav')

wavFileInfo=open("waveInfo.txt", "a")
wavFileInfo.write(str(samplerate)+'\n')
wavFileInfo.write(str(data.size)+'\n')

print(samplerate)
print(data.size)
print(data)

np.savetxt("waveData.txt", data, fmt="%2.0f")