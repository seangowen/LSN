# TEMPLATE Makefile Gowen

CC = g++
CFLAGS = -Wall -g -std=c++17

main : main.o random.o statistics.o GeometricBrownianMotion.o
	$(CC) GeometricBrownianMotion.o statistics.o random.o main.o -o main
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
statistics.o : statistics.cpp statistics.h
	$(CC) -c statistics.cpp -o statistics.o $(CFLAGS)
GeometricBrownianMotion.o : GeometricBrownianMotion.cpp GeometricBrownianMotion.h
	$(CC) -c GeometricBrownianMotion.cpp -o GeometricBrownianMotion.o $(CFLAGS)
	
clean :
	rm *.o main seed.out
