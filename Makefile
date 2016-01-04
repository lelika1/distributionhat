bin/libHelloFastCGI.so: main.cpp faculties.cpp storage.cpp
	mkdir -p bin && g++ --std=c++11 -O2 -fPIC \
	-I. -Irapidjson/include \
	main.cpp faculties.cpp storage.cpp \
	-lfastcgi-daemon2 -shared -o bin/libDHatFastCGI.so
