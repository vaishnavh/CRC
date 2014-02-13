#include "corrupt.h"
#include <time.h>



//Generate one bit in the message
struct Message corrupt_one_bit(struct Message message){
	srand(time(NULL));
	int i = rand() % 128;
	return flip_bit(message, i)	
}


struct Message corrupt_two_bits(struct Message message){
	srand(time(NULL));
	int i = rand() % 128;
	int j = i;
	while(i  == j){
		j = rand()%128;
	}
	message = flip_bit(message, i);
	return flip_bit(message, j);

}

//Generate a bursty error of length
struct Message corrupt_bursty(struct Message message, int index){
	//Assumes index < 128 - 30
	srand(time(NULL));
	int i;
	//Extreme two bits flip necessarily
	message = flip_bit(message, index);
	message = flip_bit(message, index + 29);
	for(i = 1; i < 29; i++){
		if(rand()%2){
			message = flip_bit(message, index + i);
		}
	}
	return message;
}


struct Message corrupt_random(struct Message message, int count){
	srand(time(NULL));
	struct Message corruptor = get_new_message();
	while(count){
		int bit_pos = rand() % 128;
		struct Message new_corruptor = set_bit(corruptor, bit_pos);
		if(!is_equal(new_corruptor)){
			count --;	
			corruptor = new_corruptor;
			//A new bit has been set
		}
	}

	message = get_xor(message, corruptor);
	return message;

}
