#ifndef DW_VECMATH_H
#define DW_VECMATH_H

#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>

void mapToWorldPosition(VECTOR *output, int32_t x, int32_t y, int32_t *success);
void rotateVector(SVECTOR *rotation, VECTOR *input, VECTOR *output);
void toEulerAngles(SVECTOR *output, int32_t deltaX, int32_t deltaY, int32_t deltaZ);
int32_t getDistance(int32_t deltaX, int32_t deltaY, int32_t deltaZ);
void matrixToEuler1(MATRIX *matrix, SVECTOR *output);
void matrixToEuler2(MATRIX *matrix, SVECTOR *output);
void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix);
void multiplyRotations(SVECTOR *rotation1, SVECTOR *rotation2);

#endif
