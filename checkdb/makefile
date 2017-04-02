#
#	MAKEFILE
#

# -g: Compilar
# -c: Permitir depuracion posterior

# For "valgrind -v --leak-check=yes --show-reachable=yes "
#CFLAGS =  -g -c -O0
#CPPFLAGS =  -g -c -O0 -Wno-deprecated

# Normal way
CFLAGS =  -c -O3 -Wall
CPPFLAGS =  -c -O3 -Wno-deprecated -Wall

# Normal way (cygwin)
#CFLAGS =  -c -O3 -Dcygwin

# Ficheros .o

O_INPUT = input_pajek.o input_subdue.o input_gaston.o input_gspan.o input_ffsm.o input_prolog.o
O_OUTPUT = output_pajek.o output_subdue.o output_gaston.o output_gspan.o output_dot.o output_human.o output_plain.o output_jxplore.o
O = routines.o memory.o graph.o graphops.o main.o $(O_INPUT) $(O_OUTPUT)

# Compilacion inicial

.c.o:
	gcc $(CFLAGS) $<

.cpp.o:
	g++ $(CPPFLAGS) $<

# Compilacion final
all:	$(O)
	g++ -o checkdb $(O) -lm
	
clean:
	rm -rf checkdb *.o

zip:
	rm -f checkdb.zip
	zip -rq9 checkdb.zip makefile *.cpp *.c *.h
