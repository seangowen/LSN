# TEMPLATE Makefile Gowen

CC      = g++
CFLAGS  = -Wall -O3 -std=c++23

TARGET  = MC_NVT
OBJS    = MC_NVT.o random.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) istant_output.dat
