CC=clang
CXX=clang++
LD=clang++
CFLAGS=-g -Iinclude -Wall
CXXFLAGS=--std=c++20 -g -Iinclude -Wall
LDFLAGS=-g -ldl $(shell pkg-config --libs glfw3)
CSRC=$(shell find . -name '*.c')
CXXSRC=$(shell find . -name '*.cpp')
OBJ=$(CSRC:.c=.o) $(CXXSRC:.cpp=.o)

under-sea: $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ)
	rm -f under-sea
