
CC = gcc
LD = $(CC)
DEPEND = mkdep

CFLAGS = -Wall -pedantic`


CFILES =\
       edit_input.c\
       edit_functions.c\
       editing.c\
       svi.c

HFILES =\
       edit_input.h\
       edit_functions.h\
       edit_operator.h\
       string_def.h\
       editing.h
   



svi : edit_input.o edit_functions.o editing.o svi.c 

editing.o : $(HFILES) editing.c 

edit_input.o : edit_functions.h string_def.h edit_operator.h edit_input.h edit_input.c

edit_functions.o : edit_functions.h string_def.h edit_functions.c


