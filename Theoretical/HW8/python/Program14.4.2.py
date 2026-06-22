import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

# =============================================================
# Initial condition: u(x,0) = 0 except for a triangular pulse in [9/20, 11/20]
def initial_condition(x):
    u = np.exp(-20*(x-2)**2) + np.exp(-(x-5)**2)
    return u

# =============================================================
def exact_solution(x, t):
    u_exact = np.exp(-20*(x-t-2)**2) + np.exp(-(x-t-5)**2)
    return u_exact

def leapfrog(u_prev, u, h, k, a):
    mu = a * k / h
    u_next = u_prev - mu * (np.roll(u, -1) - np.roll(u, 1))
    return u_next

def Lax_Friedrichs(u, h, k, a):
    mu = a * k / h
    u_next = 0.5 * (np.roll(u, -1) + np.roll(u, 1)) - 0.5 * mu * (np.roll(u, -1) - np.roll(u, 1))  
    return u_next

def Lax_Wendroff(u, h, k, a):
    mu = a * k / h
    u_next = u - 0.5 * mu * (np.roll(u, -1) - np.roll(u, 1)) + 0.5 * mu**2 * (np.roll(u, -1) - 2 * u + np.roll(u, 1))
    return u_next

def upwind(u, h, k, a):
    mu = a * k / h
    if a >= 0:
        u_next = u - mu * (u - np.roll(u, 1))
    else:
        u_next = u - mu * (np.roll(u, -1) - u)
    return u_next

def Beam_Warming(u, h, k, a):
    mu = a * k / h
    if a >= 0:
        u_next = u - 0.5 * mu * (3*u - 4*np.roll(u, 1) + np.roll(u, 2)) + 0.5 * mu**2 * (u - 2*np.roll(u, 1) + np.roll(u, 2))
    else:
        u_next = u - 0.5 * mu * (-3*u + 4*np.roll(u, -1) - np.roll(u, -2)) + 0.5 * mu**2 * (u - 2*np.roll(u, -1) + np.roll(u, -2)) 
    return u_next

# =============================================================
def choose_method(methodLabel):
    if methodLabel == 1:
        return 'leapfrog method', leapfrog
    elif methodLabel == 2:
        return 'Lax-Friedrichs method', Lax_Friedrichs
    elif methodLabel == 3:
        return 'Lax-Wendroff method', Lax_Wendroff
    elif methodLabel == 4:
        return 'upwind method', upwind
    elif methodLabel == 5:
        return 'Beam-Warming method', Beam_Warming
    else:
        raise ValueError("Invalid method label. Choose 1 for 'leapfrog method', 2 for 'Lax-Friedrichs method', 3 for 'Lax-Wendroff method', 4 for 'upwind method', or 5 for 'Beam-Warming method'.")

def draw_figure(func, u_0, h, T, a):
    solutions = []
    timeSteps = [0.8 * h, h]
    x = np.linspace(0, 25, len(u_0))
    # Run the simulation for each time step size
    if func == leapfrog:
        for k in timeSteps:
            t = 0
            # initial condition for u_prev at t=-k
            u_t_prev = exact_solution(np.linspace(0, 25, len(u_0)), -k) 
            u_t = u_0.copy()
            while t < T:
                temp = func(u_t_prev, u_t, h, k, a)
                u_t_prev = u_t
                u_t = temp
                t += k
            solutions.append(u_t)
    else:
        for k in timeSteps:
            t = 0
            u_t = u_0.copy()
            while t < T:
                u_t = func(u_t, h, k, a)
                t += k
            solutions.append(u_t)
    # exact solution for comparison
    u_exact = exact_solution(np.linspace(0, 25, len(u_0)), T)
    # Draw the figure
    fig, axes = plt.subplots(1, 2, figsize=(10, 5))
    x = np.linspace(0, 25, len(u_0))
    for ax, u_sol, k in zip(axes, solutions, timeSteps):
        ax.plot(x, u_sol, 
                label=f'k = {k:.3f}', 
                linewidth=1.2,     
                marker='o',        
                markersize=2,      
                markevery=1)  
        ax.plot(x, u_exact, 
                'k--', 
                linewidth=0.8,
                label='Exact')     
        ax.set_xlim([15, 25])
        ax.set_xlabel('x')
        ax.set_ylabel('u(x,t)')
        ax.grid(True)
        ax.legend()    
    plt.suptitle(f'Solution with different k using {method_name} method')
    plt.tight_layout()
    plt.savefig('figure/Program14.4.2.pdf')

# =============================================================
if __name__ == "__main__":
    # Set parameters
    L = 25.0               # interval length
    h = 0.05               # spatial step size
    T = 17                 # total time
    a = 1.0                # thermal diffusivity
    methodLabel = 5        # 1 for 'leapfrog method', 2 for 'Lax-Friedrichs method', 3 for 'Lax-Wendroff method', 4 for 'upwind method', 5 for 'Beam-Warming method'
    # Process parameters
    N = int(L/h) + 1       # number of spatial grid points (including two boundary points)
    x = np.linspace(0, L, N)
    # Choose method based on label
    method_name, method_func = choose_method(methodLabel)
    # Run main program and draw figure
    print(f"Using method: {method_name}")
    draw_figure(method_func, initial_condition(x), h, T, a)