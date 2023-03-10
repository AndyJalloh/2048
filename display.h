#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "grid2048.h"

/**
 * Initializes the display of the grid on the screen
 * 
 * PARAMETERS 
 * height           the grid's height
 * width            the grid's width 
 */
void displayInit(size_t height, size_t width);

/**
 * Updates the display of the grid on the screen 
 * 
 * PARAMETERS
 * g                A pointer to a grid
 * score            The current score to display
 */
void displayUpdate(Grid *g, long score);

/**
 * Runs a game until the end, getting user input and handling it
 * Returns the score at the end of the game, or a negative error code
 * 
 * PARAMETERS 
 * height           the grid's height
 * width            the grid's width 
 */
long displayEventLoop(size_t height, size_t width);

/**
 * Frees the display 
 */
void displayFree(void);

#endif
