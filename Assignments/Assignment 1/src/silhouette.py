from sklearn.metrics import silhouette_score
from numpy import genfromtxt
import glob

for filename in glob.iglob('Subspace*.csv'):
    data = genfromtxt(filename, delimiter=' ')
    class_label = data[:, -1]
    dataset = data[:, :-1]
    silhouette_avg = silhouette_score(dataset, class_label)
    print(filename, silhouette_avg)
