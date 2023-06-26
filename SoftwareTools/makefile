CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm
OBJFILES = main.o ternary.o arithmetic.o util.o avalanche.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
