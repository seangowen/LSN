# Metropolis_QM Makefile
# Ex.5 Sean Gowen

CC = g++
CFLAGS = -Wall -O3 --std=c++23

TARGET = main
OBJS = main.o random.o metropolis.o hydrogenPdf.o position.o blockedStatistics.o

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Phony targets
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
