#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "grid2048.h"
/*
 Les fonctions push ont pour but de déplacer les cases jusqu'à combler le/les zéro(s) rencontré(s).
 S'il ne reste que des zéros à traiter, la fonction renvoie 0. Le cas échéant, elle renvoie 1 et 
 après cette opération, nous sommes certains que la prochaine case est non-nulle.
*/
int pushNorth(Grid *g, size_t row, size_t col);
int pushSouth(Grid *g, size_t row, size_t col);
int pushWest(Grid *g, size_t row, size_t col);
int pushEast(Grid *g, size_t row, size_t col);
void moveNorth(Grid *g, size_t col);
void moveSouth(Grid *g, size_t col);
void moveWest(Grid *g, size_t row);
void moveEast(Grid *g, size_t row);
long* additionNorth(Grid *g, size_t col, long *new_score);
long* additionSouth(Grid *g, size_t col, long *new_score);
long* additionWest(Grid *g, size_t row, long *new_score);
long* additionEast(Grid *g, size_t row, long *new_score);

struct Grid_t
{
    size_t h;
    size_t w;
    unsigned **grid; 
};

Grid *gridCreate(size_t height, size_t width)
{
    Grid *g;
    size_t i, j;
    
    g = malloc(sizeof(Grid)); 
    if(!g)
        return NULL;
    
    g->grid = malloc(height * sizeof(unsigned*)); 
    
    if(!(g->grid))
    {
        free(g);
        return NULL;
    }
    
    for(i=0; i<height; i++)
    {
        g->grid[i] = malloc(width * sizeof(unsigned)); 
        if(!(g->grid[i]))
        {
            for(j=0; j<i; j++)
                free(g->grid[j]);
            free(g->grid);
            free(g);
            return NULL;
        }
        for(j=0; j<width; j++)
            g->grid[i][j] = 0;
    }
    g->h = height;
    g->w = width;
    
    return g;
}

void gridFree(Grid *g)
{
    for(size_t i=0; i<g->h; i++)
        free(g->grid[i]);
    free(g->grid);
    free(g);
}

size_t gridHeight(Grid *g)
{
    return g->h;
}

size_t gridWidth(Grid *g)
{
    return g->w;
}

unsigned gridGetTileValue(Grid *g, size_t row, size_t col)
{
    return g->grid[row][col];
}

void gridSetTileValue(Grid *g, size_t row, size_t col, unsigned val)
{
    g->grid[row][col] = val;
}

int random_tab[10] = {3, 2, 2, 2, 2, 2, 2, 2, 2, 2};
void gridAddRandomTile(Grid *g)
{
    int index = rand()%10; 
    unsigned new_tile = random_tab[index]; // pioche un nombre aléatoire entre les indices 0 et 9 de random_tab
    unsigned availability = 0; // compte le nombre de cases vides 
    for(size_t i=0; i<g->h; i++)
        for(size_t j=0; j<g->w; j++)
            if(g->grid[i][j] == 0)
                availability++;
    
    if(availability == 0) 
        return;      
    
    unsigned position = rand()%availability + 1;  // choisit le numéro (entre 1 et availability) de la case vide où sera attribuée new_tile
    unsigned counter = 0;
    unsigned quit = 0;
    for(size_t i=0; i<g->h; i++)
    {   
        for(size_t j=0; j<g->w; j++)
        {
            if(g->grid[i][j] == 0)
                counter++;
            if(counter == position)
            {
                g->grid[i][j] = new_tile;
                quit = 1;
                break;
            }
        }
        if(quit == 1) 
            break;
    }
}

int gridEndGame(Grid *g)
{
    /* vérifie que toutes les cases sont prises*/
    for(size_t i=0; i<g->h; i++)
        for(size_t j=0; j < g->w; j++)
            if(g->grid[i][j] == 0)
                return 0;
    
    /* recherche d'une égalité latérale*/
    for(size_t i=0; i<g->h; i++)
        for(size_t j=0; j <= g->w - 2; j++)
            if(g->grid[i][j] == g->grid[i][j+1])
                return 0;
    
    /* recherche d'une égalité verticale*/
    for(size_t j=0; j<g->w; j++)
        for(size_t i=0; i <= g->h - 2; i++)
            if(g->grid[i][j] == g->grid[i+1][j])
                return 0;
    
    return 1;
}

int pushNorth(Grid *g, size_t row, size_t col)
{
    size_t a = row;
    while((g->grid[a][col]==0) && a < g->h - 1)
        a++;
    
    if(g->grid[a][col] == 0)
        return 0;
       
    for(size_t p=0; a + p <= g->h - 1; p++)
    {    
        g->grid[row+p][col] = g->grid[a+p][col];
        g->grid[a+p][col] = 0;
    }
    return 1;
}

int pushSouth(Grid *g, size_t row, size_t col)
{
    row = (int)row;
    col = (int)col;

    int a = row;
    while((g->grid[a][col]==0) && a > 0)
        a--;
    
    if(g->grid[a][col] == 0)
        return 0;
       
    for(int p=0; a - p >= 0; p++) 
    {    
        g->grid[row-p][col] = g->grid[a-p][col];
        g->grid[a-p][col] = 0;
    }
    return 1;
}

int pushWest(Grid *g, size_t row, size_t col)
{
    size_t b = col;
    while((g->grid[row][b]==0) && b < g->w - 1)
        b++;
    
    if(g->grid[row][b] == 0)
        return 0;
       
    for(size_t p=0; b + p <= g->w - 1; p++)
    {    
        g->grid[row][col+p] = g->grid[row][b+p];
        g->grid[row][b+p] = 0;
    }
    return 1;
}

int pushEast(Grid *g, size_t row, size_t col)
{
    row = (int)row;
    col = (int)col;
    
    int b = col;
    while((g->grid[row][b]==0) && b > 0)
        b--;
    
    if(g->grid[row][b] == 0)
        return 0;
       
    for(int p=0; b - p >= 0; p++)
    {    
        g->grid[row][col-p] = g->grid[row][b-p];
        g->grid[row][b-p] = 0;
    }
    return 1;
}

void moveNorth(Grid *g, size_t col)
{
    for(size_t i=0; i <= (g->h) - 1; i++)
        if(g->grid[i][col] == 0)
            (pushNorth(g, i, col));     
}

void moveSouth(Grid *g, size_t col)
{
    col = (int)col;
    for(int i= (int)(g->h - 1); i >= 0; i--)
        if(g->grid[i][col] == 0)
            (pushSouth(g, i, col));      
}

void moveWest(Grid *g, size_t row)
{
   for(size_t j=0; j <= (g->w) - 1; j++)
        if(g->grid[row][j] == 0)
            (pushWest(g, row, j));     
}

void moveEast(Grid *g, size_t row)
{
   row = (int)row;
   for(int j=(int)(g->w - 1); j >= 0; j--)
        if(g->grid[row][j] == 0)
            (pushEast(g, row, j));     
}

long* additionNorth(Grid *g, size_t col, long *new_score) 
{
    size_t line = 0;
    while(line < g->h - 1)
        {
            if(g->grid[line][col] == g->grid[line+1][col])
            {
                g->grid[line][col] *= 2;
                g->grid[line+1][col] = 0;
                pushNorth(g, line+1, col);
                new_score[0] += g->grid[line][col];
            }
            line++;
        } 
    return new_score;
}

long* additionSouth(Grid *g, size_t col, long *new_score) 
{
    size_t line = g->h - 1;
    while(line > 0)
        {
            if(g->grid[line][col] == g->grid[line-1][col])
            {
                g->grid[line][col] *= 2;
                g->grid[line-1][col] = 0;
                pushSouth(g, line-1, col);
                new_score[0] += g->grid[line][col];
            }
            line--;
        } 
    return new_score;
}

long* additionWest(Grid *g, size_t row, long *new_score) 
{
    size_t column = 0;
    while(column < g->w - 1)
        {
            if(g->grid[row][column] == g->grid[row][column+1])
            {
                g->grid[row][column] *= 2;
                g->grid[row][column+1] = 0;
                pushWest(g, row, column+1);
                new_score[0] += g->grid[row][column];
            }
            column++;
        } 
    return new_score;
}

long* additionEast(Grid *g, size_t row, long *new_score) 
{
    size_t column = g->w - 1;
    while(column > 0)
        {
            if(g->grid[row][column] == g->grid[row][column-1])
            {
                g->grid[row][column] *= 2;
                g->grid[row][column-1] = 0;
                pushEast(g, row, column-1);
                new_score[0] += g->grid[row][column];
            }
            column--;
        } 
    return new_score;
}

// Complexity gridPlayMove: O(n^3)
int gridPlayMove(Grid *g, Move direction, long *score)
{
    unsigned pre_tab[g->h][g->w]; 
    for (size_t row=0; row<g->h; row++)
        for(size_t col=0; col<g->w; col++)
            pre_tab[row][col] = g->grid[row][col];
    
    switch(direction)
    {        
        case North: 
            for (size_t j=0; j<g->w; j++)
            {          
                moveNorth(g, j);
                additionNorth(g, j, score);
            }
        break;
        
        case South: 
            for (size_t j=0; j<g->w; j++)
            {          
                moveSouth(g, j);
                additionSouth(g, j, score);
            }
        break;

        case West:
            for (size_t i=0; i<g->h; i++)
            {          
                moveWest(g, i);
                additionWest(g, i, score);
            }
        break;

        case East:
            for (size_t i=0; i<g->h; i++)
            {          
                moveEast(g, i);
                additionEast(g, i, score);
            }
        break;
        
        default:
            return 0; 
    }
    
    int moves = 0;  // permet d'affirmer qu'il y a eu un déplacement de cases ou non.
    for (size_t row=0; row<g->h; row++)
        for(size_t col=0; col<g->w; col++)
            if(pre_tab[row][col] != g->grid[row][col])
                moves = 1;
    if(moves == 0)
        return 0; 
    
    return 1;
}