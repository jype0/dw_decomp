#ifndef DW_MATH_H
#define DW_MATH_H

#include <dw/types.h>
#include <dw/file_table.h>

#define ABS(x) (((x) < 0) ? -(x) : (x))

int32_t random(int32_t limit);
int32_t _sin(int32_t input);
int32_t _cos(int32_t input);
int32_t _atan(int32_t dy, int32_t dx);

#endif
