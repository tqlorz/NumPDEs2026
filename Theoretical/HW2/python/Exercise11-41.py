import numpy as np
import matplotlib.pyplot as plt

# Set parameters
n = 64
h = 1 / n
omega = 2/3
v_pairs = [[0, 0], [0, 2], [1, 1], [2, 0], [2, 2], [4, 0]]

# Plot the damping coefficients for different v_1, v_2
plt.rcParams.update({'font.size': 24})
plt.figure(figsize=(32, 30))
for v_1, v_2 in v_pairs:
    k = np.arange(1, n/2+1)                     # k = 1, 2, ..., n/2
    k_prime = n - k                             # k' = n-1, n-2, ..., n/2
    lambda_k = 1 - 2 * omega * np.sin(np.pi * k / (2 * n))**2
    lambda_k_prime = 1 - 2 * omega * np.sin(np.pi * k_prime / (2 * n))**2
    c_k = np.cos(np.pi * k / (2 * n))**2
    s_k = np.sin(np.pi * k / (2 * n))**2

    c_1 = lambda_k**(v_1 + v_2) * s_k
    c_2 = lambda_k**(v_1) * lambda_k_prime**(v_2) * s_k
    c_3 = lambda_k_prime**(v_1) * lambda_k**(v_2) * c_k
    c_4 = lambda_k_prime**(v_1 + v_2) * c_k

    plt.subplot(len(v_pairs)//2, 2, v_pairs.index([v_1, v_2]) + 1)
    plt.plot(k, c_1, label='c_1')
    plt.plot(k_prime, c_2, label='c_2')
    plt.plot(k, c_3, label='c_3')
    plt.plot(k_prime, c_4, label='c_4')
    plt.xlabel('k')
    plt.ylabel('Damping Coefficient')  
    plt.title(f'v_1={v_1}, v_2={v_2}')
    plt.legend()
    plt.grid(True)

plt.savefig('figure/Exercise11-41.pdf')