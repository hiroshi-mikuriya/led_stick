CXX			:= g++
CXXFLAGS	:= -Wall -std=c++11

TARGET		:= show_pics
PROJ_ROOT	:= $(realpath .)
INCLUDES	:= `pkg-config opencv --cflags`
SRCS		:= main.cpp
LIBS		:= `pkg-config opencv --libs` -lm -lstick
OBJS		:= $(SRCS:.cpp=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)

.PHONY: make
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<
