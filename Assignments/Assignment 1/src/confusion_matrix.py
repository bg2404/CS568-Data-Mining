from sklearn.metrics import confusion_matrix
from numpy import genfromtxt
import glob

for filename in glob.iglob('Subspace*.csv'):
    data = genfromtxt(filename, delimiter=' ')
    class_label = data[:, -1]
    dataset = data[:, :-1]
    true_label = data[:, -1]
    print(confusion_matrix(class_label, true_label))
