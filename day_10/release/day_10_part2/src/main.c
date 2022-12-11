#define AOC_IN_FILE "data.in"
#define AOC_OUT_FILE "data.out"
#define AOC_OUT_FORMAT "%d\n"

static int result = 0;

#include "aocapp.h"

/* ===== AoC INCLUDES ===== */

#include <string.h>

uint32_t cycle = 1;
int32_t  x = 1;

#define WIDTH 40
char line[WIDTH+1];
char pixid = 0;

void check_instr(void)
{
    if(x - 1 == pixid || x == pixid || x + 1 == pixid)
        line[pixid] = '#';
    if(cycle % 40 == 0){
        printf("Cycle %3d -> %s <- Cycle %3d\n", cycle-39, line, cycle);
        for (int i = 0; i < WIDTH; ++i) {
            line[i] = '.';
        }
        pixid = 0;
    } else 
        pixid ++;
}

void addx(char *data)
{
    data += 2;
    
    // Check mid-instruction
    cycle++;
    check_instr();

    // Check negative
    int mul = 1;
    if(*data == '-'){
        data++;
        mul = -1;
    }
    // Apply
    x += get_integer(&data) * mul;

    cycle++;
}

/**
 * The function that start to process the data
 * @param data A c_str that contains the input
 */
void start(char *data)
{
    char instr[5]={0}, instrIdx=0;
        for (int i = 0; i < WIDTH; ++i) {
            line[i] = '.';
        }

    while(*data && cycle){
        check_instr();

        // Read instruction
        instr[instrIdx++] = *data++;
        instr[instrIdx++] = *data++;
        instr[instrIdx++] = *data++;
        instr[instrIdx++] = *data;

        // Check instruction type
        if(strcmp(instr, "addx") == 0){
            addx(data);
            skip_line(&data);
        } else {
            cycle++;
            skip_line(&data);
        }

        instrIdx = 0;
    }
}



/* ===== MAIN FUNCTION ===== */

#include "aocmain.h"