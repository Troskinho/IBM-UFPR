CC = gcc-14
CFLAGS = -Wall -Wextra -Werror -g -std=c23
MAIN = prova2_20240487
OBJ = main.o arvoreB.o auxiliar.o
HDR = arvoreB.h auxiliar.h
 
$(MAIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJ)
 
main.o: main.c $(HDR)
	$(CC) $(CFLAGS) -c main.c
 
arvoreB.o: arvoreB.c arvoreB.h
	$(CC) $(CFLAGS) -c arvoreB.c
 
auxiliar.o: auxiliar.c auxiliar.h
	$(CC) $(CFLAGS) -c auxiliar.c
 
run: $(MAIN)
	./$(MAIN)
 
valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)
 