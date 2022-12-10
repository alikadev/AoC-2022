#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DATA_IN_FILE "data.in"
#define DATA_OUT_FILE "data.out"

int value = 0;

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

	fprintf(file, "%d\n", value);

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

	printf("Hello, world!\nFile content:\n%s\n",data);

	free(start);
	return 0;
}