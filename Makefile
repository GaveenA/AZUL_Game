# Linux (default)
CFLAGS = -Wall

default: all

clean:
	rm -f assignment2
	rm -f assignment2Debug

debug:
	g++ -Wall -Werror -std=c++14 -g -O -o  assignment2Debug main.cpp menu.cpp game.cpp player.cpp board.cpp factory.cpp bag.cpp linkedList.cpp node.cpp tile.cpp

all: main.cpp menu.cpp game.cpp player.cpp board.cpp factory.cpp bag.cpp linkedList.cpp node.cpp tile.cpp 
	g++ -Wall -Werror -std=c++14 -O -o  assignment2 main.cpp menu.cpp game.cpp player.cpp board.cpp factory.cpp bag.cpp linkedList.cpp node.cpp tile.cpp
