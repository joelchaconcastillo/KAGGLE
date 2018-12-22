# Makefile for compiling a the idraw
CC=g++
LD=g++
RM=rm -f
#Production flags
CFLAGS=-Wno-unused-result -std=c++11 -fopenmp
LDFLAGS=
UBUNTUFLAGS=-lm $(LDFLAGS) -lstdc++ -std=c++11 -fopenmp
OPENSUSEFLAGS=-lm $(LDFLAGS)
DEBUG=-O2
OBJS:=genetic.o
MAIN=Ejecutable
all:$(MAIN)
ubuntu:
	$(MAKE) "OPENSUSEFLAGS="
opensuse:
	$(MAKE) "UBUNTUFLAGS="
$(MAIN):$(OBJS)
	$(LD) $(DEBUG) $(OPENSUSEFLAGS) $(OBJS) -o $(MAIN) $(UBUNTUFLAGS)
%.o: %.cpp %.hpp 
	$(CC) $(CFLAGS) $(DEBUG) -c $<
%.o: %.c %.h
	$(CC) $(CFLAGS) $(DEBUG) -c $<	
%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -c $<	
%.o: %.cpp 
	$(CC) $(CFLAGS) $(DEBUG) -c $<	

debug:
	$(MAKE) "DEBUG=-g"
clean:
	$(RM) $(OBJS) *.*~
cleanall:
	$(RM) $(OBJS) $(MAIN) *.*~
	
