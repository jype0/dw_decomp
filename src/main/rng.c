#include <dw/rng.h>
#include <dw/types.h>

#define CUSTOM_RNG_FACTOR	0x41c650ad
#define CUSTOM_RNG_VALUE	0x3039

extern uint32_t CUSTOM_RNG_VAL1;

// clang-format off
uint32_t CUSTOM_RNG_VAL2 = 0x0013cc25;
// clang-format on

int32_t customRandom(long min, long max)
{
	int32_t tmp;

	if (max == min) {
		return min;
	}

	if (max < min) {
		tmp = min;
		min = max;
		max = tmp;
	}

	CUSTOM_RNG_VAL1 = CUSTOM_RNG_VAL1 * CUSTOM_RNG_FACTOR + CUSTOM_RNG_VALUE;
	CUSTOM_RNG_VAL2 = CUSTOM_RNG_VAL2 * CUSTOM_RNG_FACTOR + CUSTOM_RNG_VALUE;

	return min + (int32_t)(((CUSTOM_RNG_VAL1 >> 16) | (CUSTOM_RNG_VAL2 << 16)) %
	                       (max - min + 1));
}
