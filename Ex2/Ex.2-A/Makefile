#https://www.gnu.org/software/make/manual/make.html#Introduction

# program executable name
TARGET =  fork
CC = gcc
# compiler flags
CFLAGS = -g -Wall

# linker flags
LDFLAGS = -g

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.o)

# link programs
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

# compile
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)
