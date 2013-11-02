CFLAGS=-std=c++11 -g -Wall

eithertest.exe: eithertest.cpp either.h
	g++ ${CFLAGS} eithertest.cpp -o eithertest.exe

test: eithertest.exe
	./eithertest.exe