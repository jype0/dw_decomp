#include <libgte.h>
#include <libgs.h>

#include <dw/butterfly.h>
#include <dw/garbage.h>
#include <dw/math.h>
#include <dw/version.h>
#include <dw/world_object.h>

#define NUM_BUTTERFLIES		2

typedef struct {
	int16_t frame;
	int16_t pad;
	Entity *entity;
} Butterfly;

int32_t lerp(int32_t start, int32_t end, int32_t tMin, int32_t tMax,
	     int32_t tCurrent);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
int32_t worldPosToScreenPos(SVECTOR *worldPos, DVECTOR *screenPos);
void renderSprite(GsSPRITE *sprite, int32_t x, int32_t y, int32_t distance,
		  int32_t scaleX, int32_t scaleY);

extern int32_t IS_IN_MENU;

Butterfly BUTTERFLIES[NUM_BUTTERFLIES];

GsSPRITE BUTTERFLY_SPRITE = { // NOLINT used in not-implemented function
	0x50000000,	/* attribute */
	0x0,		/* x */
	0x0,		/* y */
	0x10,		/* w */
	0x10,		/* h */
	0x3f,		/* tpage */
	0x00,		/* u */
	0xb0,		/* v */
	0x110,		/* cx */
	0x1e2,		/* cy */
	0x80,		/* r */
	0x80,		/* g */
	0x80,		/* b */
	0x8,		/* mx */
	0x8,		/* my */
	0x0,		/* scalex */
	0x0,		/* scaley */
	0x0,		/* rotate */
};

#if VERSION_IS(EU)
static void *butterfly_functions[] = {
	unsetButterfly,
	setButterfly,
	tickButterfly,
	renderButterfly,
	initializeButterfly,
};
#endif

void initializeButterfly(void)
{
	int32_t i;

	for (i = 0; i < NUM_BUTTERFLIES; ++i) {
		BUTTERFLIES[i].frame = -1;
	}
}

GARBAGE(setButterfly, 5);

int32_t setButterfly(Entity *entity)
{
	Butterfly *butterfly;
	int32_t i;

	for (i = 0; i < NUM_BUTTERFLIES; ++i) {
		if (BUTTERFLIES[i].frame < 0) {
			break;
		}
	}

	if (i == NUM_BUTTERFLIES) {
		return -1;
	}

	butterfly = &BUTTERFLIES[i];
	butterfly->frame = 0;
	butterfly->entity = entity;
	addObject(0x80e, i, tickButterfly, renderButterfly);

	return i;
}

void tickButterfly(int32_t instanceId)
{
	Butterfly *butterfly = &BUTTERFLIES[instanceId];

	if (IS_IN_MENU != 1) {
		++butterfly->frame;
	}
}

void renderButterfly(int32_t instanceId)
{
	Butterfly *butterfly;
	SVECTOR pos;
	DVECTOR screen;
	int32_t angle;
	int32_t depth;

	butterfly = &BUTTERFLIES[instanceId];

	if (IS_IN_MENU == 1) {
		return;
	}

	translateConditionFXToEntity(butterfly->entity, &pos);

	angle = lerp(0, 0x200, 0, 0x3c, butterfly->frame);
	pos.vx += (_sin(angle) * 150) / 4096;
	pos.vz += (_cos(angle) * 150) / 4096;

	angle = lerp(0, 0x600, 0, 0x3c, butterfly->frame);
	pos.vy += ((_sin(angle) * 50) / 4096) - 100;

	depth = worldPosToScreenPos(&pos, &screen);
	if ((depth > 0x200) && (depth < 0x10000)) {
		BUTTERFLY_SPRITE.u = ((butterfly->frame >> 1) & 1) << 4;
		renderSprite(&BUTTERFLY_SPRITE, screen.vx, screen.vy, depth,
			     0x5400, 0x5400);
	}
}

void unsetButterfly(int32_t instanceId)
{
	if (instanceId >= 0) {
		BUTTERFLIES[instanceId].frame = -1;
		removeObject(0x80e, instanceId);
	}
}
