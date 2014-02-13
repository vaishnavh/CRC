#ifndef CORRUPT_H_
#define CORRUPT_H_

#include "message.h"

struct Message corrupt_one_bit(struct Message);
struct Message corrupt_two_bits(struct Message);
struct Message corrupt_bursty(struct Message);
struct Message corrupt_random(struct Message, int);

#endif
