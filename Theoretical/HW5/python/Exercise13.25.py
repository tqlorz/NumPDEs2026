import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

# Define a function for ivp problem
def f(u, t):
    lambda_0 = -1e6
    return lambda_0 * (u - np.cos(t)) - np.sin(t)

# Trapezoidal method
def Trapezoidal(f, u_0, t_0, k, n_steps):
    u_vals = [u_0]
    t_vals = [t_0]
    u = u_0
    t = t_0
    for _ in range(n_steps):
        def OneStep(u_n):
            return u_n - u - k/2 * (f(u, t) + f(u_n, t + k))
        u_n = fsolve(OneStep, u)[0]
        u = u_n
        t = t + k
        u_vals.append(u)
        t_vals.append(t)
    return np.array(u_vals), np.array(t_vals)

# Backward Euler method
def Backward_Euler(f, u_0, t_0, k, n_steps):
    u_vals = [u_0]
    t_vals = [t_0]
    u = u_0
    t = t_0
    for _ in range(n_steps):
        def OneStep(u_n):
            return u_n - u - k * f(u_n, t + k)
        u_n = fsolve(OneStep, u)[0]
        u = u_n
        t = t + k
        u_vals.append(u)
        t_vals.append(t)
    return np.array(u_vals), np.array(t_vals)

def plot_ivp(ax, method_name, f, u_0, t_0, k=0.1, n_steps=200,
             xlim=(-3, 1), ylim=(-3, 3)):
    if method_name == 'Trapezoidal':
        u_vals, t_vals = Trapezoidal(f, u_0, t_0, k, n_steps)
    elif method_name == 'Backward Euler':
        u_vals, t_vals = Backward_Euler(f, u_0, t_0, k, n_steps)
    else:
        raise ValueError("Unknown method name")
    
    u_ref = 1.0
    t_ref = np.linspace(t_0, t_0 + n_steps * k, 1000)
    u_ref = np.cos(t_ref) + (u_ref - 1) * np.exp(-1e-6 * t_ref)
    ax.plot(t_ref, u_ref, label='Exact Solution', linestyle='--', color='gray')

    ax.scatter(t_vals, u_vals, label=method_name)
    ax.plot(t_vals, u_vals, label=method_name, linestyle='--')
    ax.set_title(f'{method_name} Stability Regions')
    ax.set_xlabel('Re(κ)')
    ax.set_ylabel('Im(κ)')
    ax.set_xlim(xlim)
    ax.set_ylim(ylim)
    ax.grid(True, linestyle='--', alpha=0.7)
    ax.axhline(0, color='black', linewidth=0.5)
    ax.axvline(0, color='black', linewidth=0.5)
    ax.legend()

    
if __name__ == "__main__":
    u_0 = 1.5
    t_0 = 0
    k = 0.1
    t_total = 3.5
    n_steps = int(t_total / k)

    fig, axes = plt.subplots(1, 2, figsize=(10, 5))
    plot_ivp(axes[0], 'Trapezoidal', f, u_0, t_0, k, n_steps,
            xlim=(0, 3.5), ylim=(-2, 2))
    plot_ivp(axes[1], 'Backward Euler', f, u_0, t_0, k, n_steps,
            xlim=(0, 3.5), ylim=(-2, 2))
    plt.tight_layout()
    plt.savefig('figure/Exercise13-25.pdf')