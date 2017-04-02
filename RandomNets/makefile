#
#	MAKEFILE
#

# -g: Compilar
# -c: Permitir depuracion posterior

# For "valgrind -v --leak-check=yes --show-reachable=yes "
#CFLAGS =  -g -c -O0

# Normal way
CFLAGS =  -c -O4 -Wall

# Ficheros .c

C = create.c

# Ficheros .o

O = create.o

# Compilacion inicial

.c.o:
	gcc $(CFLAGS) $(C)

# Compilacion final
pfnet_reales_ok:	create.o $(O)
			gcc -o create $(O) -lm

clean:
			rm -rf create *.o
