##### CAMPI_Frontend Makefile ####

CC = gcc

EXE := MOBILE_Forces_Gen

CFLAGS := -O3 -Wall

LFLAGS := -lm

OBJS :=	forces_gen.o \
		main.o

HDRS :=	forces_gen.h

all: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(PTHREAD) $(LFLAGS)

%.o:%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(EXE)
