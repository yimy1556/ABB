
CC=gcc
CFLAGS= -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -o
VALGRIND=valgrind --leak-check=full --track-origins=yes --show-reachable=yes
OBJET=pruebas
ARCH_C= main.c cola.c abb.c pruebas_abb.c testing.c 

all:valgrind
	
	echo
	notify-send "Compilado!" -t 1500

valgrind:compilar
	
	echo
	$(VALGRIND) ./$(OBJET)

compilar:
	
	echo
	$(CC) $(ARCH_C) $(CFLAGS) $(OBJET)
	
clean:

	rm $(OBJET)
