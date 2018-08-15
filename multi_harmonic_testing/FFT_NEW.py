import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack


transmission_table = {"10" : 0.00177828,
                      "20" : 0.00175792,
                      "30" : 0.00176807,
                      "40" : 0.00177623,
                      "50" : 0.00188148,
                      "60" : 0.00190546,
                      "70" : 0.00223872,
                      "80" : 0.00251189,
                      "90" : 0.00281838,
                      "100" : 0.00316228,
                      "200" : 0.00354813,
                      "300" : 0.00398107,
                      "400" : 0.00446684,
                      "500" : 0.00501187,
                      "600" : 0.00562341,
                      "700" : 0.00630957,
                      "800" : 0.00707946,
                      "900" : 0.00794328,
                      "1000": 0.0141254,
                      "2000": 0.0177828,
                      "3000": 0.0223872,
                      "4000": 0.0281838,
                      "5000": 0.0398107,
                      "6000": 0.0630957,
                      "7000": 0.100000,
                      "8000": 0.158489,
                      "9000": 0.251189,
                      "10000": 0.3000}

# Number of samplepoints
N = 10000
# sample spacing
T = 1.0 / 10E4

amplitude = 0.1
harmonic = 9


x = np.linspace(0.0, N*T, N)
y = amplitude * np.sin(harmonic * 10**1 * 2.0*np.pi*x) +\
    amplitude * np.sin(harmonic * 10**2 * 2.0*np.pi*x) +\
    amplitude * np.sin(harmonic * 10**3 * 2.0*np.pi*x)

yf = 4*np.fft.fft(y)/N
yf = abs(yf[:int(N/2)])

print(yf)

position = int((harmonic * 10**1 * len(frq_vector)) / np.max(frq_vector))
riple = yf[position]

xf = np.linspace(0.0, 1.0/(2.0*T), N/2)
plt.figure(1)
plt.subplot(211)
plt.plot(x[0:int(len(x)/10)], y[0:int(len(y)/10)])
plt.subplot(212)
plt.semilogx(xf, np.abs(yf[:N//2]), 'r--')
plt.show()




harmonic = 1
power = 3

# for harmonic in range(1,10):
#     print(harmonic)
#     y = amplitude * np.sin(harmonic * 10 ** 1 * 2.0 * np.pi * x) + \
#         amplitude * np.sin(harmonic * 10 ** 2 * 2.0 * np.pi * x) + \
#         amplitude * np.sin(harmonic * 10 ** 3 * 2.0 * np.pi * x)
#
#     yf = scipy.fftpack.fft(y)
#     xf = np.linspace(0.0, 1.0 / (2.0 * T), N / 2)
#
#     plt.figure(1)
#     plt.subplot(211)
#     plt.plot(x[0:int(len(x) / 10)], y[0:int(len(y) / 10)])
#     plt.subplot(212)
#     plt.semilogx(xf, 2.0 / N * np.abs(yf[:N // 2]), 'r--')
#     plt.show()
#
#     print( len(  2.0 / N * np.abs(yf[:N // 2]  ) ))
