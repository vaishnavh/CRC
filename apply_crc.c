/******************
 * CS11B026	  *
 * Vaishnavh N    *
 ******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //For random number generation: bursty errors
#include "crc.h" //For applying crc
#include "corrupt.h" //For message corruption

#define ODD_MAX 127

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
	FILE* output_file;
	output_file = fopen(argv[2], "w"); //Open file to print MESSAGE_LENGTH+32 bit output strings.
	if(output_file == NULL){
		fprintf(stderr, "Output file \"%s\" cannot be opened\n",argv[2]);
		exit(1);
	}	

	int burst_successes = 0;
	int random_error_successes = 0;
	int two_bit_successes = 0;
	int one_bit_successes = 0;
	int count = 0;
	srand(time(NULL));
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
		message = apply_crc(message);
		
		fprintf(output_file,"Input: ");
		int k;
		for(k = 4; k > 0; k--){
			byte_to_binary(message.quarter[k], output_file);
		}
		fprintf(output_file,"\nCRC: ");	
		print_message(message, output_file);
		fprintf(output_file,"\n\n");
	
		struct Message corrupt_message;
		
		//One bit errors
		
		for(k = 0; k < 10; k++){
			corrupt_message = corrupt_one_bit(message);
			fprintf(output_file,"Original String with CRC: ");
			print_message(message, output_file);
			fprintf(output_file,"\nCorrupt String: ");
			print_message(corrupt_message, output_file);
			if(detect_error(corrupt_message)){
				one_bit_successes ++;
				fprintf(output_file,"\nCRC Check: Failed\n\n");
			}else{
				fprintf(output_file,"\nCRC Check: Passed\n\n");
			}
			count++;
		}

		
		//Two bit errors
		for(k = 0; k < 10; k++){
			corrupt_message = corrupt_two_bits(message);
			fprintf(output_file,"Original String with CRC: ");
			print_message(message, output_file);
			fprintf(output_file,"\nCorrupt String: ");
			print_message(corrupt_message, output_file);
			if(detect_error(corrupt_message)){
				fprintf(output_file,"\nCRC Check: Failed\n\n");
				two_bit_successes ++;
			}else{
				fprintf(output_file,"\nCRC Check: Passed\n\n");
			}
		}

		//Random odd number of errors
		int m;
		for(m = 0; m < 10; m++){
			k = (rand()%63)*2 + 3; //Generates 3 till 127 - only odd numbers 
			corrupt_message = corrupt_random(message, k);
			fprintf(output_file,"Original String with CRC: ");
			print_message(message, output_file);
			fprintf(output_file,"\nCorrupt String: ");
			print_message(corrupt_message, output_file);
			fprintf(output_file,"\nNumber of Errors: %d\n",k);
			if(detect_error(corrupt_message)){
				fprintf(output_file,"CRC Check: Failed\n\n");
				random_error_successes ++;
			}else{
				fprintf(output_file,"CRC Check: Passed\n\n");
			}
		}

		//Burst errors
		for(k = 0; k < 10; k++){
			int bit_pos = rand()%98;
			corrupt_message = corrupt_bursty(message, bit_pos);
			fprintf(output_file,"Original String with CRC: ");
			print_message(message, output_file);
			fprintf(output_file,"\nCorrupt String: ");
			print_message(corrupt_message, output_file);
			if(detect_error(corrupt_message)){
				fprintf(output_file,"\nCRC Check: Failed\n\n");
				burst_successes ++;
			}else{
				fprintf(output_file,"\nCRC Check: Passed\n\n");
			}
		}
		
			
	}
/*	
	printf("One-bit successes: %d of %d\n",one_bit_successes,count);
	printf("Two-bit successes: %d of %d\n",two_bit_successes,count);
	printf("Odd-Random-bit successes: %d of %d\n",random_error_successes,count*(ODD_MAX - 3 +2)/2);
	printf("Bursty successes: %d of %d\n",burst_successes,count);
*/
	fclose(input_file);
	fclose(output_file);
	
	return 0;
}
