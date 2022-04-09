CC = g++
CFLAGS = -std=c++11 -O3 -lm
EXECUTABLE = bin/fp

all: $(EXECUTABLE)

$(EXECUTABLE): src/main.cpp sp.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) src/main.cpp sp.o veb.o

sp.o: src/sp.cpp src/sp.h src/module.h veb.o
	$(CC) $(CFLAGS) -o sp.o -c src/sp.cpp

veb.o: src/veb.cpp src/veb.h
	$(CC) $(CFLAGS) -o veb.o -c src/veb.cpp

clean:
	rm -rf *.o *.HPWL $(EXECUTABLE) 
