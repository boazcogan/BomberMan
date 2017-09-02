#include "Coordinate.cpp"
#include "GameBoard.hpp"
#include "Player.hpp"
#include "Player_Board.hpp"
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <queue>
#include <curses.h>

using namespace std;


Player_Board::Player_Board(int size, int numPlayers, int startX, int startY)
{
    // Instantiate the private variables.
    refundBombs = new queue<int>;
    totalPlayers = numPlayers;
    board = new GameBoard(size);
    coordinate * PLocation = new coordinate();
    string Player_name;
    char who;
    // For each of the players instantiate them at a different location
    for (int i = 0; i < numPlayers; i++)
    {
        if (i==0)
        {
            // each if statement makes a different modification based on the
            // current value of i.
            who = '0';
            Player_name = "Player_1";
            PLocation->x = 1; PLocation->y = 1;
        }
        else if (i==1)
        {
            who = '1';
            Player_name = "Player_2";
            PLocation->x = size-2; PLocation->y = size-2;
        }
        else if (i==2)
        {
            who = '2';
            Player_name = "Player_3";
            PLocation->x = 1; PLocation->y = size-2;
        }
        else if (i==3)
        {
            who = '3';
            Player_name = "Player_4";
            PLocation->x = size-2; PLocation->y = 1;
        }
        // Then the players are added to the array
        players[i] = new Player(PLocation, Player_name);
        board->SetPlayer(PLocation, startX, startY, who);
    }

}


void Player_Board::Display_Players(int startX, int startY)
{
    // Displays all of the players at their current location to the screen.
    coordinate * location = new coordinate();
    for (int i = 0; i < totalPlayers; i++)
    {
        location = players[i]->getLocation();
        string player_id = to_string(i);
        char player_id_ch = player_id[0];
        mvaddch(location->x+startX, (location->y*2)+startY, player_id_ch);
    }
}


void Player_Board::PrintPlayerBoard()
{
    // Prints the Board for debugging purposes
    board->PrintBoard();
    for (int i = 0; i<totalPlayers;i++)
    {
        // The players must also be printed as well as their locations.
        coordinate * location = players[i]->getLocation();
        int status = players[i]->getStatus();
        if (status == 1)
        {
            cout<<"Player"<<i<<" location is:" <<location->x<<","<<location->y<<"\n";
        }
        else
        {
            cout<<"Player"<<i<<" is dead\n";
        }
    }
}


void Player_Board::Move(int who, char to, int startX, int startY)
{
    // Moves a player in the desired direction
    // If a player is dead then they cannot Move
    if (players[who]->getStatus() == 0)
    {
        return;
    }
    // keep track of where we were and create a marker for where we are going.
    coordinate * from = players[who]->getLocation();
    coordinate * destination = new coordinate();
    // Increment or decrement where we were to reflect where we are going and
    // set the destination to that value.
    if (to == 'u')
    {
        destination -> x = from -> x - 1;
        destination -> y = from -> y;
    }
    else if (to == 'd')
    {
        destination -> x = from -> x + 1;
        destination -> y = from -> y;
    }
    else if (to == 'l')
    {
        destination -> x = from -> x;
        destination -> y = from -> y - 1;
    }
    else if (to == 'r')
    {
        destination -> x = from -> x;
        destination -> y = from -> y + 1;
    }
    // Convert who is moving to a string so that it can be printed to the board
    string player_id = to_string(who);
    char player_id_ch = player_id[0];
    // If the player can be moved on the board then it is a legal target, so the
    // player can now be moved in the game.
    if (board->Move(from, destination, startX, startY, player_id_ch))
    {
        if (board->getSquareVal(destination) == -3)
        {
            // If we are trying to move into an exploded square then the player
            // must die.
            players[who]->Die();
        }
        // reflect the move on the Player itself.
        players[who] -> Move(destination);
    }
}


void Player_Board::createBomb(int who, int startX, int startY)
{
    // This function manages the process of a bombs existence.
    coordinate where;
    // A bomb is generated on the player who dropped it.
    where.x = players[who]->getLocation()->x;
    where.y = players[who]->getLocation()->y;
    // If the player has no bombs left to drop then end the process.
    if (!players[who]->DropBomb())
    {
        return;
    }
    if (players[who]->getStatus() == 0)
    {
        return;
    }
    // display a bomb at the desired location.
    mvaddch(where.x+startX, (where.y*2)+startY, '@');
    // the bomb should explode after the desired amount of time.
    countdown(where, startX, startY);
    // explode in every direction
    explode(where, 'u', -3, startX, startY);
    explode(where, 'd', -3, startX, startY);
    explode(where, 'l', -3, startX, startY);
    explode(where, 'r', -3, startX, startY);

    // wait for 1.5 seconds before end-exploding
    unsigned int microseconds = 1000000;
    usleep(microseconds);

    bool firstSquare = true;
    // Set all of the exploded squares to empty.
    endExplode(where, 'u', firstSquare, startX, startY);
    endExplode(where, 'd', firstSquare, startX, startY);
    endExplode(where, 'l', firstSquare, startX, startY);
    endExplode(where, 'r', firstSquare, startX, startY);

    // give the player their bomb back.
    players[who]->refundBomb();
    // Note that the thread is completed
    refundBombs->push(1);
}


void Player_Board::explode(coordinate startingPoint, char dir, int square_val, int startX, int startY)
{
    // Recursively destroys the current square stated in starting point. Returns
    // false once all valid squares in the given direction are destroyed.
    // Implemented recursively to practice recursion.
    for (int i =0; i<totalPlayers; i++)
    {
        // If any players are in the square we are trying to explode then kill
        // the player.
        coordinate * playerLocation = players[i]->getLocation();
        if (playerLocation->x == startingPoint.x && playerLocation->y == startingPoint.y)
        {
            players[i]->Die();
        }
    }
    // Base Case: If the board can not destroy the current square then the
    // function ends. Note that regardless of whether or not the destroy was
    // successful the function was called and a modification is made to the
    // GameBoard. Since this is the case the rest of the function operates under
    // the "assumption" that the board has been modified accordingly.
    if (!board->ExplodeSquare(startingPoint, square_val, startX, startY))
    {
        return;
    }
    // If the direction is [up:u, down:d, left:l, right:r] , then modify [x,y]
    // appropriately and begin the recursive call for the next square.
    if (dir == 'u')
    {
        startingPoint.x--;
        return explode(startingPoint, dir, square_val, startX, startY);
    }
    else if (dir == 'l')
    {
        startingPoint.y--;
        return explode(startingPoint, dir, square_val, startX, startY);
    }
    else if (dir == 'd')
    {
        startingPoint.x++;
        return explode(startingPoint, dir, square_val, startX, startY);
    }
    else if (dir == 'r')
    {
        startingPoint.y++;
        return explode(startingPoint, dir, square_val, startX, startY);
    }
}


void Player_Board::endExplode(coordinate startingPoint, char dir, bool firstSquare, int startX, int startY)
{
    // This function is identicle to explode only the EndExplodeSquare function
    // is called to end the explosion. All of the logic is the same.
    if (!board->EndExplodeSquare(startingPoint, firstSquare, startX, startY))
    {
        return;
    }
    firstSquare = false;
    if (dir == 'u')
    {
        startingPoint.x--;
        return endExplode(startingPoint, dir, firstSquare, startX, startY);
    }
    else if (dir == 'l')
    {
        startingPoint.y--;
        return endExplode(startingPoint, dir, firstSquare, startX, startY);
    }
    else if (dir == 'd')
    {
        startingPoint.x++;
        return endExplode(startingPoint, dir, firstSquare, startX, startY);
    }
    else if (dir == 'r')
    {
        startingPoint.y++;
        return endExplode(startingPoint, dir, firstSquare, startX, startY);
    }
}


void Player_Board::countdown(coordinate startingPoint, int startX, int startY)
{
    // Do not explode the board for 3 second after the bomb is dropped, begin a
    // countdown to reflect this change. Currently, since players are not
    // identified with icons other than numbers the bomb is represented by the
    // @ symbol. Additional changes for the future include changing players to
    // a 'P' and having them be represented by colors. Then the countdown can be
    // represented by a numerical value.
    unsigned int microseconds = 200000;
    for (int i = 0; i < 15; i++)
    {
        // if we have waited zero seconds then we have 3 seconds to go. In this,
        // location we can write a '3' instead of an '@'. This change has not
        // yet been made due to conflict with character icons.
        if ( i <= 5 )
        {
            //Check to see if there is another bomb blew up this one.
            usleep(microseconds);
            if (board->getSquareVal(startingPoint) == -3)
            {
                board->SetVal(startingPoint, 0);
                return;
            }
            mvaddch(startingPoint.x+startX, (startingPoint.y*2)+startY, '@');
	          board->SetVal(startingPoint, 3);
        }
        else if ( i <= 10 )
        {
            usleep(microseconds);
            if (board->getSquareVal(startingPoint) == -3)
            {
                board->SetVal(startingPoint, 0);
                return;
            }
            mvaddch(startingPoint.x+startX, (startingPoint.y*2)+startY, '@');
	          board->SetVal(startingPoint, 2);
        }
        else if ( i <= 15 )
        {
            usleep(microseconds);
            if (board->getSquareVal(startingPoint) == -3)
            {
                board->SetVal(startingPoint, 0);
                return;
            }
            mvaddch(startingPoint.x+startX, (startingPoint.y*2)+startY, '@');
	          board->SetVal(startingPoint, 1);
        }
    }
    board->SetVal(startingPoint, 0);

}


thread * Player_Board::newBomb(int who, int startX, int startY)
{
    // The threaded function that represents a bomb.
    thread * bombItem = new thread(&Player_Board::createBomb,*this,who, startX, startY);
    return bombItem;
}


int Player_Board::numPlayersAlive()
{
    // find out how many players are still alive
    int totalPlayersLeft = 0;
    for (int i = 0; i < totalPlayers; i++)
    {
        if (players[i]->getStatus() == 1)
        {
            totalPlayersLeft++;
        }
    }
    return totalPlayersLeft;

}


void Player_Board::getWinner()
{
    // find the player whose status is alive, that player wins.
    for (int i = 0; i < totalPlayers; i++)
    {
        if (players[i]->getStatus() == 1)
        {
            cout<<"\n\nPlayer "<<(i+1)<<" wins!!!\n\n";
        }
    }
}
