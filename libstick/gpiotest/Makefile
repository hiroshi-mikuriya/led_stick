CXX			:= gcc
CXXFLAGS	:= -Wall -std=c99

TARGET		:= gpiotest
PROJ_ROOT	:= $(realpath .)
SRCS		:= main.c
OBJS		:= $(SRCS:.c=.o)
LIBS		:= -lstick

### Rules ######################################################################

.PHONY: all
all: $(TARGET)

.PHONY: make
make: $(TARGET)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) -f $(TARGET) *.o
