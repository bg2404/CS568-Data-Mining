from sklearn.metrics import silhouette_score
from numpy import genfromtxt
import numpy as np
import glob

for filename in glob.iglob('Subspace*.csv'):
    data = genfromtxt(filename, delimiter=' ')
    class_label = data[:, -1]
    class_label = class_label.astype(int)
    dataset = data[:, :-1]
    y = np.bincount(class_label)
    if y.size > 1:
        silhouette_avg = silhouette_score(dataset, class_label)
        print(filename, silhouette_avg)
    else:
        print("1")
