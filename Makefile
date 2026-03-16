CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: task-cli test run-tests

# Help target
help:
	@echo "========================================"
	@echo "    TASK TRACKER CLI - MAKE TARGETS    "
	@echo "========================================"
	@echo ""
	@echo "Targets:"
	@echo "  make               Compile the task-cli executable"
	@echo "  make task-cli      Compile the task-cli executable"
	@echo "  make clean         Remove generated files"
	@echo "  make help          Show this help message"
	@echo ""

# compilar programa principal
task-cli: main.cc task_manager.cpp
	$(CXX) $(CXXFLAGS) -o task-cli main.cc task_manager.cpp

# ejecutar tests
run-tests: test
	./test_runner

test: test_task_manager.cpp task_manager.cpp
	$(CXX) $(CXXFLAGS) -o test_runner test_task_manager.cpp task_manager.cpp -lgtest -pthread

clean:
	rm -f task-cli test_runner test_tasks.json

.PHONY: help task-cli run-tests clean tar

tar: clean
	tar -czf task-tracker-cli-v2.tar.gz *.cc *.h *.hpp Makefile README.md LICENSE tasks.json