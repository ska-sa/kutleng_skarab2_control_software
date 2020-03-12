# the compiler to use
CC = gcc

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  =-Wall
  
#files to link:
LIBFLAGS =libxilinxbitstream.so
  
# the name to use for both the target source file, and the output file:
TARGET = prconfig

all: xilinxbitstream.c $(TARGET)
	gcc -shared -o libxilinxbitstream.so -fPIC xilinxbitstream.c 
  
$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c 

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

clean:
	rm $(TARGET).o $(TARGET) *.so
