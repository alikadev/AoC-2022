#define AOC_IN_FILE "data.in"
#define AOC_OUT_FILE "data.out"
#define AOC_OUT_FORMAT "%d\n"

static int result = 0;

#include "aocapp.h"

/* ===== AoC INCLUDES ===== */

typedef unsigned char tree;

int width=0;
int height=0;
tree *data;

/**
 * Get the tree height at a 2d coord
 * @param  x The x position
 * @param  y The y position
 * @return   The tree height
 */
tree data_at(int x, int y)
{
    return data[x + y * width];
}

/**
 * Process the input, alloc the data
 * @param input The data input
 */
void init(char *input)
{
    // Clear data
    width = 0;
    height = 0;

    // Get width
    while(input[width] != '\n' && input[width] != '\r')
        width ++;
    
    // Get height
    char *tmpInput = input;
    while(*tmpInput){
        height ++;
        tmpInput += width;
        skip_line(&tmpInput);
    }

    // Copy to data
    data = (tree *)malloc(sizeof (tree) * width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[y*width+x] = *input - '0';
            input ++;
        }
        skip_line(&input);
    }
}

/**
 * Count the number of tree that are visible in a direction
 * @param  x    The start x coord
 * @param  y    The start y coord
 * @param  xoff The direction x offset
 * @param  yoff The direction y offset
 * @return      The number of tree that are visiable
 */
int count_visible(int x, int y, int xoff, int yoff)
{
    // Init vars
    tree t = data_at(x, y);
    int nbTree = 1;

    // Do not check the current tree
    x += xoff;
    y += yoff;

    // Loop while it fail
    do {
        // Check failure
        if( x == width-1 || y == height-1 || 
            x == 0 ||       y == 0 || 
            t <= data_at(x, y))
        {
            return nbTree;
        }

        // Inctement
        x += xoff;
        y += yoff;
        nbTree++;
    } while(1);
}

/**
 * Get the scenic score of a tree
 * @param  x The X position of the tree
 * @param  y The Y position of the tree
 * @return   The scenic score
 */
int get_scenic_score(int x, int y)
{
    // init
    int score = 1, i, nbTree;
    tree t = data_at(x,y);

    // RIGHT
    score = count_visible(x, y, 1, 0);
    
    // LEFT
    score *= count_visible(x, y, -1, 0);
    
    // DOWN
    score *= count_visible(x, y, 0, 1);

    // UP
    score *= count_visible(x, y, 0, -1);

    return score;
}

/**
 * The function that start to process the data
 * @param data A c_str that contains the input
 */
void start(char *input)
{
    // init...
    init(input);

    // For each tree
    // Check the score
    result = 0;
    for (int y = 1; y < height-1; ++y) {
        for (int x = 1; x < width-1; ++x) {
            int new = get_scenic_score(x,y);
            if(new > result)
                result = new;
        }
    }

    // Clean memory
    free(data);
}



/* ===== MAIN FUNCTION ===== */

#include "aocmain.h"