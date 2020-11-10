import numpy as np
import matplotlib.pyplot as plt
import simpleaudio as sa

filename = 'output.dat'
play_sound = True
fs = 48000

with open(filename, 'rt') as file:
    signal = np.array([[int(value) for value in line.strip().split(' ')] for line in file], dtype=np.int16, order='C')
if play_sound:
    play_obj = sa.play_buffer(signal, 2, 2, int(fs))
t = np.arange(signal.shape[0]) / fs
plt.figure()
plt.subplot(211)
plt.plot(t, signal[:, 0] / 32768.0)
plt.xlabel('t [s]')
plt.title('Bal csatorna')
plt.subplot(212)
plt.plot(t, signal[:, 1] / 32768.0)
plt.xlabel('t [s]')
plt.title('Jobb csatorna')
plt.tight_layout()
plt.show()
if play_sound:
    play_obj.wait_done() 
