CC=g++
CPPFLAGS=-g
LDFLAGS=
SRC=main.cpp \
	sha3.cpp \
	main.cpp
INC=sha3.hpp
OBJ=$(SRC:.c=.o)
EXEC=sha3

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS) -I.

%.o: %.cpp
	$(CC) -c $(OBJ) $(CPPFLAGS)

.PHONY: clean cleanall

clean:
	rm *.o

cleanall:
	rm *.o $(EXEC)
