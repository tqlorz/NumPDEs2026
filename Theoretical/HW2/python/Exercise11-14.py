import numpy as np
import matplotlib.pyplot as plt

# Set parameters
n = 6
h = 1.0 / n
j = np.arange(1, n-1+1)         # j = 1, 2, ..., n-1
k = np.arange(1, n-1+1)         # k = 1, 2, ..., n-1
axis_x = np.linspace(0, 1, n*100)
axis_n = h * j

# Plot the eigenvectors of A
plt.rcParams.update({'font.size': 24})
plt.figure(figsize=(32, 30))
for m in k:
    w_k_continuous_LF = np.sin(m * np.pi * axis_x)
    w_k_continuous_HF = np.sin((2*n-m) * np.pi * axis_x)
    w_k_discrete_LF = np.sin(m * np.pi * axis_n)
    w_k_discrete_HF = np.sin((2*n-m) * np.pi * axis_n)

    plt.subplot(n//2, 2, m)
    plt.plot(axis_x, w_k_continuous_LF, label=f'm = {m}', linestyle='--')
    plt.plot(axis_x, w_k_continuous_HF, label=f'm = {2*n-m}', linestyle='--')
    plt.scatter(axis_n, w_k_discrete_LF, s=200)
    plt.scatter(axis_n, w_k_discrete_HF, s=200)
    plt.xlabel('x')
    plt.ylabel('w_k')
    plt.title('k = ' + str(m) + ' and k\' = ' + str(2*n-m))
    plt.grid(True)
plt.savefig('figure/Exercise11-14.pdf')