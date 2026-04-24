import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import sys

def ReadFromFile(filename):
    # Define the column names for the data
    df = pd.read_csv(filename, sep='\s+', header=None,
                     names=['x', 'y', 'numerical', 'exact'])
    
    # Reshape data into 2D mesh using pivot_table
    numerical_grid = df.pivot_table(index='x', columns='y', values='numerical')
    exact_grid = df.pivot_table(index='x', columns='y', values='exact')
    
    # Get x and y values
    x_unique = numerical_grid.index.values
    y_unique = numerical_grid.columns.values
    X, Y = np.meshgrid(x_unique, y_unique, indexing='ij')
    
    # Calculate error grid
    error_grid = numerical_grid.values - exact_grid.values
    
    return X, Y, numerical_grid, exact_grid, error_grid

def PlotFigure(X, Y, numerical_grid, exact_grid, error_grid, figurename):
    plt.rcParams.update({'font.size': 24})
    fig = plt.figure(figsize=(48, 10))
    
    # figure1：numerical solution
    ax1 = fig.add_subplot(1, 3, 1, projection='3d')
    surf1 = ax1.plot_surface(X, Y, numerical_grid, cmap='viridis', edgecolor='none')
    ax1.set_title('Numerical Solution')
    ax1.set_xlabel('x')
    ax1.set_ylabel('y')
    ax1.set_zlabel('u')
    ax1.set_xlim(0, 1)
    ax1.set_ylim(0, 1)
    fig.colorbar(surf1, ax=ax1, shrink=0.5, aspect=10)
    
    # figure2：exact solution
    ax2 = fig.add_subplot(1, 3, 2, projection='3d')
    surf2 = ax2.plot_surface(X, Y, exact_grid, cmap='plasma', edgecolor='none')
    ax2.set_title('Exact Solution')
    ax2.set_xlabel('x')
    ax2.set_ylabel('y')
    ax2.set_zlabel('u')
    ax2.set_xlim(0, 1)
    ax2.set_ylim(0, 1)
    fig.colorbar(surf2, ax=ax2, shrink=0.5, aspect=10)
    
    # figure3：error
    ax3 = fig.add_subplot(1, 3, 3)
    heatmap = ax3.pcolormesh(X, Y, error_grid, cmap='coolwarm', shading='auto')
    ax3.set_title('Error Heatmap (Numerical - Exact)')
    ax3.set_xlabel('x')
    ax3.set_ylabel('y')
    ax3.set_aspect('equal', adjustable='box')
    ax3.set_xlim(0, 1)
    ax3.set_ylim(0, 1)
    fig.colorbar(heatmap, ax=ax3, label='error')
    
    plt.tight_layout()
    plt.savefig(f"output/figure/{figurename}.pdf")

if __name__ == '__main__':
    filename = sys.argv[1] if len(sys.argv) > 1 else 'test.txt'
    X, Y, numerical_grid, exact_grid, error_grid = ReadFromFile(filename)
    figurename = filename.rsplit('.', 1)[0].rsplit('/', 1)[-1]
    PlotFigure(X, Y, numerical_grid, exact_grid, error_grid, figurename)