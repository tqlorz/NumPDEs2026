import numpy as np
import matplotlib.pyplot as plt

def amplification_factor(mu, theta):
    z = np.exp(-1j * theta)          # e^{-iθ}
    q = 1 - z                        # 1 - e^{-iθ}
    g = 1 - mu * q + 0.5 * mu * (mu - 1) * q**2
    return g

if __name__ == "__main__":
    theta = np.linspace(0, 2 * np.pi, 40)
    mu_list = [0.8, 1.6, 2.0, 2.4]

    # Draw the root locus for each mu
    fig, axes = plt.subplots(2, 2, figsize=(10, 10))
    axes = axes.flatten()
    for idx, mu in enumerate(mu_list):
        ax = axes[idx]        
        # Draw the amplification factor points
        g_vals = amplification_factor(mu, theta)
        ax.scatter(g_vals.real, g_vals.imag, s=15, c='blue', alpha=0.7, label=f'$\\mu = {mu}$')
        # Draw the unit circle for reference
        circle_theta = np.linspace(0, 2 * np.pi, 200)
        ax.plot(np.cos(circle_theta), np.sin(circle_theta), 'r--', linewidth=1.5, label='Unit circle')
        # Set aspect ratio and limits
        ax.set_aspect('equal')
        ax.set_xlim(-1.5, 1.5)
        ax.set_ylim(-1.5, 1.5)
        ax.grid(True, linestyle=':', alpha=0.6)
        ax.set_xlabel('Re($g$)')
        ax.set_ylabel('Im($g$)')
        ax.legend(loc='upper right')

    plt.suptitle('Absolute stability of the Beam-Warming method with Different $\\mu$ Values', fontsize=16)
    plt.tight_layout()
    plt.savefig('figure/Exercise14.100.pdf')