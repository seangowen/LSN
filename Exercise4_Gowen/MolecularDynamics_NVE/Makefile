# TEMPLATE Makefile Gowen

CC      = g++
CFLAGS  = -Wall -O3 -std=c++23

TARGET  = MolDyn
OBJS    = MolDyn_NVE_new.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

MolDyn_NVE_new.o: MolDyn_NVE_new.cpp MolDyn_NVE_new.h
	$(CC) $(CFLAGS) -c MolDyn_NVE_new.cpp -o MolDyn_NVE_new.o

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) output_*.dat
