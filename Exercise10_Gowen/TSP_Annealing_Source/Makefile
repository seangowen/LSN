# Makefile Simulated Annealing TSP Ex.10 Gowen

# =========================
# Compiler
# =========================
CXX = mpic++              # Use MPI C++ compiler wrapper
CXXFLAGS = -Wall -O3 -std=c++23

# =========================
# Target
# =========================
TARGET = TSP
OBJS = main.o geneticAlgorithm.o simulatedAnnealing.o helperFunctions.o routineFunctions.o citiesTour.o

# =========================
# Build executable
# =========================
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# =========================
# Compile object files
# =========================
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp 

geneticAlgorithm.o: geneticAlgorithm.cpp geneticAlgorithm.h 
	$(CXX) $(CXXFLAGS) -c geneticAlgorithm.cpp

simulatedAnnealing.o: simulatedAnnealing.cpp simulatedAnnealing.h
	$(CXX) $(CXXFLAGS) -c simulatedAnnealing.cpp

helperFunctions.o: helperFunctions.cpp helperFunctions.h
	$(CXX) $(CXXFLAGS) -c helperFunctions.cpp

routineFunctions.o: routineFunctions.cpp routineFunctions.h
	$(CXX) $(CXXFLAGS) -c routineFunctions.cpp

citiesTour.o: citiesTour.cpp citiesTour.h
	$(CXX) $(CXXFLAGS) -c citiesTour.cpp

# =========================
# Clean up
# =========================
.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)

