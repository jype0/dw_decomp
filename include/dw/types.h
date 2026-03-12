#ifndef DW_TYPES_H
#define DW_TYPES_H

#ifndef NULL
#define NULL 0
#endif

typedef char dw_bool;

#define DW_FALSE (0 == 1)
#define DW_TRUE (!DW_FALSE)

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#endif
