#
#	MAKEFILE
#

# -g: Compilar
# -c: Permitir depuracion posterior

# For "valgrind -v --leak-check=yes --show-reachable=yes "
#CFLAGS =  -g -c -O0

# Normal way
CFLAGS =  -c -O3

# Ficheros .c

C = pfnet.c funciones.c io_net.c

# Ficheros .o

O = pfnet.o funciones.o io_net.o

# Compilacion inicial

.c.o:
	gcc $(CFLAGS) $(C)

# Compilacion final
fast-pathfinder:	pfnet.o $(O)
			gcc -o fast-pathfinder $(O) -lm

clean:
	rm -rf fast-pathfinder *.o
