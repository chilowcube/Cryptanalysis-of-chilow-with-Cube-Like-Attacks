CFLAGS = -O3 -std=c++17
LDFLAGS = -pthread 
CC = g++

target: main 

main: main.o chilow.o split.o polynomial.o  
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


%.o:%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -f *.o main main_application
