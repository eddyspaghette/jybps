CC=g++
DEBUG=-g -Wall
CFLAGS=-I /usr/include/boost_1_77_0 -lpthread
OBJ=jybps.o

%.o: %.cpp
	$(CC) -c -o $@ $< $(DEBUG) $(CFLAGS)

jybps: $(OBJ)
	$(CC) -o $@ $^ $(DEBUG) $(CFLAGS)

.PHONY: clean


clean:
	rm -f *.o jybps
