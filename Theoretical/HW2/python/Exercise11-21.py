import numpy as np
import matplotlib.pyplot as plt

def CreateTridiagonal(main_diag, upper_diag, lower_diag):
    main_matrix = np.diag(main_diag, 0)
    upper_matrix = np.diag(upper_diag, 1)
    lower_matrix = np.diag(lower_diag, -1)
    return main_matrix+upper_matrix+lower_matrix

# Set parameters
n = 64
h = 1 / n
j = np.arange(1, n-1+1)         # j = 1, 2, ..., n-1
k = np.arange(1, n-1+1)         # k = 1, 2, ..., n-1
axis_n = h * j
omega = [1/3, 1/2, 2/3, 1]
A = 1/h**2 * CreateTridiagonal(2*np.ones(n-1), (-1)*np.ones(n-2), (-1)*np.ones(n-2))

# Plot the iterations of weighted Jacobi method for different omega
plt.rcParams.update({'font.size': 24})
plt.figure(figsize=(32, 20))
for w in omega:
    T_omega = np.eye(n-1) - (w*h**2)/2 * A
    iterations = np.zeros(n-1)
    for m in k:
        w_k = np.sin(m * np.pi * axis_n)
        norm_0 = np.linalg.norm(w_k, ord=np.inf)
        norm = norm_0
        while norm > 1e-2 * norm_0:
            w_k = T_omega @ w_k
            norm = np.linalg.norm(w_k, ord=np.inf)
            iterations[m-1] += 1
    plt.subplot(2, 2, omega.index(w)+1)
    plt.plot(k, iterations, label=f'omega = {w}')
    plt.xlabel('k')
    plt.ylabel('Iterations')
    plt.title(f'Convergence of weighted Jacobi method for omega = {w:.2f}')
    plt.grid(True)
plt.savefig('figure/Exercise11-21.pdf')