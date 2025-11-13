CC      = g++
CFLAGS  = -Wall -O3 -std=c++20

TARGET  = Monte_Carlo_ISING_1D
OBJS    = Monte_Carlo_ISING_1D.o random.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

Monte_Carlo_ISING_1D.o: Monte_Carlo_ISING_1D.cpp
	$(CC) $(CFLAGS) -c Monte_Carlo_ISING_1D.cpp -o Monte_Carlo_ISING_1D.o

random.o: random.cpp random.h
	$(CC) $(CFLAGS) -c random.cpp -o random.o

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) \
	      ./Output_data/output_*.* \
	      seed.out \
	      config.final