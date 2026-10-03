#include <inline_n.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>

#include "common.h"

#define CUSTOM_RNG_FACTOR	0x41c650ad
#define CUSTOM_RNG_VALUE	0x3039

#define ABS_VALUE(value)	((value) > 0 ? (value) : -(value))
#define MAX_VALUE(a, b)	((a) > (b) ? (a) : (b))

extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t VIEWPORT_DISTANCE;

extern uint32_t CUSTOM_RNG_1;

void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix);

const uint8_t MAIN_D_80114D68[256] = {
	0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
};

void setLineBlendingMode(int32_t mode, int32_t order);
void drawLine3P(uint32_t color, int32_t x0, int32_t y0,
		int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		int32_t order, uint32_t mode);
void drawLine2P(uint32_t color, int32_t x0, int32_t y0, int32_t x1,
		int32_t y1, int32_t order, uint32_t mode);
void mapToWorldPosition(VECTOR *output, int32_t x, int32_t y,
			int32_t *success);
void rotateVector(SVECTOR *rotation, VECTOR *input, VECTOR *output);
void toEulerAngles(SVECTOR *output, int32_t deltaX, int32_t deltaY,
		   int32_t deltaZ);
int32_t getDistance(int32_t deltaX, int32_t deltaY, int32_t deltaZ);
void matrixToEuler1(MATRIX *matrix, SVECTOR *output);
void matrixToEuler2(MATRIX *matrix, SVECTOR *output);
void multiplyRotations(SVECTOR *rotation1, SVECTOR *rotation2);
int32_t customRandom(int32_t min, int32_t max);

// clang-format off
uint32_t CUSTOM_RNG_2 = 0x0013cc25;
// clang-format on

void setLineBlendingMode(int32_t mode, int32_t order)
{
	DR_TPAGE *prim;

	if (mode == 0) {
		return;
	}

	prim = (DR_TPAGE *)GsGetWorkBase();
	setDrawTPage(prim, 1, 1, getTPage(0, mode & 3, 0, 0));

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);
}

void drawLine3P(uint32_t color, int32_t x0, int32_t y0,
		int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		int32_t order, uint32_t mode)
{
	LINE_F3 *prim;

	prim = (LINE_F3 *)GsGetWorkBase();
	*(uint32_t *)&prim->r0 = color;
	setXY3(prim, x0, y0, x1, y1, x2, y2);
	setLineF3(prim);
	setSemiTrans(prim, mode >> 2);

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);

	setLineBlendingMode(mode, order);
}

void drawLine2P(uint32_t color, int32_t x0, int32_t y0, int32_t x1,
		int32_t y1, int32_t order, uint32_t mode)
{
	LINE_F2 *prim;

	prim = (LINE_F2 *)GsGetWorkBase();

	*(uint32_t *)&prim->r0 = color;
	setXY2(prim, x0, y0, x1, y1);
	setLineF2(prim);
	setSemiTrans(prim, mode >> 2);

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);

	setLineBlendingMode(mode, order);
}

void transposeRefMatrix(SVECTOR *pos, VECTOR *out);

void transposeRefMatrix(SVECTOR *pos, VECTOR *out)
{
	MATRIX m;
	SVECTOR v;

	v.vx = pos->vx - ((VECTOR *)GsWSMATRIX.t)->vx;
	v.vy = pos->vy - ((VECTOR *)GsWSMATRIX.t)->vy;
	v.vz = pos->vz - ((VECTOR *)GsWSMATRIX.t)->vz;
	TransposeMatrix(&GsWSMATRIX, &m);
	ApplyMatrix(&m, &v, out);
}

void mapToWorldPosition(VECTOR *output, int32_t x, int32_t y,
			int32_t *success)
{
	SVECTOR positions[2];
	VECTOR transformed[2];
	int32_t i;

	if (success != NULL) {
		*success = 0;
	}

	positions[0].vx = 0;
	positions[0].vy = 0;
	positions[0].vz = 0;
	positions[1].vx = x;
	positions[1].vy = y;
	positions[1].vz = VIEWPORT_DISTANCE;

	for (i = 0; i < 2; i++) {
		transposeRefMatrix(&positions[i], &transformed[i]);
	}

	transformed[1].vx -= transformed[0].vx;
	transformed[1].vy -= transformed[0].vy;
	transformed[1].vz -= transformed[0].vz;

	if ((uint32_t)transformed[0].vy == 0 ||
	    (uint32_t)transformed[1].vy == 0 ||
	    (((transformed[0].vy > 0) ^ (transformed[1].vy > 0)) == 0)) {
		goto done;
	}

	output->vx = transformed[1].vx * transformed[0].vy /
			     -transformed[1].vy +
		     transformed[0].vx;
	output->vy = 0;
	output->vz = transformed[1].vz * transformed[0].vy /
			     -transformed[1].vy +
		     transformed[0].vz;

	if (success != NULL) {
		*success = 1;
	}
done:
	;
}

void rotateVector(SVECTOR *rotation, VECTOR *input, VECTOR *output)
{
	MATRIX matrix;
	RotMatrixYXZ(rotation, &matrix);
	ApplyMatrixLV(&matrix, input, output);
}

void toEulerAngles(SVECTOR *output, int32_t deltaX, int32_t deltaY,
		   int32_t deltaZ)
{
	int32_t adjustment;

	output->vy = ratan2(deltaX, deltaZ);
	deltaZ = getDistance(deltaX, 0, deltaZ);
	output->vx = -ratan2(deltaY, deltaZ);
	output->vz = 0;

	output->vx &= 0xfff;
	if (output->vx >= 0x800)
		adjustment = 0x1000;
	else
		adjustment = 0;
	output->vx -= adjustment;

	output->vy &= 0xfff;
	output->vy -= output->vy >= 0x800 ? 0x1000 : 0;
}

int32_t getDistance(int32_t deltaX, int32_t deltaY, int32_t deltaZ)
{
	int32_t shift;
	int32_t leadingZeroes;

	deltaX = ABS_VALUE(deltaX);
	deltaY = ABS_VALUE(deltaY);
	deltaZ = ABS_VALUE(deltaZ);
	shift = deltaX + deltaY + deltaZ;

	gte_ldlzc(shift);
	if (shift <= 0) {
		return shift != 0 ? 0x80000000 : 0;
	}

	gte_stlzc(&leadingZeroes);
	shift = 17 - leadingZeroes;
	shift = shift >= 0 ? shift : 0;
	deltaX >>= shift;
	deltaY >>= shift;
	deltaZ >>= shift;
	deltaX = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;

	gte_ldlzc(deltaX);
	deltaY = MAIN_D_80114D68[deltaX & 0xff];
	if (deltaX >= 0x100) {
		gte_stlzc(&leadingZeroes);
		deltaY = (leadingZeroes & 1) + 24 - leadingZeroes;
		deltaY = MAIN_D_80114D68[deltaX >> deltaY] << (deltaY >> 1);

		deltaY += ((deltaX / deltaY) - deltaY) >> 1;
		deltaY += ((deltaX / deltaY) - deltaY) >> 1;
	}

	deltaX = deltaY << shift;
	return deltaX < 0 ? 0x80000000 : deltaX;
}

void matrixToEuler1(MATRIX *matrix, SVECTOR *output)
{
	int32_t sinX;
	int32_t cosX;
	int32_t sinZ;
	int32_t cosZ;
	int32_t value;
	int32_t maximum;

	if (matrix->m[2][2] == 0 && matrix->m[0][0] == 0) {
		output->vy = matrix->m[0][2] > 0 ? 0x400 : -0x400;
		output->vx = ratan2(matrix->m[2][1], matrix->m[1][1]);
		output->vz = 0;
		return;
	}

	output->vx = ratan2(-matrix->m[1][2], matrix->m[2][2]);
	output->vz = ratan2(-matrix->m[0][1], matrix->m[0][0]);

	sinX = rsin(output->vx);
	cosX = rcos(output->vx);
	sinZ = rsin(output->vz);
	cosZ = rcos(output->vz);

	maximum = MAX_VALUE(MAX_VALUE(ABS_VALUE(sinX), ABS_VALUE(cosX)),
			    MAX_VALUE(ABS_VALUE(sinZ), ABS_VALUE(cosZ)));

	if (maximum == ABS_VALUE(sinX)) {
		value = matrix->m[1][2] * -0x1000 / sinX;
	} else if (maximum == ABS_VALUE(cosX)) {
		value = (matrix->m[2][2] << 12) / cosX;
	} else if (maximum == ABS_VALUE(sinZ)) {
		value = matrix->m[0][1] * -0x1000 / sinZ;
	} else if (maximum == ABS_VALUE(cosZ)) {
		value = (matrix->m[0][0] << 12) / cosZ;
	}

	output->vy = ratan2(matrix->m[0][2], value);
}

void matrixToEuler2(MATRIX *matrix, SVECTOR *output)
{
	int32_t sinX;
	int32_t value;
	int32_t sinY;
	int32_t cosY;
	int32_t sinZ;
	int32_t cosZ;
	int32_t maximum;

	if (matrix->m[2][2] == 0 && matrix->m[0][2] == 0) {
		output->vx = matrix->m[1][2] > 0 ? -0x400 : 0x400;
		output->vy = ratan2(-matrix->m[2][0], matrix->m[0][0]);
		output->vz = 0;
		return;
	}

	output->vy = ratan2(matrix->m[0][2], matrix->m[2][2]);
	output->vz = ratan2(matrix->m[1][0], matrix->m[1][1]);

	sinY = rsin(output->vy);
	cosY = rcos(output->vy);
	sinZ = rsin(output->vz);
	cosZ = rcos(output->vz);

	maximum = MAX_VALUE(MAX_VALUE(ABS_VALUE(sinY), ABS_VALUE(cosY)),
			    MAX_VALUE(ABS_VALUE(sinZ), ABS_VALUE(cosZ)));

	if (maximum == ABS_VALUE(sinY)) {
		value = (matrix->m[0][2] << 12) / sinY;
	} else if (maximum == ABS_VALUE(cosY)) {
		value = (matrix->m[2][2] << 12) / cosY;
	} else if (maximum == ABS_VALUE(sinZ)) {
		value = (matrix->m[1][0] << 12) / sinZ;
	} else if (maximum == ABS_VALUE(cosZ)) {
		value = (matrix->m[1][1] << 12) / cosZ;
	}

	sinX = -matrix->m[1][2];
	output->vx = ratan2(sinX, value);
}

void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix)
{
	GsCOORDINATE2 *stack[100];
	int32_t i;
	GsCOORDINATE2 **ptr;

	ptr = stack;
	*ptr++ = coord;
	while (coord->super != NULL)
		*ptr++ = coord = coord->super;

	*matrix = (*--ptr)->coord;
	i = 0;

	while (stack < ptr)
		GsMulCoord3(matrix, &(*--ptr)->coord);
}

void multiplyRotations(SVECTOR *rotation1, SVECTOR *rotation2)
{
	MATRIX matrix1;
	MATRIX matrix2;
	MATRIX matrix3;

	if ((rotation2->vx == 0) &&
	    (rotation2->vy == 0) &&
	    (rotation2->vz == 0)) {
		return;
	}

	RotMatrixYXZ(rotation1, &matrix1);
	RotMatrixYXZ(rotation2, &matrix2);
	MulMatrix0(&matrix1, &matrix2, &matrix3);
	matrixToEuler2(&matrix3, rotation1);
}

int32_t customRandom(int32_t min, int32_t max)
{
	int32_t tmp;

	if (max == min) {
		return min;
	}

	if (max < min) {
		tmp = min;
		min = max;
		max = tmp;
	}

	CUSTOM_RNG_1 = CUSTOM_RNG_1 * CUSTOM_RNG_FACTOR + CUSTOM_RNG_VALUE;
	CUSTOM_RNG_2 = CUSTOM_RNG_2 * CUSTOM_RNG_FACTOR + CUSTOM_RNG_VALUE;

	return min + (int32_t)(((CUSTOM_RNG_1 >> 16) | (CUSTOM_RNG_2 << 16)) %
			       (max - min + 1));
}
