CC = g++
CFLAGS = -Wall -g

# The build target
TARGET = asm

all: $(TARGET)

$(TARGET): dbscan.o main.o
			$(CC) $(CFLAGS) -o $(TARGET) main.o dbscan.o


main.o: dbscan.h main.cpp
	$(CC) $(CFLAGS) -c main.cpp


dbscan.o: dbscan.h dbscan.cpp
	$(CC) $(CFLAGS) -c dbscan.cpp

clean:
			$(RM) $(TARGET) *.o
