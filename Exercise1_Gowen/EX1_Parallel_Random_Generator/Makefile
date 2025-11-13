
CC = g++
CFLAGS = -Wall -O3 -std=c++23

TARGET = main
OBJS = main.o random.o MonteCarloIntegration.o ProbabilityDistribution.o Buffon.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

random.o: random.cpp random.h
	$(CC) $(CFLAGS) -c random.cpp -o random.o

MonteCarloIntegration.o: MonteCarloIntegration.cpp MonteCarloIntegration.h
	$(CC) $(CFLAGS) -c MonteCarloIntegration.cpp -o MonteCarloIntegration.o

ProbabilityDistribution.o: ProbabilityDistribution.cpp ProbabilityDistribution.h
	$(CC) $(CFLAGS) -c ProbabilityDistribution.cpp -o ProbabilityDistribution.o

Buffon.o: Buffon.cpp Buffon.h
	$(CC) $(CFLAGS) -c Buffon.cpp -o Buffon.o

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET) seed.out

