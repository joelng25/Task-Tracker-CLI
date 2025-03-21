all: task-cli

task-cli: main.cc
	g++ -Wall -O2 -o task-cli main.cc

clean:
	rm -f task-cli *.exe
