#ifndef DW_ENTITY_H
#define DW_ENTITY_H

#include <libgte.h>
#include <libgs.h>

#include <dw/stats.h>
#include <dw/types.h>

#define ENTITY_MAX		10

typedef struct
{
	GsDOBJ2 obj;
	GsCOORDINATE2 posMatrix;
	VECTOR scale;
	SVECTOR rotation;
	VECTOR location;
} PositionData;

typedef struct
{
	int16_t scale1[9];
	int16_t subDelta[9];
	int16_t delta[9];
	int16_t subScale[9];
	char subValue[9];
	uint8_t field5_0x51;
} MomentumData;

typedef struct {
	MomentumData *momentum;
	int32_t locX;
	int32_t locY;
	int32_t locZ;
	int16_t animFrame;
	int16_t frameCount;
	int16_t *animInstrPtr;
	void *loopStart;
	int16_t loopEndFrame;
	int16_t textureX;
	int16_t textureY;
	uint8_t animId;
	uint8_t loopCount;
	uint8_t animFlag;
	uint8_t field17_0x31;
	int16_t field18_0x32;
} EntityAnim;

typedef struct {
	int32_t type;
	PositionData *posData;
	int32_t *animPtr;
	EntityAnim anim;
	uint8_t isOnMap;
	int8_t isOnScreen;
	uint8_t flatSprite;
	uint8_t flatTimer;
} Entity;

typedef struct {
	Entity entity;
	Stats stats;
	int16_t unk1;
	uint8_t unk2_1;
	uint8_t unk2_2;
	int16_t vabId;
	uint8_t chargeMode;
	uint8_t field7_0x57;
} DigimonEntity;

typedef struct {
	DigimonEntity digimonEntity;
	uint32_t learnedMoves[2];
	uint32_t unk1;
	uint32_t unk2;
	uint8_t name[20];
	int32_t lives;
} PartnerEntity;

typedef struct {
	Entity entity;
	uint8_t tamerLevel;
	uint8_t raisedCount;
	uint16_t padding;
} TamerEntity;

extern Entity *ENTITY_TABLE[ENTITY_MAX];
extern PartnerEntity PARTNER_ENTITY;
extern TamerEntity TAMER_ENTITY;

#endif
