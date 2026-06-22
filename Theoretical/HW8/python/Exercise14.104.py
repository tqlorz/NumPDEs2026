import numpy as np
import matplotlib.pyplot as plt

def get_dependence_sets(mu, j, n):
    R = {j}
    sets = {n: R}
    
    for t in range(n-1, -1, -1):
        if mu == 1:
            R_prev = {i - 1 for i in R}          # U_k^{t+1} depends on U_{k-1}^t
        elif mu == -1:
            R_prev = {i + 1 for i in R}   # U_k^{t+1} depends on U_{k+1}^t
        else:
            raise ValueError("mu can only be 1, -1")
        R = R_prev
        sets[t] = R
    return sets

def plot_dependence(mu, j, n, ax):
    sets = get_dependence_sets(mu, j, n)    
    # Set the limits and ticks for the plot
    x_min = j - n - 1
    x_max = j + n + 1
    ax.set_xlim(x_min, x_max)
    ax.set_ylim(-0.5, n + 0.5)
    ax.set_xticks(range(x_min, x_max + 1))
    ax.set_yticks(range(0, n + 1))
    ax.grid(True, linestyle='--', alpha=0.3)
    ax.set_xlabel('$i$')
    ax.set_ylabel('$n$')
    ax.set_title(f'$\\mu = {mu}$')
    # Draw the grid points (light gray dots)
    for t in range(n + 1):
        for i in range(x_min, x_max + 1):
            ax.plot(i, t, 'o', color='lightgray', markersize=3, zorder=1)    
    # Draw the dependence points (black dots)
    for t, S in sets.items():
        if S:
            xs = sorted(S)
            ax.plot(xs, [t] * len(xs), 'ko', markersize=8, zorder=3)    
    # Draw the target point (x_j, t_n) in blue
    ax.plot(j, n, 'bs', markersize=10, label='Target point $(x_0, t_3)$', zorder=4)
    ax.legend(loc='upper right')

if __name__ == "__main__":
    j = 0          # space index
    n = 3          # time index
    mus = [1, -1]
    
    fig, axes = plt.subplots(1, 2, figsize=(9, 4))
    for ax, mu in zip(axes, mus):
        plot_dependence(mu, j, n, ax)
        
    plt.suptitle('The numerical domains of dependence of the grid point $(x_j, t_3)$ for the Lax-Wendroff method', fontsize=14)
    plt.tight_layout()
    plt.savefig('figure/Exercise14.104.pdf')