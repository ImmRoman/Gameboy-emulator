CC = gcc
CFLAGS = -Wall -g
TARGET = main
SRCS = main.c cpu.c 
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

# Rule to link the object files and create the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile individual .c files into .o (object) files
# It also specifies that they depend on cpu.h
%.o: %.c cpu.h
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)
