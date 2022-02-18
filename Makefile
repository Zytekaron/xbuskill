CC=clang++

build: main.cpp
	mkdir build
	$(CC) main.cpp --std=c++17 -o build/xbuskill

install:
	cp build/xbuskill /usr/bin/xbuskill

clean:
	rm -rf build

run:
	./build/xbuskill
