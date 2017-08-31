#include "Player_Board.hpp"
#include <iostream>
#include <thread>
#include "getChar.hpp"
#include <curses.h>
#include <sstream>
using namespace std;

void startup( void );

// This is my first iteration of the game bomberman. The game uses curses to
// create a visual representation for a series of classes that work together.
// Currently only two players are supported for the game. To support more
// players more keys would be needed.
int main()
{
    // Set the start Point for the game box
    int startX = 1; int startY = 1;
    // Create a new instance of the game
    Player_Board * board = new Player_Board(23, 3, startX, startY);
    // currently, only one character can move at once.
    char move = ' ';
    // create a queue to manage the bombs, their threads, and see which ones are
    // currently active
    queue<thread *> bombQueue;
    queue<int> * inactiveBombs = board->activeBombs();

    // Start the use of curses
    startup();
    // Add the title of the game to the screen.
    mvaddstr(0,3,"BomberMan");
    // Create the display of the GameBoard.
    board->displayGameBoard(startX,startY);
    // Draw the Players onto the board
    board->Display_Players(startX,startY);
    refresh();

    // This function is the body of the game. All interactions the players make
    // with the board a tracked in this loop. The loop will continue while more
    // than one player is alive.
    while ((board->numPlayersAlive())>1)
    {
        // Display all changes
        refresh();
        // get a character input
        move = get_char();
        // see which bombs are currently active.
        inactiveBombs = board->activeBombs();
        // If any bombs are inactive then they must be finished so...
        while ( ! inactiveBombs->empty() )
        {
            // the item in the front of the thread queue needs to be accessed
            thread * inactiveThread = bombQueue.front();
            // the bombqueue should be modified
            bombQueue.pop();
            // and the boards bombqueue needs to be modified
            board->popActiveBombs();
            // the thread must be joined back into the function
            inactiveThread->join();
        }
        // Interpret the different characters read for the move
        switch (move)
        {
            case 'w':
            {// Move p1 up
                board->Move(0, 'u',startX, startY);
                break;
            }
            case 'a':
            {// Move p1 left
                board->Move(0, 'l', startX, startY);
                break;
            }
            case 's':
            {// Move p1 down
                board->Move(0, 'd', startX, startY);
                break;
            }
            case 'd':
            {// Move p1 right
                board->Move(0, 'r', startX, startY);
                break;
            }
            case 'r':
            {// P1 dropped a bomb
                thread * thing = board->newBomb(0, startX, startY);
                // keep track of the thread by putting it in a queue
                bombQueue.push(thing);
                break;
            }
            case 'k':
            {// Move p2 up
                board->Move(1, 'u', startX, startY);
                break;
            }
            case 'h':
            {// Move p2 left
                board->Move(1, 'l', startX, startY);
                break;
            }
            case 'j':
            {// Move p2 down
                board->Move(1, 'd', startX, startY);
                break;
            }
            case 'l':
            {// Move p2 right
                board->Move(1, 'r', startX, startY);
                break;
            }
            case ';':
            {// P2 dropped a bomb
                // keep track of the thread by putting it into a queue
                thread * thing2 = board->newBomb(1, startX, startY);
                bombQueue.push(thing2);
                break;
            }
            case 'q':
            {
                // quit the game
                mvcur( 0, COLS - 1, LINES - 1, 0 );
                clear();
                refresh();
                endwin();
                exit(1);
            }
            default:
            {
                // on a unrecognized key do nothing
                break;
            }
        }
    }
    // The game has ended, the curses screen should be removed to allow user
    // input.
    mvcur( 0, COLS - 1, LINES - 1, 0 );
    clear();
    refresh();
    endwin();
    // Display the winner to the console.
    board->getWinner();

    return 0;
}


void startup( void )
{
     initscr();  /* activate the curses */
     curs_set(0);/* do not display the cursor */
     clear();    /* clear the screen that curses provides */
     noecho();   /* prevent the input chars to be echoed to the screen */
     cbreak();   /* change the stty so that characters are delivered to the
                    program as they are typed--no need to hit the return key!*/
}
