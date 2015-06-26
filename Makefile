all: clean build tests

clean:
	rm -f tests

build:
	g++ -g -o tests -Wall -fmessage-length=0 tests.cpp

tests: build
	./tests

valgrind: build
	valgrind --leak-check=full --track-origins=yes ./tests
