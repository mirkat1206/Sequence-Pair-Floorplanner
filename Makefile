CC = g++
CFLAGS = -std=c++11 -O3 -lm
EXECUTABLE = bin/fp

all: $(EXECUTABLE)

$(EXECUTABLE): src/main.cpp sp.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) src/main.cpp sp.o

sp.o: src/sp.cpp src/sp.h src/module.h
	$(CC) $(CFLAGS) -o sp.o -c src/sp.cpp

clean:
	rm -rf *.o $(EXECUTABLE)
