CC = gcc
# CFLAGS = -Wall -Wextra -Werror -g
CFLAGS = -Wall -Wextra -g

# for linking use install_name_tool to change the 
# install path embedded in the .dylib binary 
# (where the library expects to be found, double 
# check the explanation)
# install_name_tool -id <path to .dylib> <new path .dylib uses to look for itself>

#LDFLAGS = -L/Users/ace/Documents/projects/C/libraries/libcfu/src/.libs
LDFLAGS = -L./libs/libcfu/src/.libs
LDLIBS = -lcfu
# INCLUDES = -I/Users/ace/Documents/projects/C/libraries/libcfu/src
INCLUDES = -I./libs/libcfu/src


# name of executable
TARGET = cheetah

# Automatically find all .c and .h files in the src directory
SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)

# Object files (derived from the source files)
OBJS = $(SRCS:.c=.o)

# The default target
all: $(TARGET)

# Rule to compile source files into object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) ${INCLUDES} -c $< -o $@

# Rule to link the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) ${LDFLAGS} -o $(TARGET) $(OBJS) ${LDLIBS}

# Rule to clean up the build directory
clean:
	rm -f $(OBJS) $(TARGET)
