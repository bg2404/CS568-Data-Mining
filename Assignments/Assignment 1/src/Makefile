CC = g++
CFLAGS = -Wall -g

TARGET = subclu

all: $(TARGET)

$(TARGET): Subspace.o BitsUtil.o Main.o
			$(CC) $(CFLAGS) -o $(TARGET) Main.o Subspace.o BitsUtil.o

Main.o: Main.cpp
	$(CC) $(CFLAGS) -c Main.cpp

Subspace.o: Subspace.h BitsUtil.h Subspace.cpp
	$(CC) $(CFLAGS) -c Subspace.cpp

BitsUtil.o: BitsUtil.h BitsUtil.cpp
	$(CC) $(CFLAGS) -c BitsUtil.cpp

clean:
			$(RM) $(TARGET) *.o
