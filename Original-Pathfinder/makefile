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

C = pfnet.c funciones.c routines.c

# Ficheros .o

O = pfnet.o funciones.o routines.o

# Compilacion inicial

.c.o:
	gcc $(CFLAGS) $(C)

# Compilacion final
pathfinder:	pfnet.o $(O)
	gcc -o pathfinder $(O) -lm

clean:
	rm -rf pathfinder *.o

