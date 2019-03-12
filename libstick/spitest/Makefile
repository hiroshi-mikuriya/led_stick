CXX			:= gcc
CXXFLAGS	:= -Wall -std=c99

TARGET		:= spitest
PROJ_ROOT	:= $(realpath .)
SRCS		:= main.c ../spi.c
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
