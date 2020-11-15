import numpy as np
import scipy.signal
import scipy.io.wavfile
import matplotlib.pyplot as plt

def compose_signal(left, right):
    signal = np.array([left, right]).T * 32768
    signal[signal > 32767] = 32767
    signal[signal < -32768] = -32768
    signal = signal.astype(np.int16)
    return signal


filename = 'input.dat'
fs = 48000
select = 3

if select == 1:  # 440 Hz-es szinusz- es koszinuszjel
    t = np.arange(0, 1, 1 / fs)
    signal = compose_signal(np.sin(2 * np.pi * 440 * t), np.cos(2 * np.pi * 440 * t))

elif select == 2:  # 100 Hz-es haromszogjel a bal csatornan, pl. PLL tesztelesehez
    t = np.arange(0, 1, 1 / fs)
    signal = compose_signal(scipy.signal.sawtooth(2 * np.pi * 100 * t, 0.5), 0 * t)

elif select == 3:  # sweep (chirp) jel a jobb csatornan, pl. szuro meresehez
    t = np.arange(0, 1, 1 / fs)
    signal = compose_signal(0 * t, scipy.signal.chirp(t, 5000, 1, 15000, 'linear'))

elif select == 4:  # audio fajlbol, pl. zenei effekthez
    fs_file, signal = scipy.io.wavfile.read('input.wav')
    if fs_file != fs:
        print('Figyelem, eltero mintaveteli fekvencia (%d Hz)!\n' % fs_file)
    signal = np.interp(signal.astype(float), (np.iinfo(signal.dtype).min, np.iinfo(signal.dtype).max), (-1, 1))
    signal = compose_signal(signal[:, 0], signal[:, 1])


np.savetxt(filename, signal, fmt='%d')
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