import numpy as np
import matplotlib.pyplot as plt

# Set parameters
n = 64
k = np.arange(1, n-1+1)         # k = 1, 2, ..., n-1
omega = [1/3, 1/2, 2/3, 1]

# Plot the eigenvalues of T_omega for different omega
plt.rcParams.update({'font.size': 24})
plt.figure(figsize=(16, 10))
for w in omega:
    lambda_T = 1 - 2*w*np.sin(k*np.pi/(2*n))**2
    plt.plot(k, lambda_T, label=f'omega = {w:.2f}', marker='o')
plt.xlabel('k')
plt.ylabel('Eigenvalue of T_omega')
plt.title('Eigenvalues of T_omega for different omega')
plt.legend()
plt.grid(True)
plt.savefig('figure/Exercise11-18.pdf')