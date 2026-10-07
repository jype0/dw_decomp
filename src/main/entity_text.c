#include <libgs.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/garbage.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

/* A number that pops out of an entity, such as damage or healing. */
typedef struct {
	int32_t value;
	int32_t numDigits;
	int16_t x;
	int16_t y;
	uint8_t frameId; /* 0xFF when the entry is free */
	uint8_t color;
	uint8_t icon; /* 0 for none, else an index + 1 in ENTITY_TEXT_ICON_U */
	uint8_t unk2;
} EntityTextDataEntry;

/* The numbers of one fighter, shown one after another. */
typedef struct {
	int32_t activeElements;
	EntityTextDataEntry entries[8];
	uint8_t activeList[8]; /* entries in the order they show, 0xFF ends */
} EntityTextData;

/* How far a number moves down on each of its first 21 frames. */
int8_t ENTITY_TEXT_Y_OFFSETS[24] = {
	0, -7, -5, -5, -2, -1, 1, 2, 5, 5, 7, -4,
	-3, -2, 0, -1, 1, 0, 2, 3, 0, 0, 0, 0,
};
/* The u of each icon in its texture page. */
uint8_t ENTITY_TEXT_ICON_U[8] = { 0xB0, 0xC8, 0xB8, 0xC0, 0xD0, 0, 0, 0 };

EntityTextData ENTITY_TEXT_DATA[4];

void getEntityScreenPos(Entity *entity, int32_t mode, DVECTOR *out);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
extern int8_t GAME_STATE;
extern GsOT *ACTIVE_ORDERING_TABLE;

void drawEntityText(int32_t color, int32_t n, int32_t x, int32_t y,
			int32_t value, int32_t layer);
void drawEntityTextIcon(int32_t x, int32_t y, int32_t u, int32_t layer);

void initializeEntityText();
void addEntityText(Entity *entity, int16_t slotId, int16_t color, int32_t value, uint8_t icon);
void setCombatTextPosition(Entity *entity, EntityTextDataEntry *entry);
void renderEntityText(int32_t instanceId);
void removeEntityText(int32_t id);

void *entity_text_order_anchor[] = {
	removeEntityText,
	renderEntityText,
	setCombatTextPosition,
	addEntityText,
	initializeEntityText,
};

void initializeEntityText()
{
	int32_t id;
	int32_t i;
	EntityTextData *data;

	for (id = 0; id < 4; id++) {
		data = &ENTITY_TEXT_DATA[id];
		for (i = 0; i < 8; i++) {
			data->entries[i].frameId = 0xFF;
			data->activeList[i] = 0xFF;
		}
		data->activeElements = 0;
	}

	i = i;
}

GARBAGE(addEntityText, 10);

/*
 * Shows value over entity, in the slot of that fighter (0 is the partner).
 * Up to eight numbers per slot rise one after another; icon adds a small
 * sprite before the number.
 */
void addEntityText(Entity *entity, int16_t slotId, int16_t color, int32_t value,
		   uint8_t icon)
{
	int32_t i;
	int32_t digits[4];
	EntityTextData *slot;
	EntityTextDataEntry *entry;
	int32_t j;

	slot = &ENTITY_TEXT_DATA[slotId];
	if (slot->activeElements == 8) {
		return;
	}

	i = 0;
	while (i < 8) {
		if (slot->entries[i].frameId == 0xFF) {
			break;
		}
		i += 1;
	}

	entry = &slot->entries[i];
	convertValueToDigits(4, value, &entry->numDigits, digits);
	entry->value = value;
	entry->color = color;
	entry->icon = icon;
	entry->frameId = 0;
	setCombatTextPosition(entity, entry);
	if (GAME_STATE != 4) {
		entry->x += slot->activeElements * 4;
		entry->y += slot->activeElements * 4;
	} else {
		entry->x = slot->activeElements * 4;
		entry->y = slot->activeElements * 4;
	}

	for (j = 0; j < 8; j++) {
		if (slot->activeList[j] == 0xFF) {
			slot->activeList[j] = i;
			break;
		}
	}
	j = j;

	if (slot->activeElements == 0) {
		addObject(0x192, slotId, 0, renderEntityText);
	}
	slot->activeElements += 1;
}

void setCombatTextPosition(Entity *entity, EntityTextDataEntry *entry)
{
	DVECTOR xy;

	getEntityScreenPos(entity, 0, &xy);
	entry->x = xy.vx;
	entry->y = xy.vy - 8;
}

void renderEntityText(instanceId)
	int16_t instanceId;
{
	GsOT_TAG *ot;
	DVECTOR screenPos;
	uint8_t *frame;
	EntityTextData *slot;
	EntityTextDataEntry *entry;
	long i;

	ot = ACTIVE_ORDERING_TABLE->org;
	slot = &ENTITY_TEXT_DATA[instanceId];
	i = 0;
	while (i < 8) {
		if (slot->activeList[i] == 0xFF) {
			break;
		}
		entry = &slot->entries[slot->activeList[i]];
		frame = &entry->frameId;
		if (*frame == 0xFF) {
			break;
		}
		/* Each number waits for the one before it to rise a bit. */
		if (i != 0 && slot->entries[slot->activeList[i - 1]].frameId < 11) {
			break;
		}
		if (*frame < 21) {
			entry->y += ENTITY_TEXT_Y_OFFSETS[*frame];
		}

		/* In battle, the number follows its fighter. */
		if (GAME_STATE == 4) {
			getEntityScreenPos(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[instanceId]],
					   0, &screenPos);
			drawEntityText(entry->color, entry->numDigits,
				       screenPos.vx + entry->x,
				       screenPos.vy - 8 + entry->y,
				       entry->value, 14 - i);
		} else {
			drawEntityText(entry->color, entry->numDigits,
				       entry->x, entry->y,
				       entry->value, 14 - i);
		}

		if (entry->icon != 0) {
			if (GAME_STATE != 4) {
				drawEntityTextIcon(entry->x - 8, entry->y,
						   ENTITY_TEXT_ICON_U[entry->icon - 1],
						   14 - i);
			} else {
				drawEntityTextIcon(screenPos.vx + entry->x - 8,
						   screenPos.vy - 8 + entry->y,
						   ENTITY_TEXT_ICON_U[entry->icon - 1],
						   14 - i);
			}
		}

		*frame += 1;
		if (*frame >= 31) {
			entry->frameId = 0xFF;
			slot->activeList[i] = 0xFF;
		}
		i++;
	}

	/* Once the first number is gone, move the others up. */
	if (slot->activeList[0] == 0xFF) {
		for (i = 1; i < 8; i++) {
			if (slot->activeList[i] != 0xFF) {
				slot->activeList[i - 1] = slot->activeList[i];
			} else {
				slot->activeList[i - 1] = 0xFF;
				break;
			}
		}
		slot->activeList[7] = 0xFF;
	}

	for (i = 0; i < 8; i++) {
		if (slot->activeList[i] != 0xFF) {
			break;
		}
	}
	if (i == 8) {
		removeEntityText(instanceId);
	}
}

void removeEntityText(int32_t id)
{
	EntityTextData *slot;
	int32_t i;

	slot = &ENTITY_TEXT_DATA[id];
	if (slot->activeElements != 0) {
		removeObject(0x192, id);
		slot->activeElements = 0;
		i = 0;
		while (i < 8) {
			slot->entries[i].frameId = 0xFF;
			slot->activeList[i] = 0xFF;
			i += 1;
		}
	}
}
