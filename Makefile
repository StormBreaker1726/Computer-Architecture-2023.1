CXX=g++
CFLAGS=-Wall -Wextra -std=c++11 -I ./include

taca: main.cpp ./source/*.cpp ./include/*
	${CXX} ${CFLAGS} -o $@ ./source/*.cpp main.cpp
