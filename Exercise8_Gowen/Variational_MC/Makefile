# Makefile Variational_MC EX.8

CC 		= g++
CFLAGS 	= -Wall -O3 -std=c++23 

TARGET 	= Variational_MC
OBJS 	= Variational_MC.o random.o

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)