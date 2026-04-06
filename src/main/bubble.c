#include <dw/bubble.h>
#include <dw/params.h>
#include <dw/world_object.h>
#include <dw/math.h>

#include "common.h"


extern int32_t VIEWPORT_DISTANCE;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t IS_IN_MENU;

typedef struct {
	int16_t duration;
	int16_t nextIcon;
	int16_t icon;
} ConditionIcon;

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

extern ConditionBubble CONDITION_BUBBLES[];

extern GsSPRITE CONDITION_SPRITE_0;
extern GsSPRITE CONDITION_SPRITE_1;
extern GsSPRITE CONDITION_SPRITE_2;
extern GsSPRITE CONDITION_SPRITE_3;
extern GsSPRITE CONDITION_SPRITE_4;
extern GsSPRITE CONDITION_SPRITE_5;
extern GsSPRITE CONDITION_SPRITE_6;
extern GsSPRITE CONDITION_SPRITE_7;
extern GsSPRITE CONDITION_SPRITE_8;
extern GsSPRITE CONDITION_SPRITE_9;
extern GsSPRITE CONDITION_SPRITE_10;
extern GsSPRITE CONDITION_SPRITE_11;

extern GsSPRITE **CONDITION_SPRITE_TABLE[];

extern char CONDITION_SPRITE_0_DATA[];
extern char CONDITION_SPRITE_1_DATA[];
extern int16_t CONDITION_SPRITE_6_DATA_U[];
extern int16_t CONDITION_SPRITE_6_DATA_MY[];
extern char CONDITION_SPRITE_8_DATA[];
extern char CONDITION_SPRITE_9_DATA[];

extern ConditionIcon *CONDITION_ICON_TABLE[];

void setShortWithStride(int16_t* ptr, int16_t value, int32_t count,
			int32_t stride);
int32_t lerp(int32_t start, int32_t end, int32_t tMin, int32_t tMax,
	     int32_t tCurrent);
int32_t worldPosToScreenPos(SVECTOR* worldPos, DVECTOR* screenPos);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
void renderSprite(GsSPRITE* sprite, int32_t x, int32_t y, int32_t distance,
		  int32_t scaleX, int32_t scaleY);

void initializeConditionBubbles(void)
{
	setShortWithStride((int16_t *)CONDITION_BUBBLES, -1, 3,
			   sizeof(ConditionBubble));
}

int32_t addConditionBubble(int32_t type, Entity *entity)
{
	ConditionBubble *bubble;
	int32_t i;

	for (i = 0; i < 3; ++i) {
		if (CONDITION_BUBBLES[i].frameCount < 0) {
			break;
		}
	}

	if (i == 3) {
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
	bubble->iconPtr = CONDITION_ICON_TABLE[type];
	addObject(0x802, i, tickConditionBubble, renderConditionBubble);

	return i;
}

INCLUDE_ASM("asm/main/nonmatchings/bubble", renderConditionBubble);

void tickConditionBubble(int32_t instanceId)
{
	int32_t value;
	ConditionBubble *bubble;

	bubble = &CONDITION_BUBBLES[instanceId];

	if (IS_IN_MENU == 1) {
		return;
	}

	if (bubble->frameCount >= 50) {
		if (bubble->frameCount > 50) {
			unsetBubble(instanceId);
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
			value = lerp(0, -65, 0, 10, bubble->frameCount - 10);
			bubble->activeHeight =
				bubble->baseHeight + value;
		} else {
			bubble->scale = 0x6000;
			bubble->scale2 = 0x6000;
			bubble->activeHeight = bubble->baseHeight - 65;
			value = lerp(0, 512, 0, 20, bubble->frameCount);
			value = _sin(value);
			bubble->activeHeight += (value * 12) >> 12;
		}
	}

	if (bubble->frameCount2 >= bubble->iconPtr->duration) {
		bubble->frameCount2 = 0;
		bubble->iconPtr += bubble->iconPtr->nextIcon;
	}

	++bubble->frameCount2;
	++bubble->frameCount;
}

void unsetBubble(int32_t instanceId)
{
	if ((instanceId >= 0) &&
	    (instanceId < 3) &&
	    (CONDITION_BUBBLES[instanceId].frameCount >= 0)) {
		CONDITION_BUBBLES[instanceId].frameCount = -1;
		removeObject(0x802, instanceId);
	}
}
