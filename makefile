main: main.cpp
	g++ main.cpp -o main `pkg-config opencv --cflags --libs`
