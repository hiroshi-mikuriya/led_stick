CXX	   := g++
CXXFLAGS  := -Wall -std=c++0x -v

TARGET	:= sato
PROJ_ROOT := $(realpath .)
VPATH	 := $(PROJ_ROOT)
INCLUDES  := `pkg-config opencv --cflags`
SRCS	  := main.cpp stick_sdk.c
LIBS	  := -lpthread \
			 -ldl \
			 `pkg-config opencv --libs` \
			 -lboost_system \
			 -lboost_program_options \
			 -lbcm2835 \
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
