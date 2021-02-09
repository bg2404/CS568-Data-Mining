import sys
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def plot1D(ax, df, label, title):
    legend = []
    i = 1
    for pl in df[label].unique():
        fdl = df[df[label] == pl]
        ax.scatter(fdl.iloc[:, 0], np.zeros_like(
            fdl.iloc[:, 1]), color="C" + str(i))
        legend.append(str(pl))
        i = i + 1
    ax.set_xlabel(df.columns[0])
    ax.legend(legend)
    ax.set_title(title)


def plot2D(ax, df, label, title):
    legend = []
    i = 1
    for pl in df[label].unique():
        fdl = df[df[label] == pl]
        ax.scatter(fdl.iloc[:, 0], fdl.iloc[:, 1], color="C" + str(i))
        legend.append(str(pl))
        i = i + 1
    ax.set_xlabel(df.columns[0])
    ax.set_ylabel(df.columns[1])
    ax.legend(legend)
    ax.set_title(title)


def plot3D(ax, df, label, title):
    legend = []
    i = 1
    for pl in df[label].unique():
        fdl = df[df[label] == pl]
        ax.scatter3D(fdl.iloc[:, 0], fdl.iloc[:, 1],
                     fdl.iloc[:, 2], color="C" + str(i))
        legend.append(str(pl))
        i = i + 1
    ax.set_xlabel(df.columns[0])
    ax.set_ylabel(df.columns[1])
    ax.set_zlabel(df.columns[2])
    ax.legend(legend)
    ax.set_title(title)


if __name__ == '__main__':

    if len(sys.argv) < 2:
        print("Usage: ./subclu {{file}}")

    else:
        datafile_name = sys.argv[1]

        if datafile_name.startswith("Subspace") and len(datafile_name) > len("subspace.csv"):
            subspace = datafile_name[len("Subspace"):-4]
            names = []
            cur_dim = 'x'

            for dim in subspace:
                if dim == '1':
                    names.append(cur_dim)
                cur_dim = chr(ord(cur_dim) + 1)

            names.append('label')
            dims = len(names)
            df = pd.read_csv(sys.argv[1], sep='\s+', names=names)
        else:
            df = pd.read_csv(sys.argv[1], sep='\s+', header=None)
            dims = len(df.columns)
            names = []
            cur_dim = 'x'
            for dim in range(dims):
                names.append(cur_dim)
                cur_dim = chr(ord(cur_dim) + 1)

            df.columns = names
            df["label"] = 0
            dims = dims + 1

        if dims == 4:
            fig = plt.figure(figsize=(12, 8))
            ax = fig.add_subplot(111, projection='3d')
            plot3D(ax, df, "label", datafile_name[:-4])

        elif dims == 3:
            fig = plt.figure(figsize=(12, 8))
            ax = fig.add_subplot(111)
            plot2D(ax, df, "label", datafile_name[:-4])

        elif dims == 2:
            fig = plt.figure(figsize=(12, 8))
            ax = fig.add_subplot(111)
            plot1D(ax, df, "label", datafile_name[:-4])

        else:
            print("Compatibility Issue: Works only for 1D, 2D, 3D data.")

        plt.show()
