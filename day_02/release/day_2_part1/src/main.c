#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DATA_IN_FILE "data.in"
#define DATA_OUT_FILE "data.out"

int points = 0;

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

int write_data_input(void)
{
	FILE *file = fopen(DATA_OUT_FILE, "w+");
	if(!file) return 0;

	fprintf(file, "%d\n", points);

	fclose(file);

	return 1;
}

void compare_and_apply(char first, char second)
{
	first = first-'A';
	second = second-'X';
	points += second+1;

	int score = second - first;
	if ( score == 0 ){
		points += 3;
	} else if(( score > 0 || (first == 2 && second == 0)) && !(first==0 && second == 2)) {
		points += 6;
	}
}

int main(void)
{
	char *start = read_data_input();
	if(!start) {
		fprintf(stderr, "Fail to read input file '%s'\n", DATA_IN_FILE);
		return 1;
	}
	char *data = start;

	while(*data) {
		char first = *data;
		data += 2;
		char second = *data++;
		while(isspace(*data))
			data++;

		compare_and_apply(first, second);
	}

	if(!write_data_input()){
		fprintf(stderr, "Fail to write output file '%s'\n", DATA_IN_FILE);
		return 1;
	}

	free(start);
	return 0;
}