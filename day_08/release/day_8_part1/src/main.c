#define AOC_IN_FILE "data.in"
#define AOC_OUT_FILE "data.out"
#define AOC_OUT_FORMAT "%d\n"

static int result = 0;

#include "aocapp.h"

/* ===== AoC INCLUDES ===== */

typedef unsigned int uint;
typedef unsigned char tree;

uint width;
uint height;
tree *data;

#define AT(x,y) [x + y * width]

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
            data AT(x, y) = *input - '0';
            input ++;
        }
        skip_line(&input);
    }
}

int check_tree(uint x, uint y)
{
    tree t = data AT(x, y);
    int state = 0;

    // LEFT: check each cell before
    for (int i = x-1; i >= 0; --i) {
        // Check it fail
        if(t <= data AT(i, y))
            break;
        
        // Check success when end
        if (i == 0)
            return 1;
    }

    // RIGHT: check each cella after
    for (int i = x+1; i < width; ++i) {
        // Check it fail
        if(t <= data AT(i, y))
            break;
        
        // Check success when end
        if (i == width-1)
            return 1;
    }

    // UP: check each cell before
    for (int i = y-1; i >= 0; --i) {
        // Check it fail
        if(t <= data AT(x, i))
            break;
        
        // Check success when end
        if (i == 0)
            return 1;
    }

    // DOWN: check each cella after
    for (int i = y+1; i < height; ++i) {
        // Check it fail
        if(t <= data AT(x, i))
            break;
        
        // Check success when end
        if (i == height-1)
            return 1;
    }

    return 0;
}

/**
 * The function that start to process the data
 * @param data A c_str that contains the input
 */
void start(char *input)
{
    init(input);
    result = (width - 1) * 4; // For border

    for (int y = 1; y < height-1; ++y) {
        for (int x = 1; x < width-1; ++x) {
            result += check_tree(x, y);
        }
    }

failure:
    free(data);
}



/* ===== MAIN FUNCTION ===== */

#include "aocmain.h"