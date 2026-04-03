# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iexternal -I.

# Исходные файлы
PARSERS = $(wildcard src/graph/parsers/*.cpp)
SRC = src/graph/Graph.cpp src/graph/generators/GraphGenerators.cpp src/metrics/GraphMetrics.cpp $(PARSERS) src/graph/serializers/GraphVizSerializer.cpp src/graph/serializers/EdgesSerializer.cpp src/graph/algorithms/DFS.cpp

# Целевые файлы
all: main.exe

# Основная программа
main.exe: main.cpp $(SRC)
	$(CXX) $(CXXFLAGS) main.cpp $(SRC) -o main.exe

# CLI приложение
cli: cli.exe

cli.exe: cli.cpp $(SRC)
	$(CXX) $(CXXFLAGS) cli.cpp $(SRC) -o cli.exe

# Тесты
test: test_serializers.exe test_generators_metrics.exe test_graph.exe

test_serializers.exe: tests/test_serializers.cpp $(SRC)
	$(CXX) $(CXXFLAGS) tests/test_serializers.cpp $(SRC) -o test_serializers.exe

test_generators_metrics.exe: tests/test_generators_metrics.cpp $(SRC)
	$(CXX) $(CXXFLAGS) tests/test_generators_metrics.cpp $(SRC) -o test_generators_metrics.exe

test_graph.exe: tests/test_graph.cpp $(SRC)
	$(CXX) $(CXXFLAGS) tests/test_graph.cpp $(SRC) -o test_graph.exe

# Запуск программ
run: main.exe
	.\main.exe

run-cli: cli.exe
	.\cli.exe

run-test: test_serializers.exe test_generators_metrics.exe test_graph.exe
	.\test_serializers.exe
	.\test_generators_metrics.exe
	.\test_graph.exe

# Очистка
clean:
	del /Q *.exe *.dot *.edges 2>nul || true

# Справка
help:
	@echo Available targets:
	@echo   make        - Build main.exe
	@echo   make cli    - Build cli.exe
	@echo   make test   - Build all tests
	@echo   make run    - Build and run main.exe
	@echo   make run-cli - Build and run cli.exe
	@echo   make run-test - Run all tests
	@echo   make clean  - Remove .exe, .dot and .edges files
	@echo   make help   - Show this help

.PHONY: all cli test run run-cli run-test clean help