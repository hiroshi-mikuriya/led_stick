CXX       := g++
CXXFLAGS  := -Wall -std=c++0x -v

TARGET    := image_writer
PROJ_ROOT := $(realpath ..)
VPATH     := $(PROJ_ROOT)
INCLUDES  := `pkg-config opencv --cflags` \
	     -I $(PROJ_ROOT)
SRCS      := main.cpp
LIBS      := -lpthread \
	     -ldl \
	     `pkg-config opencv --libs` \
	     -lboost_system \
		 -lboost_program_options \
	     -lm

UNAME := ${shell uname}
ifeq ($(UNAME), Linux)
CXXFLAGS := $(CXXFLAGS)
LIBS := $(LIBS) -lstick_sdk
endif

OBJS      := $(SRCS:.cpp=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)
.PHONY: make
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) -f $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<
