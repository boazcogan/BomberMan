#include "Coordinate.cpp"
#include "GameBoard.hpp"
#include "Player.hpp"
#include <thread>
#include <queue>

using namespace std;

#ifndef PLAYER_BOARD_HPP
#define PLAYER_BOARD_HPP

// The Player_Board class is a manager that makes the Player class and the
// GameBoard class interact with eachother in a cohesive manner.
class Player_Board
{
    public:
        // The Player_Board constructer
        Player_Board(int size, int numPlayers, int startX, int startY);
        // Moves a player "who" in the direction "to"
        void Move(int who, char to, int startX, int startY);
        // Prints the Player Board internal structure for debugging purposes
        void PrintPlayerBoard();
        // Creates a thread to manage the bomb so that simultaneous commands can
        // be processed.
        thread * newBomb(int who, int startX, int startY);
        // Creates the bomb on the board.
        void createBomb(int who, int startX, int startY);
        // Explodes in a chosen direction recursively.
        void explode(coordinate startingPoint, char dir, int identifier, int startX, int startY);
        // begins a bomb countdown.
        void countdown(coordinate location, int startX, int startY);
        // Ends the explosion on the board.
        void endExplode(coordinate startingPoint, char dir, bool firstSquare, int startX, int startY);
        // Finds the total number of players still alive.
        int numPlayersAlive();
        // gets the number of active bombs that need to be deactivated.
        queue<int> * activeBombs()                                         {return refundBombs;}
        // remove a bomb from the queue, in this case it has been processed
        void popActiveBombs()                                              {refundBombs->pop();}
        // gets the last player still alive
        void getWinner();
        // Displays the game board with curses
        void displayGameBoard(int startX, int startY)                      {board->DisplayGameBoard(startX,startY);}
        // Display all of the players to the game board
        void Display_Players(int startX, int startY);
    private:
        // a queue to manage all of the bombs.
        queue<int> * refundBombs;
        // the actual GameBoard
        GameBoard * board;
        // access to each player.
        Player * players[4];
        // Total number of players
        int totalPlayers;

};

#endif
