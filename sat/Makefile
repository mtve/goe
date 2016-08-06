PROG		= main
OBJ		= main.o picosat.o
HDR		= sat_pico.h sat_cms.h

CC		= g++
CXXFLAGS	= -W -Wall -std=gnu++11 -march=native -O3 -I../src/cryptominisat/src
CFLAGS		= -W -Wall -march=native -O3 -fpermissive
LDLIBS		= -L/usr/local/lib -lcryptominisat5

$(PROG): $(OBJ)

#	g++ picosat.o main.o -L/usr/local/lib -lcryptominisat5 -o main

$(OBJ): $(HDR)

main.cpp: gen.inc

gen.inc: gen.pl
	perl ./gen.pl >gen.inc

clean:
	rm -f $(PROG) $(OBJ)
