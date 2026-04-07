#include <libgte.h>
#include <libgs.h>

#include <dw/butterfly.h>
#include <dw/world_object.h>

#include "common.h"


#define NUM_BUTTERFLIES		2


extern int32_t IS_IN_MENU;


typedef struct {
	int16_t frame;
	int16_t pad;
	Entity *entity;
} Butterfly;

static Butterfly BUTTERFLIES[NUM_BUTTERFLIES];

static GsSPRITE BUTTERFLY_SPRITE = { // NOLINT used in not-implemented function
	0x50000000,	/* attribute */
	0x0,		/* x */
	0x0,		/* y */
	0x10,		/* w */
	0x10,		/* h */
	0x3F,		/* tpage */
	0x00,		/* u */
	0xB0,		/* v */
	0x110,		/* cx */
	0x1E2,		/* cy */
	0x80,		/* r */
	0x80,		/* g */
	0x80,		/* b */
	0x8,		/* mx */
	0x8,		/* my */
	0x0,		/* scalex */
	0x0,		/* scaley */
	0x0,		/* rotate */
};
void* butterfly_functions[] = {
	unsetButterfly,
	renderButterfly,
	tickButterfly,
	setButterfly,
	initializeButterfly
};
void initializeButterfly(void)
{
	int32_t i;

	for (i = 0; i < NUM_BUTTERFLIES; ++i) {
		BUTTERFLIES[i].frame = -1;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/butterfly", setButterfly);

void tickButterfly(int32_t instanceId)
{
	Butterfly *butterfly = &BUTTERFLIES[instanceId];
	if (IS_IN_MENU != 1) {
		++butterfly->frame;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/butterfly", renderButterfly);

void unsetButterfly(int32_t instanceId)
{
	if (instanceId >= 0) {
		BUTTERFLIES[instanceId].frame = -1;
		removeObject(0x80e, instanceId);
	}
}
