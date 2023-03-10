#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "grid2048.h"
#include "display.h"

int main()
{
    size_t w = 4, h = 4;
    long score = 0;
    
    displayInit(h, w);
    score = displayEventLoop(h, w);
    displayFree();

    printf("You lost (or gave up) ! Final score: %lu\n", score);

    return 0;
}
