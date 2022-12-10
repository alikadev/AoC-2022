#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DATA_IN_FILE "data.in"
#define DATA_OUT_FILE "data.out"

int value = 0;

typedef struct stk_s {
	union{struct stk_s *next; struct stk_s *n;};
	union{char value; char v;};
} stk_t;

stk_t *roots;
int nbStack = 0;

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

char *init_stack_list(char *start)
{
	char *data = start;
	int nChrHead = 0;
	while(*data != '\r' && *data != '\n') {
		nChrHead++;
		data++;
	}
	nbStack = nChrHead / 4 + 1;
	roots = malloc(sizeof *roots * nbStack);
	for (int i = 0; i < nbStack; ++i)
	{
		roots[i].n = NULL;
	}
	data = start;

	/*
	 * 	if R.n
	 *		X.n = R.n
	 *	R.n = X
	 */
	int line = 0;
	while(1){
		if(line >= nbStack) line = 0;
		if(*data == '['){
			stk_t *x = malloc(sizeof *x);
			x->v = *(data+1);
			stk_t *root = &(roots[line]);
			if(root->n)
				x->n = root->n;
			root->n = x;
			data+=3;
			if(*data == ' ')
				data++;
			else{
				while(*data == '\r' || *data == '\n'){
					data++;
				}
			}
		} else if(*(data+1) == '1'){
			data += nChrHead;
			skip_spaces(&data);
			break;
		} else {
			data += 4;
		}
		line++;
	}

	return data;
}

void clean_stack(stk_t *pStack)
{
	if(pStack->next)
		clean_stack(pStack->next);
	free(pStack);
}

stk_t *get_last_node(stk_t *stack)
{
	while(stack->n != NULL)
		stack = stack->n;
	return stack;
}
stk_t *get_parent_node(stk_t *stack, stk_t *parent)
{
	while(stack->n != parent){
		if(stack->n == NULL) return NULL;
		stack = stack->n;
	}
	return stack;
}

void transfer(int dest, int src, int nMove)
{
	stk_t *srcLast = get_last_node(&(roots[src]));
	for (int i = 1; i < nMove; ++i)
	{
		srcLast = get_parent_node(&(roots[src]), srcLast);
	}
	stk_t *destLast = get_last_node(&(roots[dest]));
	destLast->n = srcLast;
	get_parent_node(&(roots[src]),srcLast)->n = NULL;
}

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

	char *data = malloc(nbStack+1);
	for (int i = 0; i < nbStack; ++i) {
		data[i] = get_last_node(&(roots[i]))->v;
	}
	fprintf(file, "%s\n", data);

	free(data);
	fclose(file);
	return 1;
}

int main(void)
{
	char *start = read_data_input();
	if(!start){
		fprintf(stderr, "Fail to read input file '%s'\n", DATA_IN_FILE);
		return 1;
	}
	
	char *data = init_stack_list(start);

	while(*data){
		data += 5; // "move "
		int nMove = get_integer(&data);
		data += 6; // " from "
		int src = get_integer(&data)-1;
		data += 4; // " to "
		int dest = get_integer(&data)-1;
		skip_spaces(&data);

		transfer(dest, src, nMove);
	}

	if(!write_data_output()){
		printf("Fail to write data output...\n");
	}

	for (int i = 0; i < nbStack; ++i) {
		clean_stack(roots[i].n);
	}
	free(roots);
	free(start);
	return 0;
}