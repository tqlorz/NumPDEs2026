import numpy as np
import matplotlib.pyplot as plt

# Set parameters
n = 6
h = 1.0 / n
j = np.arange(1, n-1+1)         # j = 1, 2, ..., n-1
k = np.arange(1, n+1)           # k = 1, 2, ..., n
axis_x = np.linspace(0, 1, n*100)
axis_n = h * j

# Plot the eigenvectors of A
plt.rcParams.update({'font.size': 24})
plt.figure(figsize=(32, 30))
for m in k:
    w_k_continuous = np.sin(m * np.pi * axis_x)
    w_k_discrete = np.sin(m * np.pi * axis_n)

    plt.subplot(n//2, 2, m)
    plt.plot(axis_x, w_k_continuous, label=f'm = {m}', linestyle='--')
    plt.scatter(axis_n, w_k_discrete, s=200)
    plt.xlabel('x')
    plt.ylabel('w_k')
    plt.title(f'k = {m}' + ' with ' + f'n = {n}')
    plt.grid(True)
plt.savefig('figure/Exercise11-11.pdf')