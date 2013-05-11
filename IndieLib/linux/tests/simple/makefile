 

# Automated Makefile

LIBS=-L. -lIndieLib -lSDL -lGLEW -lGLU -lGL -lILU -lILUT

CC = g++

CFLAGS = -Wall -O2

COMPILE = $(CC) $(CFLAGS) -c

OBJFILES = ../../../tests/CIndieLib.o simple.o



all: step1



step1: $(OBJFILES)

	$(CC) -o simple $(OBJFILES) $(LIBS)

clean:
	\rm *.o *.log ../../../tests/CIndieLib.o simple
