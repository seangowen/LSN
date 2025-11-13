# Makefile EX2 Gowen

CC = g++
CFLAGS = -Wall -O3 -std=c++23

TARGET = main
OBJS = main.o random.o statistics.o RandomWalk.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

random.o : random.cpp random.h
	$(CC) $(CFLAGS) -c random.cpp -o random.o 

statistics.o : statistics.cpp statistics.h
	$(CC) $(CFLAGS) -c statistics.cpp -o statistics.o 

RandomWalk.o : RandomWalk.cpp RandomWalk.h
	$(CC) $(CFLAGS) -c RandomWalk.cpp -o RandomWalk.o 


#Note: Without .PHONY, if a file named clean exists in the directory, 
# running make clean might do nothing, because make thinks clean is up to date.
.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
