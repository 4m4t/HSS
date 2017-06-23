CC			=	g++
CPPFLAGS	=	-Wall -W
LDFLAGS		=
SRC			=	sha3.cpp \
				main.cpp
INC			=	sha3.h
OBJ			=	$(SRC:.cpp=.o)
EXEC		=	sha3

all:	$(EXEC)

$(EXEC):	$(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LDFLAGS) -I.

%.o:	%.cpp
	$(CC) $(CPPFLAGS) $(INC) -c $(SRC)

clean:
	rm *.o

cleanall:
	rm *.o $(EXEC)

.PHONY:	clean	cleanall

