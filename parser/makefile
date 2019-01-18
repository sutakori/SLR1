LEXER = $(realpath ../lexer)

SRCS := parser.cpp
OBJS := $(SRCS:%.cpp=%.o)
OBJ_LEXER := $(LEXER)/*.o
HEADS := $(SRCS:%.cpp=%.h) tokens.h

CC = g++
CPPFLAGS = -std=c++11 -g

test/test: test.cpp $(OBJS) $(OBJ_LEXER)
	$(CC) $(CPPFLAGS) -o $@ $^

parser.o: parser.cpp $(HEADS)
	$(CC) $(CPPFLAGS) -c $<
clean:
	-rm test/test $(OBJS)
build: parser.o

test: test/test
all: test
