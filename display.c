#include "display.h"
#include "grid2048.h"

#include <ncurses.h>
#include <math.h>
#include <stdbool.h>

size_t counttiles(Grid *g);

void displayInit(size_t height, size_t width)
{

    initscr();
    noecho();             // prevents getch() to echo the input to the screen
    keypad(stdscr, TRUE); // allow keypad to generate KEY_xxx codes
    start_color();

    size_t display_height = 2 * height + 1,
           display_width = 5 * width + 1;
    WINDOW *box = subwin(stdscr, display_height, display_width, 0, 0);
    wborder(box, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, '+', '+', '+', '+');
    wrefresh(box);

    // Draw inner lines
    for (size_t i = 1; i < height; i++)
    {
        mvaddch(2 * i, 0, '+');
        mvhline(2 * i, 1, ACS_HLINE, display_width - 2);
        mvaddch(2 * i, display_width - 1, '+');
    }

    // Draw inner columns
    for (size_t j = 1; j < width; j++)
    {
        for (size_t i = 0; i < height; i++)
        {
            mvaddch(2 * i, 5 * j, '+');
            mvaddch(2 * i + 1, 5 * j, ACS_VLINE);
        }
        mvaddch(display_height - 1, 5 * j, '+');
    }
    mvprintw(2 * height + 2, 0, "Use the arrows to play, press 'q' to quit");

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_RED, COLOR_BLACK);
    init_pair(7, COLOR_BLUE, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_YELLOW);
    init_pair(9, COLOR_GREEN, COLOR_YELLOW);
    init_pair(10, COLOR_BLUE, COLOR_YELLOW);
    init_pair(11, COLOR_MAGENTA, COLOR_YELLOW);
    init_pair(12, COLOR_RED, COLOR_YELLOW);
    init_pair(13, COLOR_CYAN, COLOR_YELLOW); // 8192

    move(0, 0);
}

void displayUpdate(Grid *g, long score)
{
    size_t height = gridHeight(g), width = gridWidth(g);

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            unsigned v = gridGetTileValue(g, i, j);
            attron(COLOR_PAIR((int)log2((double)v)));
            if (v)
                mvprintw(2 * i + 1, 5 * j + 1, "%4u", gridGetTileValue(g, i, j));
            else
            {
                attron(COLOR_PAIR(1));
                mvprintw(2 * i + 1, 5 * j + 1, "    ");
            }
        }
    }

    attron(COLOR_PAIR(1));
    mvprintw(2 * height + 3, 0, "Score: %lu", score);

    move(0, 0);
    refresh();
}

size_t counttiles(Grid *g)
{
    size_t c = 0;
    for (size_t i = 0; i < gridHeight(g); i++)
    {
        for (size_t j = 0; j < gridWidth(g); j++)
        {
            if (gridGetTileValue(g, i, j) != 0)
                c++;
        }
    }
    return c;
}
long displayEventLoop(size_t height, size_t width)
{
    Grid *g = gridCreate(height, width);

    gridAddRandomTile(g);
    gridAddRandomTile(g);

    long score = 0;
    displayUpdate(g, score);

    while (!gridEndGame(g))
    {
        int c = getch();
        Move m;
        bool valid = true;
        switch (c)
        {
        case KEY_UP:
            m = North;
            break;
        case KEY_DOWN:
            m = South;
            break;
        case KEY_RIGHT:
            m = East;
            break;
        case KEY_LEFT:
            m = West;
            break;
        default:
            valid = false;
            break;
        }

        if (valid)
        {
            if (gridPlayMove(g, m, &score))
                gridAddRandomTile(g);
            displayUpdate(g, score);
        }
        else
        {
            if (c == 'q')
                break;
        }
    }

    gridFree(g);
    return score;
}

void displayFree()
{
    endwin();
}
