from sklearn.metrics import silhouette_score
from numpy import genfromtxt
import numpy as np
import pandas as pd
import glob
import sys
import math


database_filename = sys.argv[1]
db_file = open(database_filename, 'r')
db = db_file.readlines()
db_file.close()
for filename in glob.iglob('Subspace*.csv'):
    subspace_filename = filename
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
    ss_file = open(subspace_filename, 'r')
    ss = ss_file.readlines()
    ss_file.close()
    i = 0
    cid = -1
    while i < len(ss):
        line = list(map(float, ss[i].split()))
        num = int(math.floor(line[3]))
        noise = int(math.floor(line[1]))
        if num > 0 and noise == 0:
            cid = cid + 1
            for j in range(num):
                line = list(map(float, ss[i+j+1].split()))
                idx = int(line[0])
                point = list(map(float, db[idx].split()))
                point_ss = []
                
                for name in names[:-1]:
                    point_ss.append(point[int(name)-1])
                point_ss.append(cid)
                df.loc[len(df.index)] = point_ss
                df.loc[len(df.index) - 1]['label'] = int(cid)
        i = i + num + 1
    dims = len(names)
    data = df.to_numpy()
    class_label = data[:, -1]
    class_label = class_label.astype(int)
    dataset = data[:, :-1]
    y = np.bincount(class_label)
    if y.size > 1:
        silhouette_avg = silhouette_score(dataset, class_label)
        print(filename, silhouette_avg)
    else:
        print("1")
