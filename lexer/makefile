SRCS_TOKEN := space.cpp num.cpp operator.cpp word.cpp error.cpp lstr.cpp
HEAD_TOKEN = $(SRC_TOKEN:%.cpp=%.h)
OBJS_TOKEN = $(SRC_TOKEN:%.cpp=%.o)

SRCS_IND = reader.cpp token.cpp tag.cpp
HEAD_IND = $(SRC_IND:%.cpp=%.h)
OBJS_IND = $(SRC_IND:%.cpp=%.o)

SRCS := $(SRCS_TOKEN) $(SRCS_IND) lexer.cpp
OBJS := $(SRCS:%.cpp=%.o)
HEADS := $(SRCS:%.cpp=%.h)

SRCS_TEST := $(SRCS) test.cpp
OBJS_TEST = $(SRCS_TEST:%.cpp=%.o)

CC = g++
CPPFLAGS = -std=c++11 -g

test/test: test.cpp $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^

lexer.o: lexer.cpp $(HEADS)
	$(CC) $(CPPFLAGS) -c $<

$(OBJS_IND): %.cpp %.h
	$(CC) $(CPPFLAGS) -c $<

$(OBJS_TOKEN): %.cpp %.h
	$(CC) $(CPPFLAGS) -c $<

.PHONY: clean build test all
clean:
	-rm test/test $(OBJS)
build: lexer.o

test: test/test
all: test
