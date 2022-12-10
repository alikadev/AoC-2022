#ifndef AOC_APP_H_
#define AOC_APP_H_

#ifndef AOC_IN_FILE
#error You need to define AOC_IN_FILE before including the aocapp.h
#endif

#ifndef AOC_OUT_FILE
#error You need to define AOC_OUT_FILE before including the aocapp.h
#endif

#ifndef AOC_OUT_FORMAT
#error You need to define AOC_OUT_FORMAT before including the aocapp.h
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * Read the input file and put the content in a c_str
 * @return  A c_str with the content of the file (Need to be free)
 */
char *read_data_input(void)
{
	// Open the file
	FILE *file = fopen(AOC_IN_FILE, "r+");
	if(!file) return NULL;

	// Allocate the string
	fseek(file, 0, SEEK_END);
	char *data = (char *)malloc(ftell(file)+1);
	rewind(file);

	// Put the content
	int c, i = 0;
	while((c = fgetc(file)) != EOF)
		data[i++] = (char) c;
	data[i] = 0;

	// Return
	fclose(file);
	return data;
}


/**
 * Write the result in the output file
 * @return  Return 1 if success, else 0
 */
int write_data_output(void)
{
	// Open the file
	FILE *file = fopen(AOC_OUT_FILE, "w+");
	if(!file) return 0;

	// Write the content
	fprintf(file, AOC_OUT_FORMAT, result);

	// Return
	fclose(file);
	return 1;
}


/**
 * Read the integer in a c_str
 * @param  pData The pointer an integer in the c_str (will modify the address)
 * @return       The integer
 */
int get_integer(char **pData)
{
	// Variables
	char buffer[16];
	int i = 0;

	// Put the integer chars in the buffer
	while(**pData >= '0' && **pData <= '9'){
		buffer[i] = **pData;
		i++;
		(*pData)++;
	}
	buffer[i] = 0;

	// Return the integer
	return atoi(buffer);
}

/**
 * Skip all spaces in a string
 * @param  pData The pointer to the c_str
 */
void skip_spaces(char **pData)
{
	// Increment the pointer while it point to char
	while(isspace(**pData))
		(*pData) ++;
}

/**
 * Skip the current line (until stop after the '\n')
 * @param  pData The pointer to the c_str
 */
void skip_line(char **pData)
{
	// Skip until it reach a '\n'
	while(**pData != '\n')
		(*pData) ++;
	// skip the '\n'
	(*pData) ++;
}


#endif

