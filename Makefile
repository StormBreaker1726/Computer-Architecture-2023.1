CXX=g++
CFLAGS=-Wall -Wextra -std=c++11 -I ./include

taca: main.cpp ./source/*.cpp ./include/*
	${CXX} ${CFLAGS} -o $@ ./source/*.cpp main.cpp

zip:
	zip -r trabalho-arq-comp main.cpp LICENSE source/ include/ inputs scripts Makefile CMakeLists.txt README.md

.PHONY: zip
