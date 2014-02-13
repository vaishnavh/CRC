#include "message.h"
void byte_to_binary(unsigned int x, FILE* file_stream){

    unsigned int z;
    for (z = POWER_32; z > 0; z >>= 1){
        fprintf(file_stream,"%c",((x & z) == z) ? '1' : '0');
    }

}

//Generates an message with all zeroes
struct Message get_new_message(){
	struct Message message;
	int i;
	for(i = 0; i < 5; i++){
		message.quarter[i] = 0;
	}
	return message;
}

//Print message in binary
void print_message(struct Message message, FILE* file_stream){
	int i;
	for(i = 4; i >= 0; i--){
		byte_to_binary(message.quarter[i], file_stream);
	}
}


//Set a specific bit in the message
struct Message set_bit(struct Message message, int bit_pos){
	int q_no = 4 - (4*bit_pos/MESSAGE_LENGTH); //bit_pos \in [0,127]
	
	message.quarter[q_no] = message.quarter[q_no] | (1 << bit_pos);
	return message;
}

//Flip a bit in the message
struct Message flip_bit(struct Message message, int bit_pos){
	int q_no = 4 - (4*bit_pos/MESSAGE_LENGTH); //bit_pos \in [0,127]
	
	message.quarter[q_no] = message.quarter[q_no] ^ (1 << bit_pos);
	return message;
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

//check if two messages are equal
int is_equal(struct Message message_a, struct Message message_b){
	int i = 0;
	for(i = 0; i < 5; i++){
		if(message_a.quarter[i] != message_b.quarter[i]){
			return 0;
		}
	}

	return 1;
}

