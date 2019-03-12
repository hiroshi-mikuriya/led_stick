CXX			:= gcc
CXXFLAGS	:= -Wall -std=c99 -fPIC

TARGET		:= libstick.so
HEADER		:= stick.h
PROJ_ROOT	:= $(realpath .)
SRCS		:= stick.c spi.c i2c.c gpio.c
OBJS		:= $(SRCS:.c=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)

.PHONY: make
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -shared -o $@ $(OBJS)

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<

.PHONY: install
install:
	install $(TARGET) /usr/local/lib
	install $(HEADER) /usr/local/include

.PHONY: uninstall
uninstall:
	rm /usr/local/lib/$(TARGET)
	rm /usr/local/include/$(HEADER)
