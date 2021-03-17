import sys
import math
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
                     fdl.iloc[:, 2], color="C" + str(int(i)))
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

    elif len(sys.argv) == 2:
        datafile_name = sys.argv[1]
        df = pd.read_csv(sys.argv[1], sep='\s+', header=None)

        if datafile_name.startswith("Subspace") and len(datafile_name) > len("subspace.csv"):
            subspace = datafile_name[len("Subspace"):-4]
            names = []
            cur_dim = '1'

            for dim in subspace:
                if dim == '1':
                    names.append(cur_dim)
                cur_dim = chr(ord(cur_dim) + 1)

            names.append('label')
            dims = len(names)
            df.columns = names
        else:
            dims = len(df.columns)
            names = []
            cur_dim = '1'
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
    else:
        database_filename = sys.argv[1]
        subspace_filename = sys.argv[2]
        dims = 0
        names = []
        cur_dim = '1'
        subspace = subspace_filename[8:-4]
        for c in subspace:
            if c == '1':
                names.append(cur_dim)
            cur_dim = chr(ord(cur_dim) + 1)
        names.append('label')
        df = pd.DataFrame(columns=names)
        db_file = open(database_filename, 'r')
        db = db_file.readlines()
        db_file.close()
        ss_file = open(subspace_filename, 'r')
        ss = ss_file.readlines()
        ss_file.close()
        i = 0
        cid = 0
        while i < len(ss):
            line = list(map(float, ss[i].split()))
            cid = cid + 1
            num = int(math.floor(line[3]))
            for j in range(num):
                line = list(map(float, ss[i+j].split()))
                idx = int(line[0])
                point = list(map(float, db[idx].split()))
                point_ss = []
                for name in names[:-1]:
                    point_ss.append(point[int(name)-1])
                point_ss.append(cid)
                df.loc[len(df.index)] = point_ss
                df.loc[len(df.index) -1]['label'] = int(cid)
            i = i + num + 1
        dims = len(names)

        if dims == 4:
            fig = plt.figure(figsize=(12, 8))
            ax = fig.add_subplot(111, projection='3d')
            plot3D(ax, df, "label", subspace_filename[:-4])

        elif dims == 3:
            fig = plt.figure(figsize=(12, 8))
            ax = fig.add_subplot(111)
            plot2D(ax, df, "label", subspace_filename[:-4])

        elif dims == 2:
            fig = plt.figure(figsize=(12, 8))
            ax = fig.add_subplot(111)
            plot1D(ax, df, "label", subspace_filename[:-4])

        else:
            print("Compatibility Issue: Works only for 1D, 2D, 3D data.")
        plt.show()
