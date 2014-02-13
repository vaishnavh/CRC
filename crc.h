#ifndef CRC_H_
#define CRC_H_

#include "message.h"

#define CRC_DIVISOR 0x826096FD
#define GET_LAST_BIT(q) (q & 0x00000001)

struct Message apply_crc(struct Message);
unsigned int detect_error(struct Message);
#endif 
