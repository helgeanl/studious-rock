#https://www.gnu.org/software/make/manual/make.html#Introduction

# program executable name
TARGET = RTLINUX
CC = gcc
# compiler flags
CFLAGS = -g -Wall -D_GNU_SOURCE

# linker flags
LDFLAGS = -g -lrt -pthread -lcomedi  -lm

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.o)

# link programs
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# compile
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)
