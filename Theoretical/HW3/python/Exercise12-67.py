import math
import numpy as np
import matplotlib.pyplot as plt

def Adams_Bashforth(s, p):
    A = np.zeros((2*(s + 1), 2*(s + 1)))
    b = np.zeros(2*(s + 1))
    ## Generate the A matrix
    # coefficients of alpha
    for i in range(0, s + 1):
        A[i][i] = 1
    ## coefficients of beta
    for i in range(s + 1, 2*(s + 1)):
        if i == 2*(s + 1) - 1:
            A[i][i] = 1
        else:
            for j in range(0, s + 1):
                A[i][j] = (j ** (i - s)) / math.factorial(i - s)
            for j in range(s + 1, 2*(s + 1)):
                A[i][j] = -(j - s - 1)**(i - s - 1) / math.factorial(i - s - 1)
    # Generate the b vector
    b[s - 1] = -1
    b[s] = 1
    ## Solve for the coefficients
    return np.linalg.solve(A, b)

def Adams_Moulton(s, p):
    A = np.zeros((2*(s + 1), 2*(s + 1)))
    b = np.zeros(2*(s + 1))
    ## Generate the A matrix
    # coefficients of alpha
    for i in range(0, s + 1):
        A[i][i] = 1
    ## coefficients of beta
    for i in range(s + 1, 2*(s + 1)):
        for j in range(0, s + 1):
            A[i][j] = (j ** (i - s)) / math.factorial(i - s)
        for j in range(s + 1, 2*(s + 1)):
            A[i][j] = -(j - s - 1)**(i - s - 1) / math.factorial(i - s - 1)
    # Generate the b vector
    b[s - 1] = -1
    b[s] = 1
    ## Solve for the coefficients
    return np.linalg.solve(A, b)

def BDF(s, p):
    A = np.zeros((2*(s + 1), 2*(s + 1)))
    b = np.zeros(2*(s + 1))
    ## Generate the A matrix
    # coefficients of alpha
    for i in range(0, s + 1):
        if i == s:
            A[i][i] = 1
        elif i == 0:
            for j in range(0, s + 1):
                A[i][j] = 1
        else:
            for j in range(0, s + 1):
                A[i][j] = (j ** i) / math.factorial(i)
            for j in range(s + 1, 2*(s + 1)):
                A[i][j] = -(j - s - 1)**(i - 1) / math.factorial(i - 1)
    ## coefficients of beta
    for i in range(s + 1, 2*(s + 1)):
        if i == 2*(s + 1) - 1:
            for j in range(0, s + 1):
                A[i][j] = (j ** s) / math.factorial(s)
            for j in range(s + 1, 2*(s + 1)):
                A[i][j] = -(j - s - 1)**(s - 1) / math.factorial(s - 1)
        else:
            A[i][i] = 1
    # Generate the b vector
    b[s] = 1
    ## Solve for the coefficients
    return np.linalg.solve(A, b)

if __name__ == "__main__":
    [s, p] = [3, 3]
    Label = 3

    match Label:
        case 1:
            Coefs = Adams_Bashforth(s, p)
            print(f"Adams-Bashforth Coefficients for s = {s} and p = {p}")
        case 2:
            Coefs = Adams_Moulton(s, p)
            print(f"Adams-Moulton Coefficients for s = {s} and p = {p}")
        case 3:
            Coefs = BDF(s, p)
            print(f"BDF Coefficients for s = {s} and p = {p}")
            
    for i in range(0, s + 1):
        print(f"alpha_{i} = {Coefs[i]}")
    for i in range(0, s + 1):
        print(f"beta_{i} = {Coefs[i + s + 1]}")
    