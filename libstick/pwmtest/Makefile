CXX			:= gcc
CXXFLAGS	:= -Wall -std=c99

TARGET		:= pwmtest
PROJ_ROOT	:= $(realpath .)
SRCS		:= main.c
OBJS		:= $(SRCS:.c=.o)
LIBS		:= -lbcm2835

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
