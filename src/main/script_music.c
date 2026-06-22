#include <dw/clock.h>
#include <dw/pstat.h>
#include <dw/script.h>

static void *script_music_functions[] = {
	handleMusicOverride,
};

void handleMusicOverride(uint8_t *outFont, uint8_t *outVariant)
{
	switch (readPStat(PSTAT_245) & 0xff) {
	case 0:
		if (HOUR >= 6 && HOUR < 21) {
			*outVariant = 0;
		} else {
			*outVariant = 1;
		}
		break;
	case 1: *outVariant = 0;
		break;
	case 2:
		*outFont = 6;
		*outVariant = 2;
		break;
	case 3:
		*outFont = 0xb;
		*outVariant = 2;
		break;
	case 4:
		*outFont = 0xf;
		*outVariant = 2;
		break;
	case 5:
		*outFont = 0x15;
		*outVariant = 0;
		break;
	case 6:
		*outFont = 0x15;
		*outVariant = 1;
		break;
	case 7:
		*outFont = 0x1a;
		*outVariant = 0;
		break;
	case 8:
		*outFont = 0x1a;
		*outVariant = 1;
		break;
	case 9:
		*outFont = 0x1b;
		*outVariant = 0;
		break;
	case 0xa:
		*outFont = 0x1b;
		*outVariant = 1;
		break;
	}
}
