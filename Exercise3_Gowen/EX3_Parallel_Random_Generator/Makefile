# TEMPLATE Makefile Gowen

CC = g++
CFLAGS = -Wall -O3 -std=c++23

TARGET = main
OBJS = main.o random.o statistics.o GeometricBrownianMotion.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

random.o: random.cpp random.h
	$(CC) $(CFLAGS) -c random.cpp -o random.o

statistics.o: statistics.cpp statistics.h
	$(CC) $(CFLAGS) -c statistics.cpp -o statistics.o

GeometricBrownianMotion.o: GeometricBrownianMotion.cpp GeometricBrownianMotion.h
	$(CC) $(CFLAGS) -c GeometricBrownianMotion.cpp -o GeometricBrownianMotion.o

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET) seed.out
