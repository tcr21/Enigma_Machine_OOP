enigma: main.o enigma.o plugboard.o reflector.o rotor.o
	g++ -Wall -g main.o enigma.o plugboard.o reflector.o rotor.o -o enigma

main.o: main.cpp enigma.h
	g++ -Wall -g -c main.cpp -o main.o

enigma.o: enigma.cpp enigma.h
	g++ -Wall -g -c enigma.cpp -o enigma.o

plugboard.o: plugboard.cpp plugboard.h
	g++ -Wall -g -c plugboard.cpp -o plugboard.o

reflector.o: reflector.cpp reflector.h
	g++ -Wall -g -c reflector.cpp -o reflector.o

rotor.o: rotor.cpp rotor.h
	g++ -Wall -g -c rotor.cpp -o rotor.o

clean:
	rm -f *.o enigma