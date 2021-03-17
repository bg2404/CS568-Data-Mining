CC = g++
CFLAGS = -Wall -g

TARGET = subclu

all: $(TARGET)

$(TARGET): Relation.o BitsUtil.o Cluster.o Subspace.o ReadInput.o DBSCAN.o SUBCLU.o INCRDBSCAN.o INCRSUBCLU.o Main.o 
			$(CC) $(CFLAGS) -o $(TARGET) Relation.o BitsUtil.o Cluster.o Subspace.o ReadInput.o DBSCAN.o SUBCLU.o INCRDBSCAN.o INCRSUBCLU.o Main.o 

Main.o: Cluster.h Subspace.cpp SUBCLU.h INCRSUBCLU.h Main.cpp
	$(CC) $(CFLAGS) -c Main.cpp

INCRSUBCLU.o: INCRSUBCLU.h Cluster.h Relation.h Subspace.h INCRDBSCAN.h ReadInput.h INCRSUBCLU.cpp
	$(CC) $(CFLAGS) -c INCRSUBCLU.cpp

INCRDBSCAN.o: INCRDBSCAN.h Cluster.h DBSCAN.h Relation.h Subspace.h INCRDBSCAN.cpp
	$(CC) $(CFLAGS) -c INCRDBSCAN.cpp

SUBCLU.o: SUBCLU.h Cluster.h Relation.h Subspace.h DBSCAN.h ReadInput.h SUBCLU.cpp
	$(CC) $(CFLAGS) -c SUBCLU.cpp

DBSCAN.o: DBSCAN.h Cluster.h Relation.h Subspace.h DBSCAN.cpp
	$(CC) $(CFLAGS) -c DBSCAN.cpp

ReadInput.o: ReadInput.h Relation.h Subspace.h Cluster.h ReadInput.cpp
	$(CC) $(CFLAGS) -c ReadInput.cpp

Subspace.o: Subspace.h BitsUtil.h Cluster.h Subspace.cpp
	$(CC) $(CFLAGS) -c Subspace.cpp

Cluster.o: Cluster.h Cluster.cpp
	$(CC) $(CFLAGS) -c Cluster.cpp

BitsUtil.o: BitsUtil.h BitsUtil.cpp
	$(CC) $(CFLAGS) -c BitsUtil.cpp

Relation.o: Relation.h Relation.cpp
	$(CC) $(CFLAGS) -c Relation.cpp

clean:
			$(RM) $(TARGET) *.o Subspace*.csv
