all: encoder.cpp encoder.h bEncodeToken.cpp bEncodeToken.h utils.cpp utils.h
	g++ -g -o encoder.exe encoder.cpp bEncodeToken.cpp utils.cpp
	./encoder.exe

clean:
	-rm -f encoder.exe encoder.out
