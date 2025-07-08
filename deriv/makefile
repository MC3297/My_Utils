compile: main.o create.o deriv.o syntree.o token.o
	g++ main.o create.o deriv.o syntree.o token.o -o derivate

main.o: main.cpp create.h nodes.h syntree.h token.h
	g++ -c main.cpp

create.o: create.h nodes.h create.cpp
	g++ -c create.cpp

deriv.o: nodes.h create.h deriv.cpp
	g++ -c deriv.cpp

syntree.o: syntree.h create.h nodes.h token.h syntree.cpp
	g++ -c syntree.cpp

token.o: token.h token.cpp
	g++ -c token.cpp


clean:
	rm *.o *.out derivate