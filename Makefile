CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = main.c utils.c storage.c manager.c
# Object files
OBJS = $(SRCS:.c=.o)
# Output executable
TARGET = travel_app

# Default target
all: $(TARGET)

# Link objects to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile individual source files
main.o: main.c common.h utils.h storage.h manager.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h common.h
	$(CC) $(CFLAGS) -c utils.c

storage.o: storage.c storage.h common.h utils.h
	$(CC) $(CFLAGS) -c storage.c

manager.o: manager.c manager.h common.h utils.h storage.h
	$(CC) $(CFLAGS) -c manager.c

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean