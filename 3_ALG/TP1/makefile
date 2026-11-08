CC = gcc-14
CFLAGS = -Wall -Wextra -Werror -g -std=c23
MAIN = prova1_20240487
OBJ = main.o arvoreB.o auxiliares.o
HDR = arvoreB.h auxiliares.h
 
$(MAIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJ)
 
main.o: main.c $(HDR)
	$(CC) $(CFLAGS) -c main.c
 
arvoreB.o: arvoreB.c arvoreB.h
	$(CC) $(CFLAGS) -c arvoreB.c
 
auxiliares.o: auxiliares.c auxiliares.h
	$(CC) $(CFLAGS) -c auxiliares.c
 
run: $(MAIN)
	./$(MAIN)
 
valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)
 