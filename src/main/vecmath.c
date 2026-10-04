#include <inline_n.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>
#include <dw/vecmath.h>

#define ABS_VALUE(value)	((value) > 0 ? (value) : -(value))
#define MAX_VALUE(a, b)	((a) > (b) ? (a) : (b))

extern int32_t VIEWPORT_DISTANCE;

void transposeRefMatrix(SVECTOR *pos, VECTOR *out);

// clang-format off
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
// clang-format on

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
done:;
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
	if (output->vx >= 0x800) {
		adjustment = 0x1000;
	} else {
		adjustment = 0;
	}
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
	while (coord->super != NULL) {
		*ptr++ = coord = coord->super;
	}

	*matrix = (*--ptr)->coord;
	i = 0;

	while (stack < ptr) {
		GsMulCoord3(matrix, &(*--ptr)->coord);
	}
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
