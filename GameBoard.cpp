#include "GameBoard.hpp"
#include "Coordinate.cpp"
#include <iostream>
#include <curses.h>

using namespace std;

GameBoard::GameBoard(int size)
{
    // Save the size of the GameBoard
    if ((size % 2) == 0)
    {
      size += 1;
    }
    gameSize = size;
    // Create the GameBoard with size rows and columns, a GameBoards size
    // is always odd.
    board = new int*[size];
    // The GameBoard is 2-dimensional so a loop is required to populate the
    // initial array with arrays
    for (int i=0; i<size; i++)
    {
       board[i] = new int[size];
    }
    // Create the outer walls of the GameBoard. These are represented by -1
    for(int i = 0; i<size; i++)
    {
        board[0][i] = -1;
        board[size-1][i] = -1;
        board[i][0] = -1;
        board[i][size-1] = -1;
    }
    // Fill the entire array with destructable walls, these are represented by
    // the number 5.
    for (int i = 1; i<size-1;i++)
    {
        for (int j = 1; j<size-1;j++)
        {
            board[i][j] = 5;
        }
    }
    // Carve out a path for the Players to move through
    Depth_First(1,1,size,size);

}


void GameBoard::Depth_First(int r, int c, int numCols, int numRows)
{
    //This function will: carve a maze based on the depth first method.
    // set where we currently are to a movable square
    board[r][c] = 0;
    // choose a random direction
    int rd = rand()%4;
    for (int d=0; d<4;d++)
    {
        // If the random direction was right and there is a wall two squares to
        // the right, then the square next to the current location can become a
        // movable location
        if (rd == 0 && c+2<numCols &&board[r][c+2] != 0)
        {
            board[r][c+1] = 0;
            Depth_First(r, c+2, numCols, numRows);
        }
        // If the previous check failed then either the random direction was
        // initially right or the space two items to the right was already free,
        // in this case we just move on to the next direction and repeat the
        // process for up,
        else if (rd == 1 && r+2<numRows &&board[r+2][c] != 0)
        {
            board[r+1][c] = 0;
            Depth_First(r+2, c, numCols, numRows);
        }
        // then left,
        else if (rd == 2 && c>2 &&board[r][c-2] != 0)
        {
            board[r][c-1] = 0;
            Depth_First(r, c-2, numCols, numRows);
        }
        // and finally right
        else if (rd == 3 && r>2 && board[r-2][c] != 0)
        {
            board[r-1][c] = 0;
            Depth_First(r-2, c, numCols, numRows);
        }
        // At this point either we have made a modification or we have already
        // chosen a direction so we choose a new direction. Note that after 4
        // loops all of the directions will have been chosen once.
        rd=(rd+1)%4;
    }
}


int GameBoard::getSquareVal(coordinate * location)
{
    // Access the value at the passed location
    return board[location->x][location->y];
}


void GameBoard::PrintBoard()
{
    // This function is responsible for printing the board for debugging
    // purposes
    for (int i = 0; i<gameSize;i++)
    {
        for (int j = 0; j<gameSize;j++)
        {
            if (board[i][j] > -1)
            {
                cout<<" "<<board[i][j];
            }
            else
            {
                cout<<board[i][j];
            }
        }
    cout<<"\n";
    }
}


void GameBoard::DisplayGameBoard(int startCoordinateX, int startCoordinateY)
{
    //Displays the border of the game.
    for ( int i = 0; i < gameSize; i++)
    {
        for (int j = 0; j < gameSize; j++)
        {
            if (board[i][j] == -1 || board[i][j] == 5)
            {
                mvaddch(i+startCoordinateX, (j*2)+startCoordinateY,'*');
            }
        }
    }
}


bool GameBoard::Move(coordinate * from, coordinate * to, int startX, int startY, char who)
{
    // If the square is occupied by anything other than an explosion or a empty
    // square then the move was unsuccessful
    if (board[to->x][to->y] != 0 && board[to->x][to->y] != -3)
    {
        return false;
    }
    // If the board was occupied by an explosion return true and the square that
    // the player occupied should be set to empty and the move was successful
    else if (board[to->x][to->y] == -3)
    {
        board[from->x][from->y] = 0;
        mvaddch(from->x+startX,(from->y*2)+startY,' ');
        return true;
    }
    // Otherwise the board at the desired square is empty. In this case the
    // Player should have the new square set to their character and the old
    // square should be set to empty.
    board[to->x][to->y] = -2;
    board[from->x][from->y] = 0;
    mvaddch(from->x+startX,(from->y*2)+startY,' ');
    mvaddch(to->x+startX,(to->y*2)+startY, who);

    return true;
}


void GameBoard::SetPlayer(coordinate * location, int startX, int startY, char who)
{
    // Set the Board at the given location to a player.
    board[location->x][location->y] = -2;
}


void GameBoard::removePlayer(coordinate * location)
{
    // Set the players current location to empty
    board[location->x][location->y] = 0;
}


bool GameBoard::ExplodeSquare(coordinate location, int status_marker, int startX, int startY)
{
    // If the current square is a player or a wall then the square should be
    // exploded and the function should return false.
    if (board[location.x][location.y] == -2 || board[location.x][location.y] == 5)
    {
        // a Player has died or a wall has been encountered. The explode
        // should be signalled to end.
        mvaddch(location.x+startX,(location.y*2)+startY, '+');
        board[location.x][location.y] = status_marker;
        return false;
    }
    // If the square is anything other than either of the above cases or empty,
    // then the square should not be exploded as it is probably one of the outer
    // bounds.
    else if(board[location.x][location.y] < 0 && board[location.x][location.y] != -3)
    {
        return false;
    }
    // draw the character at the current square to represent an explosion.
    mvaddch(location.x+startX,(location.y*2)+startY, '+');
    // set the boards status to an exploded square.
    board[location.x][location.y] = status_marker;
    // return true to signal the board to explode the next square.
    return true;
}


bool GameBoard::EndExplodeSquare(coordinate location, bool firstSquare, int startX, int startY)
{
    // If we are not end exploding the very first square (to catch the other end
    // explode calls), and there is anything in the current square, then return
    // false.
    if(board[location.x][location.y] != -3 && !firstSquare )
    {
        return false;
    }
    // If the current square is a previously exploded square then set the value
    // to an empty square, then return true.
    mvaddch(location.x+startX,(location.y*2)+startY, ' ');
    board[location.x][location.y] = 0;
    return true;
}


void GameBoard::SetVal(coordinate * location, int value)
{
    // Set the value at the passed location to the passed value
    board[location->x][location->y] = value;
}
void GameBoard::SetVal(coordinate location, int value)
{
    // Set the value at the given location to the passed value.
    board[location.x][location.y] = value;
}
