#https://www.gnu.org/software/make/manual/make.html#Introduction

# program executable name
TARGET = xeno
CC = gcc


# Xenomai flags
XENO_DESTDIR:=
XENO_CONFIG:=$(XENO_DESTDIR)/usr/bin/xeno-config
XENO_CFLAGS:=$(shell DESTDIR=$(XENO_DESTDIR) $(XENO_CONFIG) --skin native --cflags)
XENO_LIBS:=$(shell DESTDIR=$(XENO_DESTDIR) $(XENO_CONFIG) --skin native --ldflags)

# compiler flags
CFLAGS = -g -Wall -D_GNU_SOURCE $(XENO_CFLAGS)

# linker flags
LDFLAGS = -g -lrt -pthread -lcomedi  $(XENO_LIBS) -lm




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
