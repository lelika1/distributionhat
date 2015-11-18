bin/libHelloFastCGI.so: bin/main.o
	mkdir -p bin && g++ -O2 -fPIC bin/main.o -lfastcgi-daemon2 -shared -o bin/libHelloFastCGI.so

bin/main.o: main.cpp
	g++ -c main.cpp -o bin/main.o -O2 --std=c++11
