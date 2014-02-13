/******************
 * CS11B026	  *
 * Vaishnavh N    *
 ******************/

#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include "crc.h"


int main(int argc, char** argv){
	if(argc < 3){
		fprintf(stderr,"To run program enter \"./ec <input_file> <output_file>\"\n");
		exit(1);
	}

	FILE* input_file = fopen(argv[1], "r"); //Open file to read MESSAGE_LENGTH bit strings
	if(input_file == NULL){
		fprintf(stderr, "Input file cannot be opened\n");
		exit(1);
	}

	FILE* output_file = fopen(argv[2], "w"); //Open file to print MESSAGE_LENGTH+32 bit output strings.
	if(output_file == NULL){
		fprintf(stderr, "Output file cannot be opened\n");
		exit(1);
	}	
	
	char bit = '0';
	while(bit!=EOF){
		//Read till last line in file
		int bit_pos;
		
		//printf("%c, %zd, %d\n",input_string[127],len,MESSAGE_LENGTH);	
		struct Message message = get_new_message();
		for(bit_pos = 0; bit_pos < MESSAGE_LENGTH; bit_pos++){
			bit = getc(input_file);//input_string[bit_pos];
			if(bit == EOF){
				break;
			}else if(bit != '0' && bit != '1'){
				fprintf(stderr, "Incorrect input string : %c!\n",bit);
				exit(1);
			}
			int q_no = 4 - (4*bit_pos/MESSAGE_LENGTH); 
			//Which quarter number does this bit go into
			message.quarter[q_no] = (message.quarter[q_no] << 1) | (bit - '0');
		}
		if(bit == EOF){
			break;
		}
		bit = getc(input_file);


		//We now have a message. We get the remainder and store it in the
		//padded region.
		message.quarter[0] = get_remainder(message);

		print_message(message, output_file);
		putc('\n',output_file);
	}
	fclose(input_file);
	fclose(output_file);
	return 0;
}
