#ifndef CRC_H_
#define CRC_H_

#include "message.h"

#define CRC_DIVISOR 0x82608EDB
#define GET_LAST_BIT(q) (q & 0x00000001)

struct Message apply_crc(struct Message);
int detect_error(struct Message);
#endif 
