#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_IN_FILE "data.in"
#define DATA_OUT_FILE "data.out"

int sum = 0;

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

	fprintf(file, "%d\n", sum);

	fclose(file);

	return 1;
}

int main(void)
{
	char *start = read_data_input();
	if(!start) {
		fprintf(stderr, "Fail to read input file '%s'\n", DATA_IN_FILE);
		return 1;
	}
	char *data = start;

	// get number of line
	int nbLine = 0;
	char *currentLine = data;
	char **lines = NULL;

	while(*data != 0){
		if(*data == '\n'){ // when line ends
			*data = 0;
			nbLine ++;
			lines = realloc(lines, sizeof(char*) * nbLine);
			lines[nbLine-1] = currentLine; // add current line
			currentLine = data + 1;
		}
		data++;
	}

	// check for 3 line
	for(int i = 0; i < nbLine; i+=3){
		// lines...
		char *r1 = lines[i+0];
		char *r2 = lines[i+1];
		char *r3 = lines[i+2];

		// for each char in r1
		for (int c1 = 0; c1 < strlen(r1); ++c1)
		{
			// for each char in r2
			for (int c2 = 0; c2 < strlen(r2); ++c2)
			{
				// if they're equals, compute r3
				if(r1[c1] == r2[c2]){
					for (int c3 = 0; c3 < strlen(r3); ++c3)
					{
						// if they're equals, save and goto end
						if(r1[c1] == r3[c3]){
							sum += r1[c1] > 'Z' ? r1[c1]-'a'+1 : r1[c1]-'A'+27;
							goto end;
						}
					}
				}
			}
		}
		end:;
	}

	if(!write_data_output()){
		printf("Fail to write data output...\nvalue is %d\n", sum);
	}

	free(lines);
	free(start);
	return 0;
}