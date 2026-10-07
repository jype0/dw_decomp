#include <dw/bubble.h>
#include <dw/params.h>
#include <dw/world_object.h>
#include <dw/math.h>

#define NUM_CONDITION_BUBBLES	3

typedef struct {
	int16_t duration;
	int16_t nextIcon;
	int16_t icon;
} ConditionIcon;

typedef struct {
	int8_t frame[19];
} ConditionFramesHungry;

typedef struct {
	int8_t frame[13];
} ConditionFramesPoopy;

typedef struct {
	int8_t frame[20];
} ConditionFramesInjured;

typedef struct {
	int8_t frame[19];
} ConditionFramesEvolving;

typedef struct {
	int16_t frameCount;
	int16_t activeHeight;
	int16_t baseHeight;
	int16_t frameCount2;
	ConditionIcon *iconPtr;
	uint32_t scale;
	uint32_t scale2;
	Entity *entity;
} ConditionBubble;

void setInt16WithStride(int16_t* ptr, int16_t value, int32_t count,
			int32_t stride);
int32_t lerp(int32_t start, int32_t end, int32_t tMin, int32_t tMax,
	     int32_t tCurrent);
int32_t worldPosToScreenPos(SVECTOR* worldPos, DVECTOR* screenPos);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
void renderSprite(GsSPRITE* sprite, int32_t x, int32_t y, int32_t distance,
		  int32_t scaleX, int32_t scaleY);

extern int32_t VIEWPORT_DISTANCE;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t IS_IN_MENU;

GsSPRITE CONDITION_SPRITES[12] = {
	/* 0: Hungry */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x10,		/* w */
		0x10,		/* h */
		0x3F,		/* tpage */
		0x90,		/* u */
		0x80,		/* v */
		0x110,		/* cx */
		0x1E3,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x1A,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 1: Poopy */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x10,		/* w */
		0x10,		/* h */
		0x3F,		/* tpage */
		0xD0,		/* u */
		0x90,		/* v */
		0x110,		/* cx */
		0x1E3,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x1A,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 2: Dying (unused) */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x10,		/* w */
		0x10,		/* h */
		0x3F,		/* tpage */
		0xE0,		/* u */
		0xB0,		/* v */
		0x110,		/* cx */
		0x1E2,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x1A,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 3: Sleepy, low Z */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x8,		/* w */
		0x8,		/* h */
		0x3F,		/* tpage */
		0x80,		/* u */
		0xB0,		/* v */
		0x110,		/* cx */
		0x1E2,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x0,		/* mx */
		0xD,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 4: Sleepy, middle Z */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x8,		/* w */
		0x8,		/* h */
		0x3F,		/* tpage */
		0x98,		/* u */
		0x90,		/* v */
		0x110,		/* cx */
		0x1E2,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x13,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 5: Sleepy, high Z */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x8,		/* w */
		0x8,		/* h */
		0x3F,		/* tpage */
		0x90,		/* u */
		0x90,		/* v */
		0x110,		/* cx */
		0x1E2,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x3,		/* mx */
		0x1C,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 6: Tired */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x10,		/* w */
		0x10,		/* h */
		0x3F,		/* tpage */
		0xC0,		/* u */
		0x80,		/* v */
		0x110,		/* cx */
		0x1E1,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x17,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 7: Sick */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x10,		/* w */
		0x10,		/* h */
		0x3F,		/* tpage */
		0xC0,		/* u */
		0x90,		/* v */
		0x110,		/* cx */
		0x1E1,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x1A,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 8: Injured */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x10,		/* w */
		0x10,		/* h */
		0x3F,		/* tpage */
		0xD0,		/* u */
		0xA0,		/* v */
		0x110,		/* cx */
		0x1E3,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x1A,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 9: Evolving */
	{
		0x50000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x10,		/* w */
		0x10,		/* h */
		0x3F,		/* tpage */
		0x80,		/* u */
		0xB0,		/* v */
		0x110,		/* cx */
		0x1E2,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x1A,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 10: Bubble bursting */
	{
		0x40000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x10,		/* w */
		0x10,		/* h */
		0x5F,		/* tpage */
		0xD0,		/* u */
		0xB0,		/* v */
		0x110,		/* cx */
		0x1E4,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x8,		/* mx */
		0x8,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
	/* 11: Bubble */
	{
		0x40000000,	/* attribute */
		0x0,		/* x */
		0x0,		/* y */
		0x20,		/* w */
		0x20,		/* h */
		0x5F,		/* tpage */
		0xA0,		/* u */
		0x90,		/* v */
		0x110,		/* cx */
		0x1E4,		/* cy */
		0x80,		/* r */
		0x80,		/* g */
		0x80,		/* b */
		0x10,		/* mx */
		0x1F,		/* my */
		0x0,		/* scalex */
		0x0,		/* scaley */
		0x0,		/* rotate */
	},
};

GsSPRITE *CONDITION_SPRITES_NONE[] = { NULL };

GsSPRITE *CONDITION_SPRITES_HUNGRY[] = {
	&CONDITION_SPRITES[0], NULL,
};

GsSPRITE *CONDITION_SPRITES_POOPY[] = {
	&CONDITION_SPRITES[1], NULL,
};

GsSPRITE *CONDITION_SPRITES_DYING[] = {
	&CONDITION_SPRITES[2], NULL,
};

GsSPRITE *CONDITION_SPRITES_SLEEPY_0[] = {
	&CONDITION_SPRITES[3], NULL,
};

GsSPRITE *CONDITION_SPRITES_SLEEPY_1[] = {
	&CONDITION_SPRITES[3], &CONDITION_SPRITES[4], NULL,
};

GsSPRITE *CONDITION_SPRITES_SLEEPY_2[] = {
	&CONDITION_SPRITES[3], &CONDITION_SPRITES[4],
	&CONDITION_SPRITES[5], NULL,
};

GsSPRITE *CONDITION_SPRITES_SLEEPY_3[] = {
	&CONDITION_SPRITES[4], &CONDITION_SPRITES[5], NULL,
};

GsSPRITE *CONDITION_SPRITES_SLEEPY_4[] = {
	&CONDITION_SPRITES[5], NULL,
};

GsSPRITE *CONDITION_SPRITES_SICK[] = {
	&CONDITION_SPRITES[7], NULL,
};

GsSPRITE *CONDITION_SPRITES_INJURED[] = {
	&CONDITION_SPRITES[8], NULL,
};

GsSPRITE **CONDITION_SPRITES_ICON[12] = {
	CONDITION_SPRITES_NONE,
	CONDITION_SPRITES_HUNGRY,
	CONDITION_SPRITES_POOPY,
	CONDITION_SPRITES_DYING,
	CONDITION_SPRITES_SLEEPY_0,
	CONDITION_SPRITES_SLEEPY_1,
	CONDITION_SPRITES_SLEEPY_2,
	CONDITION_SPRITES_SLEEPY_3,
	CONDITION_SPRITES_SLEEPY_4,
	NULL,
	CONDITION_SPRITES_SICK,
	CONDITION_SPRITES_INJURED,
};

ConditionFramesHungry CONDITION_HUNGRY_FRAMES = {
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3 }
};

ConditionFramesPoopy CONDITION_POOPY_FRAMES = {
	{ 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1 }
};

ConditionFramesInjured CONDITION_INJURED_FRAMES = {
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0 }
};

ConditionFramesEvolving CONDITION_EVOLVING_FRAMES = {
	{ 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
};

ConditionIcon CONDITION_ICON_HUNGRY = { 100, 0, 1 };
ConditionIcon CONDITION_ICON_POOPY = { 100, 0, 2 };
ConditionIcon CONDITION_ICON_DYING = { 100, 0, 3 };
ConditionIcon CONDITION_ICON_TIRED = { 1000, 0, 9 };
ConditionIcon CONDITION_ICON_SICK = { 100, 0, 10 };
ConditionIcon CONDITION_ICON_INJURED = { 100, 0, 11 };
ConditionIcon CONDITION_ICON_EVOLVING = { 100, 0, 12 };

int16_t CONDITION_TIRED_FRAME_U[14] = {
	0, 0, 16, 16, 32, 32, 48, 48, 48, 48, 48, 48, 48, 48,
};

int16_t CONDITION_TIRED_FRAME_MY[14] = {
	9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 6, 3, 0, -6,
};

ConditionIcon CONDITION_ICON_SLEEPY[7] = {
	{ 10, 1, 0 },
	{ 4, 1, 4 },
	{ 4, 1, 5 },
	{ 8, 1, 6 },
	{ 1, 1, 7 },
	{ 1, 1, 8 },
	{ 1, -5, 0 },
};

ConditionIcon *CONDITION_BUBBLE_TYPES[8] = {
	&CONDITION_ICON_HUNGRY,
	&CONDITION_ICON_POOPY,
	&CONDITION_ICON_SICK,
	CONDITION_ICON_SLEEPY,
	&CONDITION_ICON_TIRED,
	&CONDITION_ICON_DYING,
	&CONDITION_ICON_INJURED,
	&CONDITION_ICON_EVOLVING,
};

ConditionBubble CONDITION_BUBBLES[3];

void initializeConditionBubbles(void)
{
	setInt16WithStride((int16_t *)CONDITION_BUBBLES, -1,
			   NUM_CONDITION_BUBBLES, sizeof(ConditionBubble));
}

int32_t addConditionBubble(int32_t type, Entity *entity)
{
	ConditionBubble *bubble;
	int32_t i;

	for (i = 0; i < NUM_CONDITION_BUBBLES; ++i) {
		if (CONDITION_BUBBLES[i].frameCount < 0) {
			break;
		}
	}

	if (i == NUM_CONDITION_BUBBLES) {
		return -1;
	}

	bubble = &CONDITION_BUBBLES[i];
	bubble->frameCount = 0;
	bubble->baseHeight = -DIGIMON_DATA[entity->type].height + 20;
	bubble->activeHeight = bubble->baseHeight;
	bubble->entity = entity;
	bubble->scale = 0;
	bubble->scale2 = 0;
	bubble->frameCount2 = 0;
	bubble->iconPtr = CONDITION_BUBBLE_TYPES[type];
	addObject(0x802, i, tickConditionBubble, renderConditionBubble);

	return i;
}

void renderConditionBubble(int32_t instanceId)
{
	ConditionBubble *bubble;
	SVECTOR pos;
	DVECTOR screen;
	int32_t icon;
	GsSPRITE **sprites;
	int32_t top;
	int32_t value;
	uint32_t scale;
	int32_t depth;

	bubble = &CONDITION_BUBBLES[instanceId];

	if (IS_IN_MENU == 1) {
		return;
	}

	translateConditionFXToEntity(bubble->entity, &pos);
	pos.vy = bubble->activeHeight + bubble->entity->posData->location.vy;
	depth = worldPosToScreenPos(&pos, &screen);
	scale = bubble->scale;

	if (bubble->entity->isOnScreen != 0) {
		if (bubble->frameCount >= 51) {
			top = 8;
		} else {
			top = 32;
		}
		top = (top * ((scale * VIEWPORT_DISTANCE) / depth)) >> 12;

		if ((screen.vy + DRAWING_OFFSET_Y) < top) {
			screen.vy = -DRAWING_OFFSET_Y + top;
		}

		value = (depth >> 4) - 20;
		if (value < 36) {
			value = 36;
		}
		scale = ((int32_t)scale * value) / (depth >> 4);
		depth = value << 4;
	}

	if ((depth <= 0x200) || (depth >= 0x10000)) {
		return;
	}

	if (bubble->frameCount >= 51) {
		renderSprite(&CONDITION_SPRITES[10], screen.vx, screen.vy,
			     depth, scale, scale);
		return;
	}

	icon = bubble->iconPtr->icon;

	switch (icon) {
	case 9: {
		int32_t tiredIndex = bubble->frameCount % 13;

		CONDITION_SPRITES[6].u = CONDITION_TIRED_FRAME_U[tiredIndex] + 0xc0;
		CONDITION_SPRITES[6].my = CONDITION_TIRED_FRAME_MY[tiredIndex] + 0x17;
		renderSprite(&CONDITION_SPRITES[6], screen.vx, screen.vy,
			     depth, scale, scale);
		break;
	}
	case 1: {
		int32_t hungryIndex = bubble->frameCount % 19;
		ConditionFramesHungry frames = CONDITION_HUNGRY_FRAMES;

		CONDITION_SPRITES[0].u = (frames.frame[hungryIndex] << 4) + 0x90;
		if (frames.frame[hungryIndex] != 3) {
			CONDITION_SPRITES[0].v = 0x80;
		} else {
			CONDITION_SPRITES[0].v = 0xb0;
		}
		CONDITION_SPRITES[0].my = 0x1a;
		if (((hungryIndex + 1) % 5) == 0) {
			CONDITION_SPRITES[0].my += 3;
		}
		renderSprite(&CONDITION_SPRITES[0], screen.vx, screen.vy,
			     depth, scale, scale);
		break;
	}
	case 2: {
		int32_t poopyIndex = bubble->frameCount % 13;
		ConditionFramesPoopy frames = CONDITION_POOPY_FRAMES;

		CONDITION_SPRITES[1].u = (frames.frame[poopyIndex] << 4) + 0xd0;
		renderSprite(&CONDITION_SPRITES[1], screen.vx, screen.vy,
			     depth, scale, scale);
		break;
	}
	case 3:
		CONDITION_SPRITES[2].u = (((bubble->frameCount / 15) & 1) << 4) + 0xe0;
		renderSprite(&CONDITION_SPRITES[2], screen.vx, screen.vy,
			     depth, scale, scale);
		break;
	case 10:
		CONDITION_SPRITES[7].v = (((bubble->frameCount / 20) & 1) << 4) + 0x90;
		renderSprite(&CONDITION_SPRITES[7], screen.vx, screen.vy,
			     depth, scale, scale);
		break;
	case 11: {
		int32_t injuredIndex = bubble->frameCount % 20;
		ConditionFramesInjured frames = CONDITION_INJURED_FRAMES;

		CONDITION_SPRITES[8].u = (frames.frame[injuredIndex] << 4) + 0xd0;
		renderSprite(&CONDITION_SPRITES[8], screen.vx, screen.vy,
			     depth, scale, scale);
		break;
	}
	case 12: {
		int32_t evolvingIndex = bubble->frameCount % 19;
		ConditionFramesEvolving frames = CONDITION_EVOLVING_FRAMES;

		CONDITION_SPRITES[9].u = (frames.frame[evolvingIndex] << 4) + 0x80;
		if (frames.frame[evolvingIndex] == 0) {
			CONDITION_SPRITES[9].v = 0xb8;
			CONDITION_SPRITES[9].my = 0x12;
			CONDITION_SPRITES[9].h = 8;
		} else {
			CONDITION_SPRITES[9].v = 0xb0;
			CONDITION_SPRITES[9].my = 0x1a;
			CONDITION_SPRITES[9].h = 0x10;
		}
		renderSprite(&CONDITION_SPRITES[9], screen.vx, screen.vy,
			     depth, scale, scale);
		break;
	}
	default:
		sprites = CONDITION_SPRITES_ICON[icon];
		while (*sprites != NULL) {
			renderSprite(*sprites, screen.vx, screen.vy, depth,
				     scale, scale);
			++sprites;
		}
		break;
	}

	renderSprite(&CONDITION_SPRITES[11], screen.vx, screen.vy, depth,
		     scale, scale);
}

void tickConditionBubble(int32_t instanceId)
{
	ConditionBubble *bubble;
	int32_t angle;
	int32_t sine;

	bubble = &CONDITION_BUBBLES[instanceId];

	if (IS_IN_MENU == 1) {
		return;
	}

	if (bubble->frameCount >= 50) {
		if (bubble->frameCount > 50) {
			removeConditionBubble(instanceId);
			return;
		}
		bubble->activeHeight = bubble->baseHeight - 230;
		bubble->scale = 0xf000;
		bubble->scale2 = 0xf000;
	} else {
		if (bubble->frameCount < 10) {
			bubble->scale = lerp(0, 0x6000, 0, 10,
					     bubble->frameCount);
			bubble->scale2 = bubble->scale;
			bubble->activeHeight =
				bubble->baseHeight +
				lerp(0, -65, 0, 10, bubble->frameCount - 10);
		} else {
			bubble->scale = 0x6000;
			bubble->scale2 = 0x6000;
			bubble->activeHeight = bubble->baseHeight - 65;
			angle = lerp(0, 512, 0, 20, bubble->frameCount);
			sine = _sin(angle);
			bubble->activeHeight += (sine * 12) >> 12;
		}
	}

	if (bubble->frameCount2 >= bubble->iconPtr->duration) {
		bubble->frameCount2 = 0;
		bubble->iconPtr += bubble->iconPtr->nextIcon;
	}

	++bubble->frameCount2;
	++bubble->frameCount;
}

void removeConditionBubble(int32_t instanceId)
{
	if ((instanceId >= 0) &&
	    (instanceId < NUM_CONDITION_BUBBLES) &&
	    (CONDITION_BUBBLES[instanceId].frameCount >= 0)) {
		CONDITION_BUBBLES[instanceId].frameCount = -1;
		removeObject(0x802, instanceId);
	}
}
