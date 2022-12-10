#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DATA_IN_FILE "data.in"
#define DATA_OUT_FILE "data.out"

int max3 = 0, max2 = 0, max1 = 0, val = 0;

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

	fprintf(file, "%d", max1 + max2 + max3);

	fclose(file);

	return 1;
}

void compare_and_apply(void)
{
	if (val > max1) {
		max3 = max2;
		max2 = max1;
		max1 = val;
	} else if (val > max2) {
		max3 = max2;
		max2 = val;
	} else if (val > max3) {
		max3 = val;
	}
}

int main(void)
{
	char *start = read_data_input();
	char *data = start;
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
	// last val and compare
	buff[i] = 0;
	val += atoi(buff);
	i=0;
	compare_and_apply();

	free(start);
	return !write_data_output();

	return 0;
}