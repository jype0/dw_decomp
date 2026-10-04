#ifndef DW_LINE_H
#define DW_LINE_H

#include <dw/types.h>

void drawLine3P(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                int32_t order, uint32_t mode);
void drawLine2P(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);

#endif
