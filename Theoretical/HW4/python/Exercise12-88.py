import numpy as np
import matplotlib.pyplot as plt
from numpy.polynomial import Polynomial

# Adams-Bashforth coefficients (alpha_s = 1, alpha_{s-1} = -1, beta_s = 0)
AB_coeffs = {
    1: {'s': 1, 'alpha': [-1, 1], 'beta': [1, 0]},                
    2: {'s': 2, 'alpha': [0, -1, 1], 'beta': [-1/2, 3/2, 0]},
    3: {'s': 3, 'alpha': [0, 0, -1, 1], 'beta': [5/12, -16/12, 23/12, 0]},
    4: {'s': 4, 'alpha': [0, 0, 0, -1, 1], 'beta': [-9/24, 37/24, -59/24, 55/24, 0]},
}

# Adams-Moulton coefficients (alpha_s = 1, alpha_{s-1} = -1, beta_s ≠ 0)
AM_coeffs = {
    1: {'s': 1, 'alpha': [-1, 1], 'beta': [0, 1]},        
    2: {'s': 1, 'alpha': [-1, 1], 'beta': [1/2, 1/2]},        
    3: {'s': 2, 'alpha': [0, -1, 1], 'beta': [-1/12, 8/12, 5/12]},
    4: {'s': 3, 'alpha': [0, 0, -1, 1], 'beta': [1/24, -5/24, 19/24, 9/24]},
    5: {'s': 4, 'alpha': [0, 0, 0, -1, 1], 'beta': [-19/720, 106/720, -264/720, 646/720, 251/720]},
}

# BDF coefficients (alpha_k = 1, beta_j = 0 for j<k)
BDF_coeffs = {
    1: {'s': 1, 'alpha': [-1, 1], 'beta': [0, 1]},            
    2: {'s': 2, 'alpha': [1/3, -4/3, 1], 'beta': [0, 0, 2/3]},
    3: {'s': 3, 'alpha': [-2/11, 9/11, -18/11, 1], 'beta': [0, 0, 0, 6/11]},
    4: {'s': 4, 'alpha': [3/25, -16/25, 36/25, -48/25, 1], 'beta': [0, 0, 0, 0, 12/25]},
}

# Compute the root locus curve
def gamma_curve(coeffs, theta):
    alpha = coeffs['alpha']
    beta  = coeffs['beta']
    z = np.exp(1j * theta)
    rho = np.polyval(alpha[::-1], z)
    sigma = np.polyval(beta[::-1], z)
    return rho / sigma

# Check stability region
def check_stability(Kappa, coeffs, tol=1e-12):
    alpha = np.array(coeffs['alpha'])
    beta  = np.array(coeffs['beta'])
    poly_asc = alpha - Kappa * beta
    poly_asc = np.trim_zeros(poly_asc, 'f')
    if len(poly_asc) <= 1:
        return True
    poly_desc = poly_asc[::-1]
    roots = np.roots(poly_desc)
    return np.all(np.abs(roots) < 1.0 - tol)

def plot_ras(ax, coeffs_dict, method_name, max_order=4, grid_size=200,
             xlim=(-3, 1), ylim=(-3, 3)):
    # Generate grid
    x = np.linspace(xlim[0], xlim[1], grid_size)
    y = np.linspace(ylim[0], ylim[1], grid_size)
    X, Y = np.meshgrid(x, y)
    Kappa = X + 1j * Y

    theta = np.linspace(0, 2*np.pi, 1000)

    for p in range(1, max_order+1):
        coeffs = coeffs_dict[p]
        # Compute the root locus curve
        gamma = gamma_curve(coeffs, theta)
        ax.plot(gamma.real, gamma.imag, label=f'p={p}')

        # Check stability region for the highest order method
        if p == max_order:
            stable_mask = np.zeros_like(Kappa, dtype=bool)
            for i in range(grid_size):
                for j in range(grid_size):
                    stable_mask[i, j] = check_stability(Kappa[i, j], coeffs)
            ax.contourf(X, Y, stable_mask, levels=[0.5, 1.5],
                        colors=['lightblue'], alpha=0.5)

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
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))

    plot_ras(axes[0], AB_coeffs, 'Adams-Bashforth', max_order=4,
            xlim=(-2.5, 0.5), ylim=(-1.5, 1.5))
    plot_ras(axes[1], AM_coeffs, 'Adams-Moulton', max_order=5,
            xlim=(-7, 1), ylim=(-4, 4))
    plot_ras(axes[2], BDF_coeffs, 'BDF', max_order=4,
            xlim=(-2, 15), ylim=(-8, 8))

    plt.tight_layout()
    plt.savefig('figure/Exercise12-88.pdf')