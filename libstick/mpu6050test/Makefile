CXX			:= gcc
CXXFLAGS	:= -Wall -std=c99

TARGET		:= mpu6050test
PROJ_ROOT	:= $(realpath .)
SRCS		:= main.c mpu6050.c ../i2c.c
INCLUDES	:= ../
OBJS		:= $(SRCS:.c=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)

.PHONY: make
make: $(TARGET)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.PHONY: clean
clean:
	$(RM) -f $(TARGET) *.o
