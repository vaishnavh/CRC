#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdlib.h>
#include <stdio.h>
#define POWER_32 0x80000000
#define MESSAGE_LENGTH 128

struct Message{
	unsigned int quarter[5]; 
};	
/*
 * A Message is an encapsulation for a
 * MESSAGE_LENGTH bit message that is split into four parts
 * stored as int values.
 * We have a dummy part for the remainder
 * Note:
 * Zeroeth index corresponds to the trailing quarter
 */

struct Message get_new_message();
void print_message(struct Message, FILE*);
void byte_to_binary(unsigned int, FILE*);
struct Message flip_bit(struct Message, int);
struct Message set_bit(struct Message, int);
struct Message get_xor(struct Message, struct Message);
int is_equal(struct Message, struct Message);
#endif
