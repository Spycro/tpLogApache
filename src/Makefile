ECHO=@echo
RM=@rm
COMP=g++
EDL=g++
RMFLAGS=-f
CPPFLAGS=-Wall -Werror -ansi -pedantic -std=c++11
DEBUGFLAGS= -DMAP -g
EDLFLAGS=
INT=
REAL=$(INT:.h=.cpp)
OBJ=$(INT:.h=.o)
EXE=analog
LIBS= #-lpthread ?
CLEAN=clean

.PHONY : $(CLEAN)

default : $(EXE)

debug: CPPFLAGS+= $(DEBUGFLAGS)
debug: $(EXE)

analog.o : analog.cpp


%.o : %.cpp
	$(ECHO) compil de \<$@\>
	$(COMP) $(CPPFLAGS) -c $<


$(EXE) : $(OBJ)
	$(ECHO) "edition des liens de <$(EXE)>"
	$(EDL) -o ../bin/$(EXE) $(OBJ) $(LIBS)

$(CLEAN) :
	$(RM) $(RMFLAGS) $(OBJ) $(EXE) core

#options de make :
#	-s silence
#	-t touch
#	-p toutes les réglès implicites + info du makefile dans le meme dossier
#	-i ingore errors in commands
#	-k keep going after erros
