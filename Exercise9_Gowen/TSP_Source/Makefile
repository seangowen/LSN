# Makefile TSP Ex.9 Gowen

CC = g++
CFLAGS = -Wall -O3 -std=c++23

TARGET = TSP
OBJS = main.o geneticAlgorithm.o americanCitiesDatabase.o citiesTour.o random.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp 

geneticAlgorithm.o: geneticAlgorithm.cpp geneticAlgorithm.h 
	$(CC) $(CFLAGS) -c geneticAlgorithm.cpp

americanCitiesDatabase.o: americanCitiesDatabase.cpp americanCitiesDatabase.h
	$(CC) $(CFLAGS) -c americanCitiesDatabase.cpp

citiesTour.o: citiesTour.cpp citiesTour.h
	$(CC) $(CFLAGS) -c citiesTour.cpp

random.o: random.cpp random.h 
	$(CC) $(CFLAGS) -c random.cpp

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
