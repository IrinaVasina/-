CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall
TARGET = lab3

# Основная цель
all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

# Очистка
clean:
	rm -f $(TARGET).exe $(TARGET) measurements.csv

# Запуск
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run