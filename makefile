
CFLAGS = -ggdb -std=c++11 -Wall

all: Player_Board.x

Player.o: Player.cpp Player.hpp Coordinate.cpp
	g++ -g -std=c++11 -c Player.cpp -o Player.o

GameBoard.o: GameBoard.cpp GameBoard.hpp
	g++ $(CFLAGS) -c GameBoard.cpp -o GameBoard.o

getChar.o: getChar.cpp
	gcc -std=c++11 -c -ggdb getChar.cpp -o getChar.o

Player_Board.o: Player_Board.cpp Player_Board.hpp
	g++ $(CFLAGS) -g -c Player_Board.cpp -o Player_Board.o

Player_Board.x:getChar.o Player.o GameBoard.o Player_Board.o Player_Board_main.cpp
	g++ $(CFLAGS) -g getChar.o Player.o GameBoard.o Player_Board.o Player_Board_main.cpp -o Player_Board.x -lcurses -ltermcap
	rm -r *.dSYM


clean:
	rm -r *.x *.o *.dSYM
