CC=g++
CFLAGS=-c -Wall -g 
INC=/polusfs/soft/magma-2.4.0_open_blas/include /polusfs/soft/PGI/linuxpower/2018/cuda/9.1/include
INC_PARAMS=$(foreach d, $(INC), -I$d)
LDFLAGS=-L /polusfs/soft/magma-2.4.0_open_blas/lib -lmagma -L /polusfs/soft/PGI/linuxpower/2018/cuda/9.1/lib64 -lcublas -L /polusfs/soft/openblas-0.3.4/lib -lopenblas
SOURCES=cmain.cpp cmatrix.cpp $(wildcard alglib/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=t1

all: $(SOURCES) $(EXECUTABLE)
	
clean: 
	rm -rf *.o
	
submit:
	mpisubmit.pl -n 2 -w 00:15 --gpu t1 "test"

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(INC_PARAMS) $(CFLAGS) $< -o $@
	