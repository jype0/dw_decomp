#include <libgte.h>
#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>
#include <libcd.h>

typedef struct {
	int16_t state;
	int16_t unk2;
	int16_t unk4;
} CloudFXEntry;

extern int16_t MAIN_D_80138AA4[];
extern int16_t MAIN_D_801389B4[];
extern char *EFE_FLASH_DATA;
extern uint32_t MAIN_D_8012343C[];
extern char MAIN_D_8012342C[];
extern char MAIN_D_80134220[4];
extern int16_t EFE_LOADED_MOVE_DATA[];
extern char EFE_SCRIPT_MEM1_DATA[];
extern char *EFE_DATA_STACK;
extern u_long SOME_IMAGE_DATA[];
void setShortWithStride();
void removeObject(int32_t objectId, int32_t instanceId);
char *strcpy(char *dst, char *src);
char *strcat(char *dst, char *src);
int32_t rand(void);
extern uint8_t MAIN_D_801387B8[];
typedef struct {
	int16_t v;
	int16_t pad1;
	int16_t pad2;
} EfeParticleField;
extern EfeParticleField MAIN_D_80138888[];
extern EfeParticleField MAIN_D_8013888A[];
extern EfeParticleField MAIN_D_8013888C[];
int32_t _sin(int32_t a);
extern int32_t FLASH_INSTANCE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
void getDrawingOffsetCopy(int32_t *x, int32_t *y);
int32_t isTamerOnScreen(void);
extern uint8_t MAIN_D_8013421C[];
extern int32_t VIEWPORT_DISTANCE;
int32_t lerp(int16_t a, int16_t b, int32_t c, int16_t d, int32_t e);
void renderParticleFlash(int16_t *params);
typedef struct {
	int16_t x;
	int16_t y;
	int16_t depth;
	uint32_t size;
	int16_t w;
	int16_t h;
	int16_t tpage;
	uint8_t clut;
	int8_t attr;
	int16_t clut2;
	int8_t r;
	int8_t g;
	int8_t b;
	int8_t a;
} FlashParams;
int32_t _cos(int32_t a);
extern GsSPRITE CLOUD_FX_SPRITE;
extern int8_t MAIN_D_801233CC[];
extern int16_t MAIN_D_801233DC[];
extern int16_t MAIN_D_80123400[];
extern uint8_t MAIN_D_8012341C[];
int32_t worldPosToScreenPos(int16_t *world, int16_t *screen);
void renderSprite(GsSPRITE *spr, int16_t x, int16_t y, int32_t depth,
		  int32_t sx, int32_t sy);
typedef void (*EfeFn)(int32_t);
int32_t addObject(int32_t objectId, int32_t instanceId, EfeFn tick, EfeFn render);

#include "common.h"

void initializeParticleFX();
void createParticleFX();
void tickParticleFX(int32_t id);
void renderParticleFX();
int32_t addEntityParticleFX(int32_t *typePtr, int16_t timer);
void initializeEntityParticleFX();
void tickEntityParticleFX(int32_t id);
void renderEntityParticleFX(int32_t id);
void removeEntityParticleFX();
void initializeCloudFXData();
void removeAllCloudFX();
void createCloudFX(int16_t *pos);
void tickCloudFX();
void renderCloudFX(int32_t id);
void rotateVector();
char *initializeFlashData(char *base);
static inline int16_t *efe_s16ptr(char *arg0)
{
	return (int16_t *)arg0;
}
void createFlash();
void tickEFEFlash();
void renderEFEFlash(int32_t layer);
volatile unsigned long setEFEFlashOffset(int32_t id, int16_t x, int16_t y);
void downloadSomeImage();
void modifySomeImage(int32_t dim, int16_t stp);
void findEFEDATFile(void);
void initializeEFE();
void getEFEDATEntry();
void renderParticleFlash();

void *efe_order_anchor[] = {
	renderParticleFlash,
	getEFEDATEntry,
	initializeEFE,
	findEFEDATFile,
	modifySomeImage,
	downloadSomeImage,
	setEFEFlashOffset,
	renderEFEFlash,
	tickEFEFlash,
	createFlash,
	initializeFlashData,
	rotateVector,
	renderCloudFX,
	tickCloudFX,
	createCloudFX,
	removeAllCloudFX,
	initializeCloudFXData,
	removeEntityParticleFX,
	renderEntityParticleFX,
	tickEntityParticleFX,
	initializeEntityParticleFX,
	addEntityParticleFX,
	renderParticleFX,
	tickParticleFX,
	createParticleFX,
	initializeParticleFX,
};

void initializeParticleFX(void)
{
	int32_t i;
	SVECTOR ang;
	SVECTOR vec;
	MATRIX m;

	setShortWithStride(MAIN_D_801387B8, -1, 4, 0x34);
	for (i = 0; i < 0x32; i++) {
		ang.vx = (((rand() & 0x7F) - 0x40) << 12) / 64;
		ang.vy = (((rand() & 0x7F) - 0x40) << 12) / 64;
		ang.vz = (((rand() & 0x7F) - 0x40) << 12) / 64;
		vec.vy = 0;
		vec.vx = 0;
		vec.vz = ((rand() % 150) + 0xFA) * 0x10;
		RotMatrixZYX(&ang, &m);
		ApplyMatrixSV(&m, &vec, &vec);
		MAIN_D_80138888[i].v = vec.vx;
		MAIN_D_8013888A[i].v = vec.vy;
		MAIN_D_8013888C[i].v = vec.vz;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/efe", createParticleFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", tickParticleFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", renderParticleFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", addEntityParticleFX);

void initializeEntityParticleFX(void)
{
	setShortWithStride(MAIN_D_801389B4, -1, 0x14, 0xC);
}

void tickEntityParticleFX(int32_t id)
{
	char *e;
	int32_t bc;

	e = (char *)&MAIN_D_801389B4[id * 6];
	if (*(int16_t *)e <= 0) {
		removeEntityParticleFX(id);
		return;
	}
	if ((*(int16_t *)e % 4) == 0) {
		bc = DIGIMON_DATA[**(int32_t **)(e + 4)].boneCount - 1;
		*(int16_t *)(e + 2) = (rand() % bc) + 1;
		*(int16_t *)(e + 8) = rand() % 9;
	}
	*(int16_t *)e = *(int16_t *)e - 1;
}

INCLUDE_ASM("asm/main/nonmatchings/efe", renderEntityParticleFX);

void removeEntityParticleFX(int32_t id)
{
	MAIN_D_801389B4[id * 6] = -1;
	removeObject(0x502, id);
}

void initializeCloudFXData(void)
{
	setShortWithStride(MAIN_D_80138AA4, -1, 0x3C, 6);
}

void removeAllCloudFX(void)
{
	int32_t i;

	for (i = 0; i < 0x3C; i++) {
		if (((CloudFXEntry *)MAIN_D_80138AA4)[i].state != -1) {
			((CloudFXEntry *)MAIN_D_80138AA4)[i].state = -1;
			removeObject(0x601, i);
		}
	}
}

void createCloudFX(int16_t *pos)
{
	CloudFXEntry *e;
	int32_t i;

	for (i = 0; i < 0x3C; i++) {
		if (((CloudFXEntry *)MAIN_D_80138AA4)[i].state < 0) {
			break;
		}
	}
	if (i != 0x3C) {
		e = &((CloudFXEntry *)MAIN_D_80138AA4)[i];
		e->state = 0;
		e->unk2 = pos[0];
		e->unk4 = pos[2];
		addObject(0x601, i, tickCloudFX, renderCloudFX);
	}
}

void tickCloudFX(int32_t id)
{
	int16_t *p;

	p = &MAIN_D_80138AA4[id * 3];
	*p += 1;
	if (*p >= 0xE) {
		*p = -1;
		removeObject(0x601, id);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/efe", renderCloudFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", rotateVector);

char *initializeFlashData(char *base)
{
	int32_t i;
	char *new_var;
	int32_t ofs;

	EFE_FLASH_DATA = base;
	i = 0;
	ofs = 0;
	while (i < 0xC) {
		new_var = EFE_FLASH_DATA;
		*efe_s16ptr((ofs + new_var) + 8) = -1;
		i += 1;
		ofs += 0x28;
		do { } while (0);
	}
	return base + 0x1E0;
}

INCLUDE_ASM("asm/main/nonmatchings/efe", createFlash);

void tickEFEFlash(int32_t id)
{
	char *e;

	e = EFE_FLASH_DATA + (id * 0x28);
	((int16_t *)e)[4] = ((int16_t *)e)[4] + 1;
	if (((int16_t *)e)[4] >= ((int16_t *)e)[5]) {
		((int16_t *)e)[4] = -1;
		removeObject(0x602, id);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/efe", renderEFEFlash);

volatile unsigned long setEFEFlashOffset(int32_t id, int16_t x, int16_t y)
{
	char *e;
	char *new_var;

	new_var = EFE_FLASH_DATA + (id *= 0x28);
	e = new_var;
	((int16_t *)e)[18] = x;
	((int16_t *)e)[19] = y;
}

void downloadSomeImage(void)
{
	RECT r;

	r.x = 0x200;
	r.y = 0xF8;
	r.w = 0x100;
	r.h = 7;
	StoreImage(&r, SOME_IMAGE_DATA);
	DrawSync(0);
}

INCLUDE_ASM("asm/main/nonmatchings/efe", modifySomeImage);

void findEFEDATFile(void)
{
	char name[0x40];
	CdlFILE file;
	uint8_t mode;
	int32_t i;

	i = 0;
	while (CdReadSync(1, 0) != 0) {}
	mode = 0x80;
	name[0] = 0x5C;
	strcpy(&name[1], MAIN_D_8012342C);
	strcat(name, MAIN_D_80134220);
	while ((int32_t)CdSearchFile(&file, name) == -1) {}
	CdControl(0xE, &mode, 0);
	MAIN_D_8012343C[i] = CdPosToInt(&file.pos);
}

void initializeEFE(void)
{
	setShortWithStride(EFE_LOADED_MOVE_DATA, -1, 0x11, 2);
	EFE_DATA_STACK = EFE_SCRIPT_MEM1_DATA;
	findEFEDATFile();
}

void getEFEDATEntry(int32_t id)
{
	CdlLOC loc;

	id -= 0x100;
	CdIntToPos(MAIN_D_8012343C[0] + (id * 0xA), &loc);
}

INCLUDE_ASM("asm/main/nonmatchings/efe", renderParticleFlash);

