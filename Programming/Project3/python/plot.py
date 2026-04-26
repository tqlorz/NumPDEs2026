import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys

def ReadFromFile(filename):
    # Define the column names for the data
    df = pd.read_csv(filename, sep='\s+', header=None,
                     names=['t', 'x', 'y', 'z', 'vx', 'vy', 'vz'])
    
    T = df['t'].values
    X = df['x'].values
    Y = df['y'].values

    return T, X, Y

def PlotFigure(T, X, Y, figurename):
    fig = plt.figure(figsize=(5, 5))
    mu = 0.012277471
    
    # Plot the positions of the two primary bodies
    plt.scatter(-mu, 0, color='red', label='The heavier body', s=20)
    plt.scatter(1-mu, 0, color='green', label='The lighter body', s=20)

    # Plot numerical solution
    plt.plot(X, Y, label='Numerical', color='blue')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('Planar motion of the asteroid')
    plt.grid()
    plt.legend(loc = 'upper right')
    plt.tight_layout()
    plt.savefig(f"output/figure/{figurename}.pdf")

if __name__ == '__main__':
    filename = sys.argv[1] if len(sys.argv) > 1 else 'Project3/output/data/result.txt'
    T, X, Y = ReadFromFile(filename)
    figurename = filename.rsplit('.', 1)[0].rsplit('/', 1)[-1]
    PlotFigure(T, X, Y, figurename)