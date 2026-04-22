CC=gcc
CFLAGS=-Wall

TARGET=MyShell

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

debug:
	@echo "Source files: $(SRC)"
	@echo "Object files: $(OBJ)"
clean:
	rm -f *.o $(TARGET)
