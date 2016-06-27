CC    	= g++
C++	    = g++
CFLAGS 	= -O3 -std=gnu++0x -Wall
EXEC	= neuron.cpp

.SUFFIXES:.o .f
.f.o: ; $(F77) -c $(CFLAGS) $<

.SUFFIXES:.o .c
.c.o: ; $(CC)  -g -c $(CFLAGS) $(INCLUDE) $<

.SUFFIXES:.o .cc
.cc.o: ; $(C++) -c $(CFLAGS) $<

all: $(OBJ)
	$(CC) $(EXEC) $(CFLAGS) $(LINK)

try:
	make all
	./a.out