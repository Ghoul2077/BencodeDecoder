all: encoder.cpp encoder.h utils.cpp utils.h
	g++ -o encoder.exe encoder.cpp utils.cpp
	./encoder.exe

clean:
	-rm -f encoder.exe encoder.out
