This iteration of the game BomberMan features two characters, "1" and "2", that
navigate a maze dropping bombs with the goal of destroying the other character.
Player "1" navigates using keys: wasd and drops bombs using the key r.
Player "2" navigates using keys: hjkl and drops bombs using the key ;.
The game can be ended at any point by selecting q.
Walls and players within the maze are destructible. The number of players is
limited by the number of keys that a keyboard has. The game can support up to
four different players each starting in their separate corners. The screen is
displayed using a curses window which closes once the game ends.

Working Features:
Maze with destructible walls.
bombs.
countdown on dropped bombs.
game properly exits upon player win.
Supports two players.
Navigation

upcoming features:
A displayed countdown on the bombs.
Player characters represented by a "P" with color representing different players
Ability to Play with other Players over a network

Files:
GameBoard.*: A datatype designed to track data about where all objects are located
decide whether or not moves are allowed. This is the primary structure of the game.

Player.*: Stores information about the player such as: are they alive, can they 
move, where are they located, and how many bombs have they dropped.

Player_Board.*: The interface that the user primarily interacts with. This file serves
as a manager for the Board and Player allowing them to work together. Bombs only exist
on this level of the game structure.

Coordinate.cpp: A struct to store a coordinate location.

Instructions for run:
Type the following commands:
make
./Player_Board.x
