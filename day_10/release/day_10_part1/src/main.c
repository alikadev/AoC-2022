#define AOC_IN_FILE "data.in"
#define AOC_OUT_FILE "data.out"
#define AOC_OUT_FORMAT "%d\n"

static int result = 0;

#include "aocapp.h"

/* ===== AoC INCLUDES ===== */

#include <string.h>

uint32_t cycle = 1;
int32_t  x = 1;

void check_instr(void)
{
    if(cycle > 220)
        return;
    
    if(cycle%40 == 20)
        result += cycle * x;
    
}

void addx(char *data)
{
    data += 2;
    
    // Check mid-instruction
    //x ++;
    cycle++;
    check_instr();
    //x --;

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

    while(*data && cycle <= 220){

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

        check_instr();

        instrIdx = 0;
    }
}



/* ===== MAIN FUNCTION ===== */

#include "aocmain.h"