import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

# =============================================================
# Function to create a tridiagonal matrix
def create_tridiagonal(N, a, b, c):
    return np.diag(a, 0) + np.diag(b, 1) + np.diag(c, -1)

# =============================================================
# Initial condition: u(x,0) = 0 except for a triangular pulse in [9/20, 11/20]
def initial_condition(x):
    u = np.zeros_like(x)
    mask1 = (x >= 9/20) & (x < 1/2)
    mask2 = (x >= 1/2) & (x < 11/20)
    u[mask1] = 20 * (x[mask1] - 9/20)
    u[mask2] = -20 * (x[mask2] - 11/20)
    return u

# Dirichlet boundary conditions: u(0,t) = 0, u(1,t) = 0
def boundary_condition(t):
    return [0.0, 0.0]

# =============================================================
def exact_solution(x, t):
    # Exact solution using Fourier series expansion
    u_exact = np.zeros_like(x)
    for n in range(1, 100):  # sum over first 100 terms
        A_n = 40/(n**2 * np.pi**2) * (- np.sin(9/20 * n * np.pi) + 2 * np.sin(1/2 * n * np.pi) - np.sin(11/20 * n * np.pi))
        u_exact += A_n * np.exp(-n**2 * np.pi**2 * t) * np.sin(n * np.pi * x)
    return u_exact

def Crank_Nicolson(u, h, k, nu):
    r = nu * k / h**2
    M = len(u)
    N = M - 2                     # internal points count
    # Create the tridiagonal matrix A for the implicit scheme
    A = create_tridiagonal(N, -2.0*np.ones(N), np.ones(N-1), np.ones(N-1))
    u_internal = u[1:N+1]         # internal points
    u_internal_next = np.linalg.solve(np.eye(N) - 0.5 * r * A, (np.eye(N) + 0.5 *r * A) @ u_internal)
    # add boundary values back
    return np.concatenate(([0], u_internal_next, [0]))  

def BTCS(u, h, k, nu):
    r = nu * k / h**2
    M = len(u)
    N = M - 2                     # internal points count
    # Create the tridiagonal matrix A for the implicit scheme
    A = create_tridiagonal(N, -2.0*np.ones(N), np.ones(N-1), np.ones(N-1))
    u_internal = u[1:N+1]         # internal points
    u_internal_next = np.linalg.solve(np.eye(N) - r * A, u_internal)
    # add boundary values back
    return np.concatenate(([0], u_internal_next, [0]))  

def FTCS(u, h, k, nu):
    r = nu * k / h**2
    M = len(u)
    N = M - 2                     # internal points count
    # Create the tridiagonal matrix A for the implicit scheme
    A = create_tridiagonal(N, -2.0*np.ones(N), np.ones(N-1), np.ones(N-1))
    u_internal = u[1:N+1]         # internal points
    u_internal_next = (np.eye(N) + r * A) @ u_internal
    # add boundary values back
    return np.concatenate(([0], u_internal_next, [0])) 

def Gauss_Legendre_RK(u, h, k, nu):
    """
    RK formula:
        y1 = f(u + k/2 * y1, t + k/2)
        u_next = u + k * y1
    where f(u) = (nu/h^2) A u, replace k * y1 with v, we have
        v = r * A * ( u + 0.5 * v )   =>   (I - 0.5*r*A) v = r*A*u
        u_next = u + v
    """
    r = nu * k / h**2
    M = len(u)
    N = M - 2                       # internal points count
    A = create_tridiagonal(N, -2.0*np.ones(N), np.ones(N-1), np.ones(N-1))
    u_internal = u[1:N+1]           # internal points
    # calculate the right-hand side
    rhs = r * (A @ u_internal)
    # solve (I - 0.5*r*A) v = rhs
    I = np.eye(N)
    v_internal = np.linalg.solve(I - 0.5 * r * A, rhs)
    # update internal points
    u_internal_next = u_internal + v_internal
    # concatenate boundaries (maintaining Dirichlet zero boundaries)
    return np.concatenate(([0.0], u_internal_next, [0.0]))  

def Collection_RK(u, h, k, nu):
    """
    RK formula:
        y1 = f(u + 5k/12 * y1 - k/12 * y2, t + k/3)
        y2 = f(u + 3k/4 * y1 + k/4 * y2, t + k)
        u_next = u + 3k/4 * y1 + k/4 * y2
    where f(u) = (nu/h^2) A u, replace k * y1 with v1 and k * y2 with v2, we have
        v1 = r * A * ( u + 5/12 * v1 - 1/12 * v2 )
        v2 = r * A * ( u + 3/4 * v1 + 1/4 * v2 )
        [I - 5/12*r*A, 1/12*r*A] [v1]  =  [r*A*u]
        [-3/4*r*A,  I - 1/4*r*A] [v2]  =  [r*A*u]
    and finally update u:
        u_next = u + 3/4 * v1 + 1/4 * v2
    """
    r = nu * k / h**2
    M = len(u)
    N = M - 2                       # internal points count
    A = create_tridiagonal(N, -2.0*np.ones(N), np.ones(N-1), np.ones(N-1))
    u_internal = u[1:N+1]           # internal points
    # calculate the right-hand side
    rhs = np.concatenate((r * (A @ u_internal), r * (A @ u_internal)))
    # solve linear system for v1 and v2
    I = np.eye(N)
    M11 = I - (5.0/12.0) * r * A
    M12 = (1.0/12.0) * r * A
    M21 = -(3.0/4.0) * r * A
    M22 = I - (1.0/4.0) * r * A
    block_mat = np.block([[M11, M12],
                          [M21, M22]])
    v_internal = np.linalg.solve(block_mat, rhs)
    v1_internal = v_internal[:N]
    v2_internal = v_internal[N:]
    # update internal points
    u_internal_next = u_internal + (3.0/4.0) * v1_internal + (1.0/4.0) * v2_internal
    # concatenate boundaries (maintaining Dirichlet zero boundaries)
    return np.concatenate(([0.0], u_internal_next, [0.0]))  

# =============================================================
def choose_method(methodLabel):
    if methodLabel == 0:
        return 'Exact Solution', exact_solution
    elif methodLabel == 1:
        return 'Crank-Nicolson', Crank_Nicolson
    elif methodLabel == 2:
        return 'BTCS', BTCS
    elif methodLabel == 3:
        return 'FTCS', FTCS
    elif methodLabel == 4:
        return 'Collection RK', Collection_RK
    elif methodLabel == 5:
        return 'Gauss-Legendre RK', Gauss_Legendre_RK
    else:
        raise ValueError("Invalid method label. Choose 0 for 'Exact Solution', 1 for 'Crank-Nicolson', 2 for 'BTCS', 3 for 'FTCS', 4 for 'Collection RK', or 5 for 'Gauss-Legendre RK'.")

def draw_figure(func, u_0, h, k, nu):
    r = nu * k / h**2 
    solutions = []
    times = [1, 2, 10]
    u = u_0.copy()
    # Iterate and store solutions at specified times
    if func == exact_solution:
        x = np.linspace(0, 1, 200)
        for i in range(1, 11):
            u = exact_solution(x, i * k)
            if i in times:
                solutions.append(u.copy())
            if i == max(times):
                break
    else:
        x = np.linspace(0, 1, len(u_0))
        for i in range(1, 11):
            u = func(u, h, k, nu)
            if i in times:
                solutions.append(u.copy())
            if i == max(times):
                break
    # Draw the figure
    fig, axes = plt.subplots(1, 3, figsize=(15, 5), sharex=True, sharey=True)
    for ax, u_sol, t in zip(axes, solutions, times):
        ax.plot(x, u_sol, 
                label=f't = {t}k', 
                linewidth=0.8,     
                marker='o',        
                markersize=3,      
                markevery=1)       
        ax.set_xlabel('x')
        ax.set_ylabel('u(x,t)')
        ax.set_xlim(0, 1)
        ax.set_ylim(-0.2, 0.5)
        ax.grid(True)
        ax.legend()
    plt.suptitle(f'Solution at different times using {method_name} method (r={r:.2f})')
    plt.tight_layout()
    plt.savefig('figure/Program14.4.1.pdf')

# =============================================================
if __name__ == "__main__":
    # Set parameters
    L = 1.0                # interval [0, 1]
    h = 0.05               # spatial step size
    r = 1.0                # the non-dimensional number
    nu = 1.0               # thermal diffusivity
    methodLabel = 1        # 0 for 'Exact Solution', 1 for 'Crank-Nicolson', 2 for 'BTCS', 3 for 'FTCS', 4 for 'Collection RK', 5 for 'Gauss-Legendre RK'
    # Process parameters
    N = int(L/h) + 1       # number of spatial grid points (including two boundary points)
    x = np.linspace(0, L, N)
    k = r * h**2 / nu      # time step size
    # Choose method based on label
    method_name, method_func = choose_method(methodLabel)
    # Run main program and draw figure
    print(f"Using method: {method_name}")
    draw_figure(method_func, initial_condition(x), h, k, nu)