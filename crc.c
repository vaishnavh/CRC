#include "crc.h"
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

	//WONT happen in code
	return 1;
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

//Given a message, appends the CRC division remainder. 
struct Message apply_crc(struct Message message){
	message.quarter[0] = get_remainder(message);
	return message;
}


//Given a CRC output message, checks if there are any errors
int detect_error(struct Message message){
	unsigned int remainder = get_remainder(message);
	return (remainder);
}
