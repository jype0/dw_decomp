#ifndef DW_EFE_H
#define DW_EFE_H

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/model.h>
#include <dw/types.h>
#include <dw/version.h>

#define EFE_POP(ptr, type) ((type)*--(ptr))
#define EFE_PUSH(ptr, type, value) (*(ptr)++ = (int32_t)(type)(value))

#define EFE_POP1(type) EFE_POP(EFE_DATA_STACK, type)
#define EFE_PUSH1(type, value) EFE_PUSH(EFE_DATA_STACK, type, value)
#define EFE_DROP1() (--EFE_DATA_STACK)

#define EFE_POP2(type) EFE_POP(EFE_CALL_STACK, type)
#define EFE_PUSH2(type, value) EFE_PUSH(EFE_CALL_STACK, type, value)

#define EFE_SCRATCH ((EfeScratch *)getScratchAddr(0))
#define EFE_SPRITE_SCRATCH ((EfeSpriteScratch *)getScratchAddr(0))
#define EFE_PROJ_SCRATCH ((EfeProjScratch *)getScratchAddr(42))
#define EFE_WAVE_SCRATCH ((EfeWaveScratch *)getScratchAddr(34))
#define EFE_RIBBON_SCRATCH ((EfeRibbonScratch *)getScratchAddr(42))

#define EFE_SORT_WORKSPACE getScratchAddr(44)

typedef struct {
	int32_t vx;
	int32_t vy;
	int32_t vz;
} EfeVector;

typedef struct {
	int32_t red;
	int32_t green;
	int32_t blue;
} EfeColor;

typedef struct {
	EfeVector position;
	EfeVector rotation;
} EfeTransform;

typedef struct {
	int32_t frame;
	EfeTransform transform;
} EfeInstance;

typedef struct {
	int16_t boneId;
	int16_t positionX;
	int16_t positionY;
	int16_t positionZ;
} EfeBoneOffset;

typedef struct {
	int16_t type;
	EfeBoneOffset offset;
} EfeTechBoneOffset;

typedef struct {
	int16_t frame;
	int16_t angle;
	int16_t spin;
	int16_t offsets[3];
	Entity *entity;
} EfeConfusion;

typedef struct {
	int16_t frame;
	int16_t duration;
	int16_t state;
	int16_t pad;
	Entity *entity;
} EfeStun;

typedef struct {
	int16_t frame;
	int16_t pad;
	Entity *entity;
} EfeStunSpark;

typedef struct {
	int16_t frame;
	int16_t pad;
	Entity *entity;
} EfePoison;

typedef struct {
	int16_t frame;
	int16_t pad;
	int16_t offsetX;
	int16_t offsetZ;
	Entity *entity;
} EfePoisonBubble;

typedef struct {
	int16_t distance;
	int16_t velocity;
	SVECTOR direction;
	SVECTOR position;
} EfeParticle;

typedef struct {
	EfeInstance *transform;
	int16_t frames;
	int16_t startOffset;
	int16_t startVelocity;
	int16_t acceleration;
	struct {
		int8_t r, g, b;
	} color;
	int8_t type;
	EfeParticle particles[21];
} EfeParticleEffect;

typedef struct {
	int16_t *inst;
	int16_t *someInst;
	EfeInstance *instance;
	EfeInstance *parentInstance;
	Entity *sourceEntity;
	Entity *targetEntity;
	EfeBoneOffset *boneOffset;
} EfeSubEffect;

typedef struct {
	int16_t frames;
	int8_t deltaU;
	int8_t deltaV;
} EfeUvKeyframe;

typedef struct {
	int16_t unk0;
	int16_t unk2;
	int16_t unk4;
	int16_t unk6;
	int16_t unk8;
	int16_t unkA;
	int16_t numKeyframes;
	int16_t keyframe;
	int16_t uvFrame;
	int16_t frame;
	int16_t numFrames;
	int16_t pad;
	EfeUvKeyframe *uvData;
	EfeUvKeyframe *uv;
} EfeUvAnim;

typedef struct {
	int32_t startScript;
	int32_t initScript;
	uint32_t uvAnims;
	int32_t uvAnimsEnd;
	uint32_t subEffects;
	int32_t tmdStart;
	int32_t tmdEnd;
	int32_t timStart;
	int32_t timEnd;
	int32_t dataEnd;
	int32_t unk28;
	int32_t numSubEffects;
	int32_t effectId;
} EfeFileHeader;

/* One entry of the EFE slot table at EFE_DATA_PTR. */
typedef struct {
	int32_t effectId;
	int16_t *initScript;
	int32_t data;
	int16_t *startScript;
	EfeSubEffect *subEffects;
	int16_t numSubEffects;
	int16_t pad;
	ModelComponent *model;
	int32_t numObjects;
	EfeUvAnim *uvAnims;
	EfeUvAnim *uvAnimsEnd;
} EfeSlot;

typedef struct {
	int32_t mask;
	EfeSubEffect *context;
} EfeSound;

typedef struct {
	GsDOBJ2 obj;
	SVECTOR rot;
	MATRIX m0;
	MATRIX m1;
	MATRIX m2;
	VECTOR *scale;
	long id;
} EfeScratch;

typedef struct {
	long sxy;
	long otz;
	long p;
	long flag;
	VECTOR *position;
	SVECTOR point;
	GsSPRITE sprite;
} EfeSpriteScratch;

typedef struct {
	long p;
	long flag;
	long otz;
} EfeProjScratch;

typedef struct {
	int32_t ring;
	uint32_t unk8C;
	int32_t halfWidth;
	int32_t height;
	int32_t radius;
	int32_t prevHalfWidth;
	int32_t prevHeight;
	int32_t prevRadius;
	EfeProjScratch proj;
	int32_t phase;
	uint16_t tpage;
	uint16_t clut;
	SVECTOR quad[4];
} EfeWaveScratch;

typedef struct {
	EfeProjScratch proj;
	SVECTOR edge[10][4];
	uint16_t tpage;
	uint16_t clut;
	RGB8 color;
	uint8_t pad0;
	RGB8 colorHalf;
	uint8_t pad1;
	long frame;
	int32_t width;
} EfeRibbonScratch;

typedef struct {
	int16_t type;
	int16_t unk2;
	int16_t unk4;
	int16_t unk6;
	int16_t unk8;
	int16_t u;
} EfeAuraType;

typedef struct {
	int32_t frame;
	SVECTOR position;
	SVECTOR rotation;
	SVECTOR velocity;
	EfeAuraType *type;
	Entity *owner;
} EfeAura;

typedef struct {
	int16_t size;
	int16_t x;
	int16_t y;
	int16_t depth;
	int16_t u;
} EfeParallaxSprite;

typedef struct {
	int16_t frame;
	int16_t duration;
	Entity *entity;
} EfeFinisherAura;

typedef struct {
	int16_t state;
	int16_t pad;
	int8_t *isLoaded;
	int16_t *moves;
	int16_t *effectIds;
	ModelComponent *model;
} EfeLoad;

typedef struct {
	SVECTOR worldPos;
	int16_t progress;
	int16_t tMax;
	int16_t mode;
	int16_t fixedDepth;
	int16_t redMin;
	int16_t greenMin;
	int16_t blueMin;
	int16_t redMax;
	int16_t greenMax;
	int16_t blueMax;
	int32_t scaleMin;
	int32_t scaleMax;
	int16_t offsetX;
	int16_t offsetY;
} EfeFlashData;

typedef struct {
	EfeFlashData data[12];
	int32_t unk1E0;
} EfeFlashBuffer;

typedef struct {
	DVECTOR screenPos;
	int16_t depth;
	int16_t pad;
	int32_t scale;
	uint16_t sizeX;
	uint16_t sizeY;
	uint16_t tpage;
	uint8_t uBase;
	uint8_t vBase;
	uint16_t clut;
	RGB8 color;
	uint8_t colorScale;
} ParticleFlashData;

extern EfeFlashData *EFE_FLASH_DATA;

void removeEntityParticleFX(int32_t id);
void removeAllCloudFX(void);
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
void removeAllParticleFX(void);
void removeEFEFlash(int32_t id);
#endif
extern int32_t *EFE_DATA_STACK;
extern int32_t EFE_SCRIPT_MEM1_DATA[];
extern int16_t EFE_LOADED_MOVE_DATA[];
extern EfeInstance *EFE_CURRENT_DATA_SEGMENT;
extern int32_t EFE_PREVIOUS_DATA_SEGMENT;

extern EfeParticleEffect *EFE_PARTICLE_EMITTERS;
extern int32_t EFE_SUB_EFFECT_INDEX;
extern long COMBAT_EFFECT_ITR;
extern int32_t EFE_HIT_ENTITY_INDEX;
extern long EFE_TARGET_ENTITY_INDEX;
extern int32_t EFE_ACTIVE_SECTION;
extern EfeSubEffect *EFE_SCRIPT_CONTEXT;
extern int32_t EFE_SCRIPT_REGISTER;
extern EfeBoneOffset *EFE_BONE_OFFSET;
extern int32_t *EFE_CALL_STACK;
extern int16_t *EFE_SCRIPT_PTR;
extern int16_t EFE_SCRIPT_CURRENT_VALUE;
extern int32_t EFE_SCRIPT_HEAD;
extern EfeSlot *EFE_DATA_ITERATOR;
extern EfeSlot *EFE_DATA_PTR;
extern char *EFE_HEAP_POINTER;
extern char *EFE_HEAP_BASE;
extern int16_t UNUSED_EFE_ARRAY[];
extern EfeLoad EFE_LOAD_REQUEST;
extern EfeSound EFE_SOUND_DATA[10];
extern GsRVIEW2 EFE_FIXED_VIEW;
extern int32_t EFE_CALL_STACK_BUFFER[16];

#endif
