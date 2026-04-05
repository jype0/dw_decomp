#include <dw/swap.h>


void swapByte(uint8_t *a, uint8_t *b)
{
	uint8_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapShort(int16_t *a, int16_t *b)
{
	int16_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapInt(int32_t *a, int32_t *b)
{
	int32_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
