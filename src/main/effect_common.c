#include <inline_n.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/garbage.h>
#include <dw/graphics.h>
#include <dw/params.h>
#include <dw/tamer.h>
#include <dw/types.h>
#include <dw/vecmath.h>

extern GsOT *ACTIVE_ORDERING_TABLE;
extern Entity *ENTITY_TABLE[];
extern MATRIX GsWSMATRIX;
extern int32_t VIEWPORT_DISTANCE;

int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
int32_t getOriginalType(int32_t type);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void downloadCLUT1(uint32_t *src);
void fadeoutCLUT1(long level, int16_t *clut);
void downloadCLUT2(u_long *buffer);
void fadeoutCLUT2(long fade, int16_t *clut);
int32_t addPolyFT3Prim(POLY_FT3 *prim, int32_t order);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
void renderSprite(GsSPRITE *sprite, int16_t x, int16_t y, int32_t distance, int32_t width, int32_t height);
void addFXPrim(POLY_FT4 *prim, int16_t x, int16_t y, int16_t width, int16_t height, int32_t depth);
void setInt16WithStride(int16_t *dest, int16_t value, int16_t count, int16_t stride);
void renderTMDModel(uint8_t *buffer, int32_t id, GsCOORDINATE2 *coord, GsCOORDINATE2 *super, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
void initializeSomeArenaArrays(int32_t count, int32_t arg1, int32_t arg2, int32_t *out);
int32_t processSomeArenaArrays(int32_t count, int32_t t, int32_t *keys, int32_t *values, int32_t *slopes);
int32_t doSomethingWithSomePoints(int16_t *rect, DVECTOR *line);
int16_t DOOA_storeDigimonY(void);
int16_t DOOA_getStoredDigimonY(void);
void renderFXParticle(SVECTOR *pos, int32_t size, RGB8 *color);

static void *effect_common_functions[] = {
	renderFXParticle,
	DOOA_getStoredDigimonY,
	DOOA_storeDigimonY,
	doSomethingWithSomePoints,
	processSomeArenaArrays,
	initializeSomeArenaArrays,
	renderTMDModel,
	setInt16WithStride,
	addFXPrim,
	renderSprite,
	worldPosToScreenPos,
	addScreenPolyFT4,
	addScreenPolyFT3,
	addPolyFT3Prim,
	fadeoutCLUT2,
	downloadCLUT2,
	fadeoutCLUT1,
	downloadCLUT1,
	calculateBoneMatrix,
	getOriginalType,
	translateConditionFXToEntity,
	lerp,
};

// clang-format off
SVECTOR CONDITION_FX_OFFSETS[177] = {
	{ 0x005a, 0x0014, 0x0000, 0x0003 },
	{ 0x0000, 0xffce, 0x0000, 0x0002 },
	{ 0x0000, 0xffbe, 0x0000, 0x0002 },
	{ 0x0064, 0x0041, 0x0000, 0x0003 },
	{ 0x0043, 0x0000, 0x0067, 0x0007 },
	{ 0xffda, 0xffbc, 0x0000, 0x0004 },
	{ 0x0082, 0xffce, 0x0000, 0x0003 },
	{ 0x000a, 0x0000, 0xffce, 0x0002 },
	{ 0x004b, 0x006e, 0x0000, 0x0003 },
	{ 0x0064, 0x001e, 0x0000, 0x0004 },
	{ 0xffba, 0x0000, 0x0023, 0x0003 },
	{ 0x0000, 0xff74, 0xff9c, 0x0003 },
	{ 0x0000, 0x0032, 0x0000, 0x0004 },
	{ 0x000f, 0xffc4, 0x0000, 0x0001 },
	{ 0x005e, 0x0000, 0x0000, 0x0004 },
	{ 0x0000, 0xffe7, 0x0000, 0x0002 },
	{ 0x0000, 0xff56, 0xffb5, 0x0001 },
	{ 0x0050, 0x0032, 0x0000, 0x0003 },
	{ 0x0000, 0xffba, 0xffb0, 0x0003 },
	{ 0x002d, 0x0069, 0x0000, 0x0003 },
	{ 0x0064, 0x0000, 0x0000, 0x0003 },
	{ 0x0000, 0xffba, 0xffec, 0x0004 },
	{ 0xffd2, 0x0000, 0x002a, 0x0004 },
	{ 0x007e, 0x0000, 0x0000, 0x0003 },
	{ 0xffba, 0x0000, 0x00d2, 0x0002 },
	{ 0x001c, 0x0040, 0x0000, 0x0004 },
	{ 0xffd3, 0xff2e, 0x0000, 0x0004 },
	{ 0xffbf, 0xfff6, 0x0000, 0x000a },
	{ 0x0082, 0x002a, 0x0000, 0x0006 },
	{ 0x0000, 0xff92, 0x0000, 0x0002 },
	{ 0x0000, 0xff6e, 0x0000, 0x0002 },
	{ 0x0019, 0x0000, 0x003c, 0x0003 },
	{ 0x003c, 0x0000, 0x005a, 0x0005 },
	{ 0x0000, 0xff71, 0xff94, 0x0005 },
	{ 0xffdb, 0x001b, 0x0000, 0x0003 },
	{ 0x0041, 0x0082, 0x0000, 0x0004 },
	{ 0x0064, 0x0000, 0x0000, 0x0004 },
	{ 0x0000, 0x0000, 0x0000, 0x0002 },
	{ 0x003c, 0x0000, 0x0078, 0x0003 },
	{ 0x007e, 0x0024, 0xfff5, 0x0004 },
	{ 0x0058, 0x0000, 0x0000, 0x0003 },
	{ 0xffbe, 0x0000, 0x0000, 0x0002 },
	{ 0xffd1, 0x0059, 0x0000, 0x0004 },
	{ 0x0000, 0xffce, 0xffe7, 0x0001 },
	{ 0x0000, 0xffd1, 0x0000, 0x0003 },
	{ 0x0000, 0x0050, 0x0000, 0x0003 },
	{ 0x0046, 0x0000, 0x0000, 0x0005 },
	{ 0x0000, 0xffc4, 0xffb0, 0x0004 },
	{ 0x000a, 0xffa9, 0x0000, 0x0003 },
	{ 0x0000, 0xffc4, 0xffce, 0x0003 },
	{ 0x000a, 0x0000, 0x009b, 0x0004 },
	{ 0x000f, 0x005a, 0x0000, 0x0003 },
	{ 0x0082, 0xffec, 0x0000, 0x0002 },
	{ 0x0064, 0x0000, 0x0000, 0x0002 },
	{ 0x0000, 0x005a, 0x0000, 0x0004 },
	{ 0x0050, 0x0000, 0x0000, 0x0002 },
	{ 0x0000, 0xff9c, 0x000a, 0x0002 },
	{ 0x001e, 0x003a, 0x0000, 0x0003 },
	{ 0xffb5, 0x0000, 0x0000, 0x000a },
	{ 0x0000, 0xffd3, 0xffe7, 0x0004 },
	{ 0x0005, 0x0064, 0x0000, 0x0003 },
	{ 0xffba, 0x0000, 0x0023, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0xffa1, 0x0000, 0x0003 },
	{ 0x0000, 0xffe8, 0xffda, 0x0003 },
	{ 0x0064, 0x0023, 0x0000, 0x0003 },
	{ 0xffd3, 0x000f, 0x0000, 0x0005 },
	{ 0x0000, 0xffa1, 0xffdd, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0064, 0x0023, 0x0000, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x003c, 0x0000, 0x005a, 0x0005 },
	{ 0x0000, 0xff71, 0xff94, 0x0005 },
	{ 0x0000, 0xffb0, 0xffec, 0x0005 },
	{ 0x001c, 0x0040, 0x0000, 0x0004 },
	{ 0x0082, 0xffec, 0x0000, 0x0002 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0xffe9, 0x002d, 0x0000, 0x0004 },
	{ 0x007e, 0x0000, 0x0000, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0043, 0x0000, 0x0067, 0x0007 },
	{ 0x0078, 0x0041, 0x0000, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0046, 0x0000, 0x0000, 0x0005 },
	{ 0x0000, 0xff74, 0xff9c, 0x0003 },
	{ 0x0000, 0xffc4, 0xffb0, 0x0004 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0xffbe, 0x0000, 0x0000, 0x0002 },
	{ 0x0041, 0x0082, 0x0000, 0x0004 },
	{ 0x00a0, 0x001e, 0x0000, 0x0002 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0064, 0x0023, 0x0000, 0x0003 },
	{ 0x000a, 0x0000, 0x009b, 0x0004 },
	{ 0x0019, 0x0000, 0x003c, 0x0003 },
	{ 0xffe9, 0x002d, 0x0000, 0x0004 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0xffe8, 0xffda, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0xffc4, 0x0000, 0x001e, 0x0004 },
	{ 0x0000, 0xffba, 0xffec, 0x0004 },
	{ 0x0000, 0xff8d, 0x0000, 0x0002 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x005a, 0x0000, 0x005a, 0x0002 },
	{ 0xff92, 0xffd8, 0x0000, 0x0004 },
	{ 0x0000, 0xffd8, 0xffe2, 0x0008 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0064, 0x0023, 0x0000, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x002d, 0x0064, 0x0000, 0x0006 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0043, 0x0000, 0x0067, 0x0007 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0046, 0x0000, 0x0000, 0x0005 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

int16_t ORIGINAL_DIGIMON_TYPES[180] = {
	0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0030,
	0x0036, 0x002a, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0003,
	0x0022, 0x0027, 0x0020, 0x0021, 0xffff, 0x0019, 0x0034, 0x0026,
	0xffff, 0x0017, 0x0011, 0x0004, 0xffff, 0x0014, 0x002e, 0x000b,
	0x002f, 0x0022, 0x0029, 0x0023, 0xffff, 0x0039, 0xffff, 0x0032,
	0x001f, 0x0050, 0x0003, 0x0019, 0x0006, 0x0045, 0xffff, 0x0050,
	0x0009, 0x0016, 0x0015, 0xffff, 0x0017, 0xffff, 0xffff, 0xffff,
	0x003d, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b,
	0x000c, 0x000d, 0x000e, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015,
	0x0016, 0x0017, 0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001f,
	0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002a, 0x002d, 0x002e, 0x002f, 0x0030, 0x0031,
	0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x003a,
	0x0039, 0x0042, 0x0005, 0x000c,
};
// clang-format on

int16_t DOOA_STORED_DIGIMON_Y;

int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t)
{
	int32_t result;
	int32_t range;
	int32_t duration;

	if (t1 == t0) {
		return 0;
	}

	range = end - start;
	duration = t1 - t0;
	result = (range * (t - t0)) / duration;
	if (range >= 0) {
		result = result % (range + 1);
		if (result < 0) {
			result += range;
		}
	} else {
		result = result % (range - 1);
		if (result > 0) {
			result += range;
		}
	}

	result += start;
	return result;
}

void translateConditionFXToEntity(Entity *entity, SVECTOR *out)
{
	SVECTOR *offset;
	MATRIX *matrix;

	offset = &CONDITION_FX_OFFSETS[getOriginalType(entity->type)];
	matrix = &entity->posData[offset->pad].posMatrix.workm;
	ApplyMatrixSV(matrix, offset, out);
	out->vx += (int16_t)matrix->t[0];
	out->vy += (int16_t)matrix->t[1];
	out->vz += (int16_t)matrix->t[2];
}

int32_t getOriginalType(int32_t type)
{
	int32_t originalType;

	if ((type < 0) || (type > 0xb0)) {
		return -1;
	}

	originalType = ORIGINAL_DIGIMON_TYPES[type];
	if (originalType < 0) {
		return type;
	}

	return originalType;
}

void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out)
{
	GsCOORDINATE2 *coord;
	GsCOORDINATE2 *bone;

	if (boneId >= DIGIMON_DATA[entity->type].boneCount) {
		boneId = 0;
	}

	coord = &entity->posData->posMatrix;
	bone = &entity->posData[boneId].posMatrix;
	RotMatrix(&entity->posData->rotation, &coord->coord);
	ScaleMatrix(&coord->coord, &entity->posData->scale);
	TransMatrix(&coord->coord, &entity->posData->location);
	calculatePosition(bone, out);
}

void downloadCLUT1(uint32_t *buffer)
{
	RECT rect;

	setRECT(&rect, 0, 480, 256, 7);
	StoreImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

GARBAGE(fadeoutCLUT1, 27);

void fadeoutCLUT1(long level, int16_t *clut)
{
	int16_t buffer[1792];
	RECT rect;
	int16_t *src;
	int16_t *dst;
	int32_t i;
	int16_t red;
	int16_t green;
	int16_t blue;
	int16_t mask;

	src = clut;
	dst = buffer;
	for (i = 0; i < 256; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - level) / 255;
		*dst += (green * (255 - level) / 255) << 5;
		*dst += (blue * (255 - level) / 255) << 10;
		*dst++ += mask << 15;
	}

	src += 768;
	dst += 768;
	for (i = 0; i < 768; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - level) / 255;
		*dst += (green * (255 - level) / 255) << 5;
		*dst += (blue * (255 - level) / 255) << 10;
		*dst++ += mask << 15;
	}

	setRECT(&rect, 0, 480, 256, 7);
	LoadImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void downloadCLUT2(u_long *buffer)
{
	RECT rect;

	setRECT(&rect, 272, 480, 16, 1);
	StoreImage(&rect, buffer);
	setRECT(&rect, 96, 501, 16, 1);
	StoreImage(&rect, buffer + 8);
	setRECT(&rect, 224, 488, 16, 24);
	StoreImage(&rect, buffer + 16);
	DrawSync(0);
}

void fadeoutCLUT2(long fade, int16_t *clut)
{
	int16_t pixels[416];
	RECT rect;
	int16_t *src;
	int16_t *dst;
	int32_t i;
	int16_t red;
	int16_t green;
	int16_t blue;
	int16_t mask;

	src = clut;
	dst = pixels;
	for (i = 0; i < 416; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - fade) / 255;
		*dst += (green * (255 - fade) / 255) << 5;
		*dst += (blue * (255 - fade) / 255) << 10;
		*dst++ += mask << 15;
	}

	setRECT(&rect, 272, 480, 16, 1);
	LoadImage(&rect, (u_long *)&pixels[0]);

	setRECT(&rect, 96, 501, 16, 1);
	LoadImage(&rect, (u_long *)&pixels[16]);

	setRECT(&rect, 224, 488, 16, 24);
	LoadImage(&rect, (u_long *)&pixels[32]);
	DrawSync(0);
}

int32_t addPolyFT3Prim(POLY_FT3 *prim, int32_t order)
{
	if ((order >= 33) && (order < 0x1000)) {
		AddPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}

	return order;
}

void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2)
{
	int32_t otz;
	long p;
	long flag;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	otz = RotTransPers3(v0, v1, v2, (long *)((char *)prim + 8),
			    (long *)((char *)prim + 0x10),
			    (long *)((char *)prim + 0x18),
			    &p, &flag);
	otz = otz >> 2;
	addPolyFT3Prim(prim, otz);
}

int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2,
			SVECTOR *v3)
{
	int32_t otz;
	long p;
	long flag;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	otz = RotTransPers4(v0, v1, v2, v3,
			    (long *)&poly->x0, (long *)&poly->x1,
			    (long *)&poly->x2, (long *)&poly->x3,
			    &p, &flag);
	otz = otz >> 2;
	if ((otz > 0x20) && (otz < 0x1000)) {
		AddPrim(ACTIVE_ORDERING_TABLE->org + otz, poly);
		poly++;
		GsSetWorkBase((PACKET *)poly);
	}
}

int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out)
{
	int32_t otz;

	GsSetLsMatrix(&GsWSMATRIX);
	gte_ldv0(pos);
	gte_rtps();
	gte_stsxy(out);
	gte_stszotz(&otz);
	otz = otz << 2;

	return otz;
}

void renderSprite(GsSPRITE *sprite, int16_t x, int16_t y, int32_t distance,
		  int32_t width, int32_t height)
{
	sprite->x = x;
	sprite->y = y;
	sprite->scalex = ((uint32_t)(width * VIEWPORT_DISTANCE) /
			  (uint32_t)distance);
	sprite->scaley = ((uint32_t)(height * VIEWPORT_DISTANCE) /
			  (uint32_t)distance);
#if VERSION_EQUAL_OR_OLDER(JP_TRIAL)
	GsSortSprite(sprite, ACTIVE_ORDERING_TABLE, distance >> 4);
#else
	distance = distance >> 4;
	if ((distance >= 0) && (distance < 0x1000)) {
		GsSortSprite(sprite, ACTIVE_ORDERING_TABLE, distance);
	}
#endif
}

void addFXPrim(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width,
	       int16_t height, int32_t distance)
{
	int32_t scaledWidth;
	int32_t scaledHeight;

	scaledWidth = ((uint32_t)(width * VIEWPORT_DISTANCE) /
		       (uint32_t)distance);
	posX -= scaledWidth >> 1;
	scaledHeight = ((uint32_t)(height * VIEWPORT_DISTANCE) /
			(uint32_t)distance);
	posY -= scaledHeight >> 1;
	prim->x0 = posX;
	prim->y0 = posY;
	prim->x1 = posX + scaledWidth;
	prim->y1 = posY;
	prim->x2 = posX;
	prim->y2 = posY + scaledHeight;
	prim->x3 = posX + scaledWidth;
	prim->y3 = posY + scaledHeight;
	distance >>= 4;
	distance -= 0x37;

	if ((distance > 0x20) && (distance < 0x1000)) {
		AddPrim(ACTIVE_ORDERING_TABLE->org + distance, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void setInt16WithStride(int16_t *dest, int16_t value, int16_t count,
			int16_t stride)
{
	int16_t i;

	for (i = 0; i < count; i++) {
		*dest = value;
		dest = (int16_t *)((int32_t)dest + stride);
	}
}

// clang-format off
void renderTMDModel(buffer, id, coord, super, trans, rot, scale)
	uint8_t *buffer;
	int16_t id;
	GsCOORDINATE2 *coord;
	GsCOORDINATE2 *super;
	VECTOR *trans;
	SVECTOR *rot;
	VECTOR *scale;
// clang-format on
{
	GsDOBJ2 obj;
	MATRIX m;

	GsLinkObject4((unsigned long)&buffer[12], &obj, id);
	GsInitCoordinate2(super, coord);

	obj.attribute = 0;
	obj.coord2 = coord;

	RotMatrix(rot, &coord->coord);
	ScaleMatrix(&coord->coord, scale);
	TransMatrix(&coord->coord, trans);
	coord->flg = 0;

	GsGetLw(obj.coord2, &m);
	GsSetLightMatrix(&m);

	GsGetLs(obj.coord2, &m);
	GsSetLsMatrix(&m);

	GsSortObject4(&obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
}

void initializeSomeArenaArrays(int32_t count, int32_t arg1, int32_t arg2,
			int32_t *out)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		out[i] = 0;
	}
}

int32_t processSomeArenaArrays(int32_t count, int32_t t, int32_t *keys,
			   int32_t *values, int32_t *slopes)
{
	uint32_t off;
	int32_t *key;
	int32_t *value;
	int32_t hi;
	int32_t mid;
	int32_t x0;
	int32_t dt;
	int32_t dx;
	int32_t *slope;
	int32_t lo;
	int32_t m0;
	int32_t m1;

	lo = 0;
	hi = count - 1;
	while (lo < hi) {
		mid = (lo + hi) / 2;
		if (keys[mid] < t) {
			lo = mid + 1;
		} else {
			hi = mid;
		}
	}

	if (lo > 0) {
		lo--;
	}

#if !VERSION_IS(US)
	dx = keys[lo + 1] - keys[lo];
	dt = t - keys[lo];

	return values[lo] +
	       dt * (((values[lo + 1] - values[lo]) / dx -
		      dx * (slopes[lo + 1] + slopes[lo] * 2)) +
		     dt * (slopes[lo] * 3 +
			   (dt * (slopes[lo + 1] - slopes[lo])) / dx));
#else
	off = lo * 4;
	key = (int32_t *)(off + (uint32_t)keys);
	x0 = key[0];
	dx = key[1] - key[0];
	dt = t - x0;
	value = (int32_t *)((uint32_t)values + off);
	slope = (int32_t *)((uint32_t)slopes + off);
	lo = value[0];
	m0 = slope[0];
	m1 = slope[1];

	return lo + dt * (((value[1] - lo) / dx - dx * (m1 + m0 * 2)) +
			  dt * (m0 * 3 + (dt * (m1 - m0)) / dx));
#endif
}

int32_t doSomethingWithSomePoints(int16_t *rect, DVECTOR *line)
{
	int16_t *c;
	DVECTOR *p;
	int16_t code[2];
	int32_t i;
	int32_t dx;
	int32_t dy;
	int32_t cx[4];
	int32_t cy[4];
	int32_t cross;

	for (i = 0; i < 2; i++) {
		p = &line[i];
		c = &code[i];

		if (rect[2] < p->vx) {
			*c = 5;
		} else if (p->vx < rect[0]) {
			*c = 3;
		} else {
			*c = 4;
		}

		if (rect[3] < p->vy) {
			*c += 3;
		} else if (p->vy < rect[1]) {
			*c -= 3;
		}

		if (*c == 4) {
			return -1;
		}
	}

	if ((code[0] + code[1]) == 8) {
		return -1;
	}

	if ((code[0] / 3) == (code[1] / 3)) {
		return 0;
	}

	if ((code[0] % 3) == (code[1] % 3)) {
		return 0;
	}

	dx = line[1].vx - line[0].vx;
	dy = line[1].vy - line[0].vy;

	cx[0] = rect[0] - line[0].vx;
	cy[0] = rect[1] - line[0].vy;
	cx[1] = rect[2] - line[0].vx;
	cy[1] = rect[1] - line[0].vy;
	cx[2] = rect[0] - line[0].vx;
	cy[2] = rect[3] - line[0].vy;
	cx[3] = rect[2] - line[0].vx;
	cy[3] = rect[3] - line[0].vy;

	cross = (dx * cy[0]) - (dy * cx[0]);

	for (i = 1; i < 4; i++) {
		if ((cross * ((dx * cy[i]) - (dy * cx[i]))) < 0) {
			return -1;
		}
	}

	return 0;
}

int16_t DOOA_storeDigimonY(void)
{
	DOOA_STORED_DIGIMON_Y = ENTITY_TABLE[1]->posData->location.vy;

	return DOOA_STORED_DIGIMON_Y;
}

int16_t DOOA_getStoredDigimonY(void)
{
	return DOOA_STORED_DIGIMON_Y;
}

void renderFXParticle(SVECTOR *pos, int32_t size, RGB8 *color)
{
	POLY_FT4 *prim;
	int32_t depth;
	DVECTOR screenPos;

	prim = (POLY_FT4 *)GsGetWorkBase();
	depth = worldPosToScreenPos(pos, &screenPos);
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->code |= 2;
	setRGB0(prim, color->r, color->g, color->b);
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 489);
	setUVWH(prim, 0, 160, 15, 15);
	addFXPrim(prim, screenPos.vx, screenPos.vy, size, size, depth);
}
