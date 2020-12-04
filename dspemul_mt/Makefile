EXECUTABLE = main.exe
CC = gcc
CPP = g++
CPPFLAGS = -std=c++11 -pthread
LDFLAGS = -lm
RM = del

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
CSRC = $(wildcard *.c)
COBJ = $(CSRC:.c=.o)

all: $(OBJ) $(COBJ)
	$(CPP) $^ $(LDFLAGS) -o $(EXECUTABLE)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c "$<" -o "$@"

%.o: %.c
	$(CC) -c "$<" -o "$@"

.PHONY: clean
clean:
	$(RM) $(OBJ) $(COBJ) $(EXECUTABLE)