EXECUTABLE = main.exe
CC = gcc
LDFLAGS = -lm
RM = del

src = $(wildcard *.c)
obj = $(src:.c=.o)

all: $(obj)
	$(CC) -o $(EXECUTABLE) $^ $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(obj) $(EXECUTABLE)