#include "Coordinate.cpp"
#include "Square.cpp"

#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

// This class represents the board that the players will exist on. It is a 2D
// array with specialized functions.
class GameBoard
{
    public:
        // Default constructor
        GameBoard(int size);
        // Prints the boards values for debugging purposes
        void PrintBoard();
        // Determines whether or not an item can be moved to the desired
        // location and then moves the item. If the item cannot be moved it
        // returns false. Otherwise it returns true.
        bool Move(coordinate * from, coordinate * to, int startX, int startY, char who);
        // Sets a Player to the desired location on the board. This function is
        // used for the games initialization.
        void SetPlayer(coordinate * location, int startX, int startY, char who);
        // Removes a player from the game board. This function is used only when
        // a player has died.
        void removePlayer(coordinate * location);
        // Sets the value on the board at the passed location to the passed
        // value
        void SetVal(coordinate * location, int value);
        // Sets the value on the board at the passed location to the passed
        // value
        void SetVal(coordinate location, int value);
        // Sets the board at the given square to an exploded square. If the
        // square is not occupied then it returns true, if it was then the
        // return value is false.
        bool ExplodeSquare(coordinate location, int statusMarker, int startX, int startY);
        // Sets the value of the given location to a empty square, if the value
        // is already occupied by anything other than an explosion it returns
        // false. Otherwise it returns true.
        bool EndExplodeSquare(coordinate location, bool firstSquare, int startX, int startY);
        // Gets the value stored at the given location
        int getSquareVal(coordinate * location);
        int getSquareVal(coordinate location);
        // Generates the maze that the players will play on
        void Depth_First(int r, int c, int NumCols, int NumRows);
        // Displays the Board.
        void DisplayGameBoard(int startCoordinateX, int startCoordinateY);
    private:
        Square ** board;
        int gameSize;

};

#endif
