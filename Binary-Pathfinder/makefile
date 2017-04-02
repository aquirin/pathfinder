#
#	MAKEFILE
#

# -g: Compilar
# -c: Permitir depuracion posterior

# For "valgrind -v --leak-check=yes --show-reachable=yes "
#CFLAGS =  -g -c -O0

# Normal way
CFLAGS =  -c -O3 -Wall

# Ficheros .c

C = binary.c routines.c

# Ficheros .o

O = binary.o routines.o

# Compilacion inicial

.c.o:
	gcc $(CFLAGS) $(C)

# Compilacion final
binary:	$(O)
	gcc -o binary $(O) -lm

clean:
	rm -rf binary *.o
