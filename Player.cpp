#include "Player.hpp"
#include "Coordinate.cpp"
#include <string>
#include <iostream>

using namespace std;


Player::Player(coordinate * xy, string playerName)
{
	// Default constructor for a Player. A Player's primary roll is to store
	// and correlate information about a users character.
	location = new coordinate();
	location->x = xy->x;
	location->y = xy->y;
	status = 1;
	numBombs = 2;
	totalBombs = 0;
	name = playerName;
}

void Player::Move(coordinate * destination)
{
// moves a player in the given direction based off of their xy coordinates
    location = destination;
}

bool Player::DropBomb()
{
  // DropBomb() determines whether or not a Player is capable of dropping a bomb
	// and returns true if they can and false if they cant. A Player can only drop
	// a number of bombs equal to the private variable numBombs. This function
	// also increments the total number of bombs dropped for post game statistics.
	if (numBombs > 0)
	{
		numBombs--;
		totalBombs++;
		return true;
	}
	return false;

}

void Player::refundBomb()
{
	// This function increments the number of total Bombs a player can drop.
	numBombs++;
}
