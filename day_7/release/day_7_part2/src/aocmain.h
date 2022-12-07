#ifndef AOC_MAIN_H_
#define AOC_MAIN_H_


#ifndef AOC_IN_FILE
#error You need to define AOC_IN_FILE before including the aocapp.h
#endif

#ifndef AOC_OUT_FILE
#error You need to define AOC_OUT_FILE before including the aocapp.h
#endif

#ifndef AOC_OUT_FORMAT
#error You need to define AOC_OUT_FORMAT before including the aocapp.h
#endif

/**
 * The main function
 * @return  The error code
 */
int main(void)
{
	// Get file content
	char *data = read_data_input();
	if(!data){
		fprintf(stderr, "Fail to open the input file '%s'...\n", 
			AOC_IN_FILE);
		return 1;
	}

	// process input
	start(data);

	// write output
	if(!write_data_output()){
		printf("Fail to create or open the output file '%s'...\n", 
				AOC_OUT_FILE);
		printf("The program result is " AOC_OUT_FORMAT "\n",
				result);
	}

	// free memory
	free(data);
	return 0;
}

#endif