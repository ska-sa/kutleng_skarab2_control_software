# the compiler to use
CC = gcc

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  =-Wall
  
#files to link:
  
# the name to use for both the target source file, and the output file:
TARGET = testfilegen

all: $(TARGET)
  
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c  -o $(TARGET) 

clean:
	rm $(TARGET) 
