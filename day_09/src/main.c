#define AOC_IN_FILE "data.in"
#define AOC_OUT_FILE "data.out"
#define AOC_OUT_FORMAT "%d\n"

static int result = 0;

#include "aocapp.h"

/* ===== AoC INCLUDES ===== */

#include <math.h>

typedef short pos_t;
typedef unsigned long coord_t;

static coord_t *coords;
static size_t nbCoords=0;

static pos_t hx, hy, tx, ty;

/**
 * From 2 positions to a coord
 * @param  xPos The x position
 * @param  yPos The y position
 * @return      The coordinate
 */
coord_t to_coord(pos_t xPos, pos_t yPos)
{
    return (coord_t)(*(unsigned short *)&xPos) | ( (coord_t)(*(unsigned short *)&yPos) << 16);
}

/**
 * Add a coordinate in the coords array
 * @param  c The coordinate of the tail
 * @return   1 if success, else 0
 */
int add_coord(pos_t x, pos_t y)
{
    coord_t c = to_coord(x,y);
    // Check for duplicate
    for (int i = 0; i < nbCoords; ++i) {
        if(coords[i] == c)
            return 1;
    }

    // Allocate memory for new coord
    coord_t *newAddr = (coord_t *)realloc(coords, sizeof *coords * (nbCoords+1));
    if(!newAddr)
        return 0;
    coords = newAddr;
    nbCoords++;

    // Add item
    coords[nbCoords-1] = c;

    return 1;
}

/**
 * Check if 2 points touch (distance in x,y not > 1)
 * @param  p1 The first point
 * @param  p2 The second point
 * @return    1 if touch, else 0
 */
int touch(pos_t x1, pos_t y1, pos_t x2, pos_t y2)
{
    if(abs(x1 - x2) > 1)
        return 0;
    if(abs(y1 - y2) > 1)
        return 0;
    return 1;
}

void move(int xoff, int yoff)
{
    int xstep = xoff > 0 ? 1 : -1;
    int ystep = yoff > 0 ? 1 : -1;

    for (int i = 0; abs(i) < abs(xoff); i += xstep) {
        hx += xstep;
        if(!touch(hx,hy,tx,ty)){
            tx = hx - xstep;
            ty = hy;
            add_coord(tx, ty);
        }
    }

    for (int i = 0; abs(i) < abs(yoff); i += ystep) {
        hy += ystep;
        if(!touch(hx,hy,tx,ty)){
            tx = hx;
            ty = hy - ystep;
            add_coord(tx, ty);
        }
    }
}

void interpret(char *data)
{
    int xoff=0, yoff=0;
    char dir = *data;
    data += 2;
    int i = get_integer(&data);

    switch (dir) {
    case 'D': yoff = -i; break;
    case 'U': yoff =  i; break;
    case 'L': xoff = -i; break;
    case 'R': xoff =  i; break;
    }

    move(xoff, yoff);
}

/**
 * The function that start to process the data
 * @param data A c_str that contains the input
 */
void start(char *data)
{
    if(!add_coord(0,0)){
        printf("Fail to add coord\n");
        goto failure;
    }

    while(*data){
        interpret(data);
        skip_line(&data);
    }

    result = nbCoords;

    failure:
    free(coords);
}



/* ===== MAIN FUNCTION ===== */

#include "aocmain.h"