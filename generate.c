#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define STRING_LENGTH 128
int main(int argc, char **argv){
	if(argc < 3){
		fprintf(stderr, "Run command must be of the following format: ./generate <input_file> <no_of_strings>\n");
		exit(1);
	}

	/* initialize random seed: */
	srand (time(NULL));
	
	FILE* output_file = fopen(argv[1], "w");
	int no_of_strings = atoi(argv[2]); //Number of strings to be generated
	int i;
	for(i = 0; i < no_of_strings; i++){
		int j;
		for(j = 0; j < STRING_LENGTH; j++){
			char bit = '0' + (rand() % 2); //Generate a random bit
			putc(bit, output_file);
		}
		putc('\n', output_file);
	}

	fclose(output_file);
}
