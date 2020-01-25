# What to do?
TARGET = frequency

# C compiled r and linker flags
CC = gcc
#
CPPFLAGS = -I../inc
#CFLAGS   = -g3 -pedantic -ansi -Wall -Werror -DDEBUG
CFLAGS   = -g3 -Wall -Werror
LDFLAGS  = -g3

.PHONY: all clean

all: $(TARGET)

# Linker built-in rule
#%: %.o
#	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)
$(TARGET): trie.o main.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)
# C compiler built-in rule
#%.o: %.c
#	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $(@) $(<)

trie.o: trie.c trie.h 
main.o: main.c

clean:
	rm -f *.o $(TARGET)

test: $(TARGET)
	./$(TARGET) -v

