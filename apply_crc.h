/******************
 * CS11B026	  *
 * Vaishnavh N    *
 ******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CRC_DIVISOR 0x82608EDB
#define GET_LAST_BIT(q) (q & 0x00000001)


//A boolean function that checks if the divisor is small enough to divide the quotient
int is_divisor_smaller(struct Message quotient, struct Message divisor){
	int i;
	for(i = 4; i > 0; i--){
		if(divisor.quarter[i] || quotient.quarter[i]){ 
			//We have found a quarter where one of them is non-zero
			//If both the quarters are zero let's not waste time computing which of them is larger
			break;
		}
	}
	//Here, i is the message quarter we found one of them to be non-zero. Or if both quotient and divisor are zeros totally.
	if(!i){
		return 1; 	
	}
	unsigned int bit_pos =  POWER_32;
	while(bit_pos){ //Do this while we don't exhaust all 32 bit positions
		unsigned int divisor_bit = (bit_pos & divisor.quarter[i]);
		unsigned int quotient_bit = (bit_pos & quotient.quarter[i]);
		if(divisor_bit){
			//If divisor has a 1 at bit_pos while message doesn't have one there,
			//we have a larger divisor: return 0. 
			return quotient_bit;
		}else if(quotient_bit){
			//We have encountered a 1 in the message before the divisor
			//This WONT happen, but still let's have this for logical integrity of the code.
			return 1;
		}

		//Well, zeroes on both messages; try the next bit position
		bit_pos = bit_pos >> 1;
	}

	//We'll reach here only when both quotient and divisor are zero.
	//WONT happen in code
	//return 1;
}




//A function to right shift the divisor by 1 bit
struct Message right_shift_message(struct Message message){
	int i;
	unsigned int last_bit = 0;
	for(i = 4; i >=0; i--){
		unsigned int new_quarter = (message.quarter[i] >> 1) | (last_bit << 31);
		last_bit = GET_LAST_BIT(message.quarter[i]);
		message.quarter[i] = new_quarter;
	}
	return message;
}

//is_quotient_zero checks whether the first four parts of the message are
//zero indicating that quotient part of the message has been zeroed out
int is_quotient_zero(struct Message message){
	int i; 
	for(i = 1; i < 5; i++ ){
		if(message.quarter[i]){
			return 0;
		}
	}

	//Control comes here if all of them were zero
	return 1;
}

//get_xor computes the XOR of two MESSAGE_LENGTH bit strings
struct Message get_xor(struct Message message_a, struct Message message_b){
	struct Message xor_result;
	int i; 
	for(i = 0; i < 5; i++){
		xor_result.quarter[i] = message_a.quarter[i] ^ message_b.quarter[i]; //Compute XOR 
	}

	return xor_result;
}


//get_remainder computes the remainder of a message
int get_remainder(struct Message message){
	 //Set up the divisor polynomial
	 struct Message divisor;
	 divisor.quarter[4] = CRC_DIVISOR;
	 int i; 
	 for(i = 0; i < 4; i++){
		divisor.quarter[i] = 0; 
	 }  
	 message.quarter[0] = 0; //Padding with zeroes

	 while(!is_quotient_zero(message)){
		 //printf("Hi\n");
		while(!is_divisor_smaller(message, divisor)){
			//printf("Hi2\n");
			//Keep right shifting divisor until it is legal to divide the quotient
			divisor = right_shift_message(divisor);
		} 
		message = get_xor(message, divisor);
	 }

	 //The non-quotient portion, namely the last 32 bits of the message is the remainder
	 return message.quarter[0]; 

}


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
