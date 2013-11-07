CFLAGS=-std=c++11 -g -Wall

eithertest.exe: eithertest.cpp either.h TestFramework.h
	g++ ${CFLAGS} eithertest.cpp -o eithertest.exe

optionaltest.exe: optionaltest.cpp optional.h TestFramework.h
	g++ ${CFLAGS} optionaltest.cpp -o optionaltest.exe

test: eithertest.exe optionaltest.exe
	./eithertest.exe && ./optionaltest.exe