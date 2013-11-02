eithertest.exe: eithertest.cpp either.h
	g++ -std=c++11 eithertest.cpp -o eithertest.exe

test: eithertest.exe
	./eithertest.exe