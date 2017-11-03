enigma: enigma.o main.o
	g++ -g enigma.o main.o -o enigma

enigma.o: enigma.cpp enigma.h
	g++ -Wall -g -c enigma.cpp

main.o: main.cpp enigma.h
	g++ -Wall -g -c main.cpp

clean:
	rm -f *.o enigma
