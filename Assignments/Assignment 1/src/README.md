## SUBCLU Static Implementation
This is the static C++ implementation of the SUBCLU algorithm.  
To **run**   
```
      make
      ./subclu help
      ./subclu {data set} {min points} {epsilon}
```

Dependencies for **testing**
1. sklearn `pip3 install scikit-learn`
2. numpy  `pip3 install numpy`
3. matplotlib `pip3 install matplotlib`

To **test** the results  
```
      python3 silhoutte.py
      python3 confusion_matrix.py
```

To **visualize** the results  
```
      python3 visualize.py {subspace result}
```

![alt text](https://github.com/bg2404/CS568-Data-Mining/blob/main/Assignments/Assignment%201/mouse.png)
