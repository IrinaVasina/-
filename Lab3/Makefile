CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall
TARGET = lab3

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET).exe $(TARGET) measurements.csv

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
