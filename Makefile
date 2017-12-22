CXX=clang++ -std=c++14 -g

all: clean build

build: test

clean: 
	rm test
