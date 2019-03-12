CXX	   := gcc
CXXFLAGS  := -Wall -std=c99

TARGET	:= mpu6050test
PROJ_ROOT := $(realpath .)
SRCS	  := main.c i2c.c mpu6050.c
OBJS	  := $(SRCS:.c=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)

.PHONY: make
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.PHONY: clean
clean:
	$(RM) -f $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<
