CC = g++
CFLAGS = -std=c++11 -O3
INC = -I/$$GUROBI_HOME/include/
LIB = -L/$$GUROBI_HOME/lib -lm -lpthread

all: main_multithreading

main_multithreading: main_multithreading.o 
	$(CC) -o $@ $^ $(CFLAGS) $(INC) $(LIB)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(INC)

clean:
	/bin/rm -f *.o main main_test main_multithreading
