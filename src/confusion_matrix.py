from sklearn.metrics import confusion_matrix
from numpy import genfromtxt
import numpy as np
filename = "confusion_matrix.csv"
comp_data = genfromtxt(filename, delimiter=' ')
class_label = comp_data[:, -1]
dataset = comp_data[:, :-1]
true_data = genfromtxt("Iris-Labeled.csv")
true_label = true_data[:, -1]
l = [i for i in range(class_label.size) if(class_label[i] == -1)]
class_label = np.delete(class_label, l)
true_label = np.delete(true_label, l)
print(confusion_matrix(class_label, true_label))
