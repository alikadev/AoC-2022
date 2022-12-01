#include <stdio.h>
#include <stdlib.h>

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