#include "Coordinate.cpp"
#include <string>

using namespace std;

#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player
{
    public:
        // Create a Player (default constructor)
        Player(coordinate * xy, string name);
        // Set a Player's Status to 0 to prevent future interaction
        void Die()                                      {status = 0;}
        // Move a Player
        void Move(coordinate * direction);
        // Adjust the number of bombs a Player has
        bool DropBomb();
        // Adjust the number of bombs a Player has
        void refundBomb();
        // Check whether the player is alive
        int getStatus()                                 {return status;}
        // Get the Player's location
        coordinate * getLocation()                      {return location;}
        // Check the number of bombs the Player has left.
        int numBombsLeft()                              {return numBombs;}
    private:
        // Store a Players Location
        coordinate * location;
        // Store the number of bombs a player has left as well as their status
        // and the total number of bombs dropped
        int status, numBombs, totalBombs;
        // store the Players name
        string name;

};

#endif
