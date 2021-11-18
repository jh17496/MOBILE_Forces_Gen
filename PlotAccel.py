import numpy as np
import matplotlib.pyplot as plt

# Load data, ignoring header
t, x, y, z= np.loadtxt("input_forces.dat", delimiter=" ", skiprows=3, usecols=(0,7,8,9), unpack=True)

t = t[:-1]
x = x[:-1]
y = y[:-1]
z = z[:-1]

# Plot data
plt.rcParams['axes.grid'] = True
fig, axs = plt.subplots(3,1);

axs[0].plot(t,x)
axs[0].set_ylim(-10,10)
axs[0].set_ylabel('x / $ms^{-2}$')

axs[1].plot(t,y)
axs[1].set_ylim(-10,10)
axs[1].set_ylabel('y / $ms^{-2}$')

axs[2].plot(t,z)
axs[2].set_xlabel('time / s')
axs[2].set_ylabel('z / $ms^{-2}$')

fig.tight_layout()
plt.show()
