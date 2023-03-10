#ifndef _GRID2048_H_
#define _GRID2048_H_

#include <stdlib.h>
#include <stdio.h>

/* 2048 grid (opaque) structure */
typedef struct Grid_t Grid;

typedef enum Move_t {
    North = 0,
    East  = 1,
    South = 2,
    West  = 3
} Move;

/**
 * Creates an empty 2048 grid from its dimensions (standard game: 4x4)
 * Returns a pointer to the created grid. The grid should be cleaned
 * with gridFree after usage.
 * 
 * PARAMETERS
 * height           the grid's height
 * width            the grid's width 
 */
Grid *gridCreate(size_t height, size_t width);

/**
 * Frees a 2048 grid
 * 
 * PARAMETERS
 * g                A pointer to a grid
 */
void gridFree(Grid *g);

/**
 * Returns the height of a grid 
 * 
 * PARAMETERS
 * g                A pointer to a grid
 */
size_t gridHeight(Grid *g);

/**
 * Returns the width of a grid 
 * 
 * PARAMETERS
 * g                A pointer to a grid
 */
size_t gridWidth(Grid *g);

/**
 * Returns the content of the tile, or 0 if empty or the grid is NULL.
 * row=0 and col=0 corresponds to the top left tile.
 * 
 * PARAMETERS
 * g                A pointer to a grid
 * row              The row of the location to check
 * col              The column of the location to check
 */
unsigned gridGetTileValue(Grid *g, size_t row, size_t col);

/** 
 * Sets the value of a tile to an arbitratry value (e.g., 3).
 * row=0 and col=0 corresponds to the top left tile.
 * 
 * PARAMETERS
 * g                A pointer to a grid
 * row              The row of the location to check
 * col              The column of the location to check
 * val              The value to set the tile at
 */
void gridSetTileValue(Grid *g, size_t row, size_t col, unsigned val);

/**
 * Returns 1 if the game is over (there are no free cells left and no
 * possible merges), 0 otherwise.
 * 
 * PARAMETERS
 * g                A pointer to a grid
 */
int gridEndGame(Grid *g);

/**
 * Plays a move on the given grid, and updates the score
 * Returns 1 if some moves has been performed, 0 otherwise.
 * 
 * 
 * PARAMETERS
 * g                A pointer to a grid 
 * m                The move to be played 
 * score            A pointer to the score
 */
int gridPlayMove(Grid *g, Move m, long *score);

/**
 * Adds a random tile to the grid 
 *
 * Spawning rules:
 * - 10% chance to get a '4' tile, otherwise a '2'
 * - the location is chosen uniformly between all the options
 * - Add nothing if the grid is full
 * 
 * PARAMETERS
 * g                A pointer to a grid 
 */
void gridAddRandomTile(Grid *g);

#endif // !_GRID2048_H_
