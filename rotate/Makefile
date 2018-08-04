CXX	   := g++
CXXFLAGS  := -Wall -std=c++0x -v

TARGET	:= rotate
PROJ_ROOT := $(realpath .)
VPATH	 := $(PROJ_ROOT)
INCLUDES  := `pkg-config opencv --cflags`
SRCS	  := main.cpp
LIBS	  := -lpthread \
			 -ldl \
			 `pkg-config opencv --libs` \
			 -lm

OBJS	  := $(SRCS:.cpp=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)
.PHONY: make
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS) -L../lib

.PHONY: clean
clean:
	$(RM) -f $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<
