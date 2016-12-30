CC=g++
CPPFLAGS=-g
LDFLAGS=
SRC=main.cpp \
	hexa.cpp
INC=hexa.hpp
OBJ=$(SRC:.c=.o)
EXEC=hexa

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
