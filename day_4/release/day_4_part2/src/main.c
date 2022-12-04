#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_IN_FILE "data.in"
#define DATA_OUT_FILE "data.out"

int nbContains = 0;

typedef int range_t[2];

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

	fprintf(file, "%d\n", nbContains);

	fclose(file);
	return 1;
}

int get_integer(char **data)
{
	char buffer[10];
	int i = 0;

	while(**data >= '0' && **data <= '9'){
		buffer[i] = **data;
		i++;
		(*data)++;
	}
	buffer[i] = 0;
	return atoi(buffer);
}

void skip_spaces(char **data) {
	while(isspace(**data))
		(*data) ++;
}

int main(void)
{
	char *start = read_data_input();
	if(!start){
		fprintf(stderr, "Fail to read input file '%s'\n", DATA_IN_FILE);
		return 1;
	}
	char *data = start;

	while(*data != 0) {
		range_t first, second;
		first[0] = get_integer(&data);
		data ++;
		first[1] = get_integer(&data);
		data ++;
		second[0] = get_integer(&data);
		data ++;
		second[1] = get_integer(&data);
		skip_spaces(&data);

		for (int i = first[0]; i <= first[1]; ++i) {
			for (int j = second[0]; j <= second[1]; ++j) {
				if(i == j){
					nbContains++;
					goto end;
					break;
				}
			}
		}
		end:;
	}

	if(!write_data_output()){
		fprintf(stderr, "Fail to write data output... value is %d\n", nbContains);
	}
	free(start);
	return 0;
}