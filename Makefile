build: main.cpp
	g++ -std=c++11 -o build/main.exe main.cpp

debug: main.cpp
	g++ -std=c++11 -o build/main.exe main.cpp -D DEBUG

run:
	cd build && main