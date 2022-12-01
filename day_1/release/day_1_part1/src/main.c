#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DATA_IN_FILE "data.in"
#define DATA_OUT_FILE "data.out"

int max = 0, val=0;

char *read_data_input(void)
{
	FILE *file = fopen(DATA_IN_FILE, "r+");
	if(!file) return NULL;

	fseek(file, 0, SEEK_END);
	char *data = (char *)malloc(ftell(file)+1);
	rewind(file);

	int c, i = 0;
	while((c = fgetc(file)) != EOF)
		data[i++] = (char) c;
	data[i] = 0;

	fclose(file);

	return data;
}

int write_data_output(void)
{
	FILE *file = fopen(DATA_OUT_FILE, "w+");
	if(!file) return 0;

	fprintf(file, "%d", max);

	fclose(file);

	return 1;
}

void compare_and_apply(void)
{
	if(val > max) max = val;
}

int main(void)
{
	char *data = read_data_input();
	if(!data) return 1;

	char buff[16] = {0}, i = 0;

	// for all chars
	while(*data != 0){
		// if is list end, add to buffer
		if(isspace(*data)){
			buff[i] = 0;
			val += atoi(buff);
			i=0;
			// is next is space, compare and apply with current max
			if(isspace(*(data+1))){
				compare_and_apply();
				val = 0;
			}
		} else // add char to buffer
			buff[i++] = *data;

		// increment pointer
		data++;
	}
	// last compare
	compare_and_apply();

	return !write_data_output();


	return 0;
}