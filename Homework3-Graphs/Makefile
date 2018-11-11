build : livrare

livrare: main1.c task1.o task2.o task3.o 
	gcc -Wall main1.c task1.o task2.o task3.o -o livrare -g
task1.o: task1.c task1.h
	gcc -c -Wall task1.c -o task1.o -g 
task2.o:task2.c task2.h
	gcc -c -Wall task2.c -o task2.o -g
task3.o:task3.c task3.h
	gcc -c -Wall task3.c -o task3.o -g
clean:
	rm task1.o task2.o task3.o livrare
