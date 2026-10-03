#include <stdlib.h>
#include <string.h>

#include <dw/anim.h>
#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>

#include "common.h"

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} AnimVec;

void calculatePosMatrix(PositionData *posData, int32_t unused1,
			int32_t unused2, int32_t translate);
void resetMomentumData(MomentumData *momentum);
void animateEntityTexture(Entity *entity, EntityAnim *anim);
void tickMomentum(Entity *entity, MomentumData *momentumBase);
void animHandleKeyFrameInstruction(MomentumData *momentum, int16_t **instrPtr);
void animReadMomentumInstruction(int16_t *delta, int16_t *reload1,
			         int16_t *subDelta, int16_t *reload2,
			         int8_t *sign, int16_t **instrPtr,
			         int16_t *divisor);
int32_t animApplyMomentum(int16_t base, int16_t reload, int16_t delta,
		          int16_t *counter, int8_t step, int32_t offset);
void applyRootMomentum(MomentumData *momentum, Entity *entity);

static void *anim_text_order[] = {
	applyRootMomentum,
	animApplyMomentum,
	animReadMomentumInstruction,
	animHandleKeyFrameInstruction,
	tickMomentum,
	tickAnimation,
	startAnimation,
	setupModelMatrix,
	animateEntityTexture,
	resetMomentumData,
	calculatePosMatrix,
};

void calculatePosMatrix(PositionData *posData, int32_t unused1,
			int32_t unused2, int32_t translate)
{
	GsCOORDINATE2 *matrix;

	matrix = &posData->posMatrix;
	if (translate != 0) {
		TransMatrix(&matrix->coord, &posData->location);
	}

	RotMatrix(&posData->rotation, &matrix->coord);
	ScaleMatrix(&matrix->coord, &posData->scale);
	matrix->flg = 0;
}

void resetMomentumData(MomentumData *momentum)
{
	AnimVec zero;

	zero.x = zero.y = zero.z = 0;
	memcpy(&momentum->delta[3], &zero, 6);
	memcpy(&momentum->delta[6], &zero, 6);
	memcpy(&momentum->delta[0], &zero, 6);
	memcpy(&momentum->subDelta[3], &zero, 6);
	memcpy(&momentum->subDelta[6], &zero, 6);
	memcpy(&momentum->subDelta[0], &zero, 6);
}

void animateEntityTexture(Entity *entity, EntityAnim *anim)
{
	int16_t texX;
	int16_t frame;
	RECT rect;

	switch (entity->type) {
	case 0x09:
	case 0x85:
	case 0x68:
		texX = 0x1e;
		break;
	case 0x15:
	case 0x6a:
	case 0x8f:
		texX = 0x28;
		break;
	case 0x65:
	case 0x45:
		texX = 0x10;
		break;
	case 0x7f:
		texX = 0x2c;
		break;
	default:
		return;
	}

	if (PLAYTIME_FRAMES % 2)
		return;

	if (PLAYTIME_FRAMES % 6 == 0) {
		frame = 2;
	} else if (PLAYTIME_FRAMES % 4 == 0) {
		frame = 1;
	} else {
		frame = 0;
	}

	setRECT(&rect, anim->textureX, anim->textureY + 0x20 + frame * 0x20, texX, 0x20);
	MoveImage(&rect, anim->textureX, anim->textureY);
}

void setupModelMatrix(PositionData *posData)
{
	GsCOORDINATE2 *matrix;

	matrix = &posData->posMatrix;
	TransMatrix(&matrix->coord, &posData->location);
	RotMatrix(&posData->rotation, &matrix->coord);
	ScaleMatrix(&matrix->coord, &posData->scale);
	matrix->flg = 0;
}

void tickMomentum(Entity *entity, MomentumData *momentumBase)
{
	int32_t i;
	int32_t boneCount;
	PositionData *posData;
	long *scale;
	long *location;
	int32_t j;
	int16_t *scale1;
	int16_t *subScale;
	int16_t *rotation;
	int8_t *subValue;
	int16_t *subDelta;
	int16_t *delta;
	int32_t updateScale;
	int32_t updateRot;
	int32_t updateLoc;

	posData = entity->posData;
	boneCount = DIGIMON_DATA[entity->type].boneCount;

	for (i = 0; i < boneCount; i++, momentumBase++, posData++) {
		updateLoc = 0;
		updateRot = 0;
		updateScale = 0;

		scale = &posData->scale.vx;
		location = &posData->location.vx;
		rotation = &posData->rotation.vx;
		scale1 = &momentumBase->scale1[0];
		subDelta = &momentumBase->subDelta[0];
		delta = &momentumBase->delta[0];
		subScale = &momentumBase->subScale[0];
		subValue = &momentumBase->subValue[0];

		for (j = 0; j < 9; scale1++, subDelta++, delta++, subScale++,
				   subValue++, j++) {
			if (*delta != 0 || *subDelta != 0) {
				if (j < 3) {
					updateScale = 1;
					scale[j] = animApplyMomentum(*delta, *scale1,
								     *subDelta, subScale,
								     *subValue, scale[j]);
				} else if (j < 6) {
					updateRot = 1;
					*(rotation + j - 3) = animApplyMomentum(
						*delta, *scale1, *subDelta, subScale,
						*subValue, *(rotation + j - 3));
				} else {
					if (i == 0)
						break;

					updateLoc = 1;
					*(location + j - 6) = animApplyMomentum(
						*delta, *scale1, *subDelta, subScale,
						*subValue, *(location + j - 6));
				}
			}
		}

		if (i == 0) {
			calculatePosMatrix(posData, 1, 1, 0);
			applyRootMomentum(momentumBase, entity);
			setupModelMatrix(posData);
		} else {
			calculatePosMatrix(posData, updateScale, updateRot,
					   updateLoc);
		}
	}
}

void startAnimation(entity, animId)
	Entity *entity;
	uint8_t animId;
{
	int32_t i;
	uint32_t *animTable;
	VECTOR *location;
	MomentumData *momentum;
	ModelComponent *model;
	int32_t hasScale;
	int16_t *instrPtr;
	PositionData *posData;
	EntityAnim *anim;
	uint8_t boneCount;

	animTable = (uint32_t *)entity->animPtr;
	if (animTable[animId] == 0)
		return;

	instrPtr = (int16_t *)((int32_t)animTable + animTable[animId]);
	anim = &entity->anim;
	boneCount = DIGIMON_DATA[entity->type].boneCount;
	posData = entity->posData;
	model = getEntityModelComponent(entity->type, getEntityType(entity));

	anim->textureX = (model->pixelPage - 0x10) * 64;
	anim->textureY = model->pixelOffsetY + 0x100;
	anim->loopEndFrame = 1;
	anim->animId = animId;
	anim->animFrame = 1;
	anim->animFlag = 1;
	anim->loopCount = 0;
	momentum = anim->momentum;
	anim->frameCount = *instrPtr++;

	if (anim->frameCount & 0x8000)
		hasScale = 1;
	else
		hasScale = 0;
	anim->frameCount &= 0x7fff;
	location = &posData->location;
	anim->locX = location->vx << 15;
	anim->locY = location->vy << 15;
	anim->locZ = location->vz << 15;

	resetMomentumData(momentum);
	momentum++;
	setupModelMatrix(posData);
	posData++;

	for (i = 1; i < boneCount; i++, momentum++, posData++) {
		if (!hasScale) {
			posData->scale.vx = 0x1000;
			posData->scale.vy = 0x1000;
			posData->scale.vz = 0x1000;
		} else {
			posData->scale.vx = *instrPtr++;
			posData->scale.vy = *instrPtr++;
			posData->scale.vz = *instrPtr++;
		}
		posData->rotation.vx = *instrPtr++;
		posData->rotation.vy = *instrPtr++;
		posData->rotation.vz = *instrPtr++;

		posData->location.vx = *instrPtr++;
		posData->location.vy = *instrPtr++;
		posData->location.vz = *instrPtr++;

		resetMomentumData(momentum);
		setupModelMatrix(posData);
	}

	anim->animInstrPtr = instrPtr;
}

void tickAnimation(Entity *entity)
{
	EntityAnim *anim;
	MomentumData *momentum;
	int16_t **instrPtrPtr;
	int16_t *framePtr;
	RECT rect;
	int16_t bankId;

	anim = &entity->anim;
	instrPtrPtr = &anim->animInstrPtr;
	framePtr = &anim->animFrame;
	momentum = anim->momentum;

	if (!(anim->animFlag & 1))
		return;

	tickMomentum(entity, momentum);

	if (**instrPtrPtr & 0x1000) {
		anim->loopCount = **instrPtrPtr & 0xff;
		(*instrPtrPtr)++;
		anim->loopStart = *instrPtrPtr;
		anim->loopEndFrame = **instrPtrPtr & 0xfff;
	}

	while (*framePtr == anim->loopEndFrame) {
		switch (**instrPtrPtr & 0xf000) {
		case 0x0000:
			(*instrPtrPtr)++;
			animHandleKeyFrameInstruction(momentum, instrPtrPtr);
			break;
		case 0x1000:
			anim->loopCount = **instrPtrPtr & 0xff;
			(*instrPtrPtr)++;
			anim->loopStart = *instrPtrPtr;
			break;
		case 0x2000:
			if (anim->loopCount != 0xff)
				anim->loopCount--;
			if (anim->loopCount == 0) {
				*instrPtrPtr += 2;
			} else {
				(*instrPtrPtr)++;
				*framePtr = **instrPtrPtr;
				*instrPtrPtr = anim->loopStart;
			}
			break;
		case 0x3000:
			(*instrPtrPtr)++;
			setRECT(&rect,
				anim->textureX + ((**instrPtrPtr & 0xff00) >> 8),
				anim->textureY + (*(*instrPtrPtr)++ & 0xff),
				(**instrPtrPtr & 0xff00) >> 8,
				*(*instrPtrPtr)++ & 0xff);
			MoveImage(&rect,
				  anim->textureX + ((**instrPtrPtr & 0xff00) >> 8),
				  anim->textureY + (*(*instrPtrPtr)++ & 0xff));
			break;
		case 0x4000:
			(*instrPtrPtr)++;
			bankId = (**instrPtrPtr & 0xff00) >> 8;
			if (entity->isOnScreen == 1) {
				playSound(bankId != 4
						  ? bankId
						  : ((DigimonEntity *)entity)
							    ->stats.current.vabId,
					  **instrPtrPtr & 0xff);
			}
			(*instrPtrPtr)++;
			break;
		}
		anim->loopEndFrame = **instrPtrPtr & 0xfff;
	}

	if (*framePtr == anim->frameCount)
		anim->animFlag &= 0xfe;
	else
		(*framePtr)++;

	animateEntityTexture(entity, anim);
}

void animHandleKeyFrameInstruction(MomentumData *base, int16_t **instrPtr)
{
	MomentumData *momentum;
	int16_t *reload1;
	int16_t *subDelta;
	int16_t *delta;
	int16_t *reload2;
	int8_t *sign;
	int16_t instruction;
	int16_t divisor;
	uint16_t flag;
	int32_t i;

	goto check;
	do {
		instruction = *(*instrPtr)++;
		momentum = &base[instruction & 0x3f];
		divisor = *(*instrPtr)++;

		reload1 = &momentum->scale1[0];
		subDelta = &momentum->subDelta[0];
		delta = &momentum->delta[0];
		reload2 = &momentum->subScale[0];
		sign = &momentum->subValue[0];

		flag = 0x4000;
		for (i = 0; i < 9; i++) {
			if (instruction & flag) {
				animReadMomentumInstruction(delta + i, reload1 + i,
							    subDelta + i, reload2 + i,
							    sign + i, instrPtr, &divisor);
			}
			flag = flag >> 1;
		}
	check:
		;
	} while (**instrPtr & 0x8000);
}

void animReadMomentumInstruction(int16_t *delta, int16_t *reload1,
			         int16_t *subDelta, int16_t *reload2,
			         int8_t *sign, int16_t **instrPtr,
			         int16_t *divisor)
{
	int16_t value;

	value = *(*instrPtr)++;
	*delta = value / *divisor;
	*subDelta = value % *divisor;
	if (*subDelta != 0) {
		if (*subDelta > 0) {
			*sign = 1;
		} else {
			*sign = -1;
		}

		*subDelta = abs(*subDelta);
		*reload2 = *reload1 = *divisor;
	}
}

int32_t animApplyMomentum(int16_t base, int16_t reload, int16_t delta,
		          int16_t *counter, int8_t step, int32_t offset)
{
	if (delta != 0) {
		*counter -= delta;
		if (*counter <= 0) {
			*counter += reload;
			return step + (offset + base);
		}
	}

	return offset + base;
}

void applyRootMomentum(MomentumData *momentum, Entity *entity)
{
	int32_t i;
	VECTOR input;
	VECTOR result;
	PositionData *posData;
	int32_t base[3];
	int16_t *scale1;
	int16_t *subDelta;
	int16_t *delta;
	int16_t *subScale;
	int8_t *subValue;
	EntityAnim *anim;

	anim = &entity->anim;
	scale1 = &momentum->scale1[6];
	subDelta = &momentum->subDelta[6];
	delta = &momentum->delta[6];
	subScale = &momentum->subScale[6];
	subValue = &momentum->subValue[6];
	i = 0;

	for (; i < 3; i++, delta++, subScale++, subDelta++) {
		if (i == 0) {
			if (anim->animId == 0x24 || anim->animId == 0x23) {
				base[0] = 0;
				continue;
			}
		}

		if (*subDelta != 0) {
			*subScale -= *subDelta;
			if (*subScale <= 0) {
				base[i] = (*delta + subValue[i]) << 15;
				*subScale += scale1[i];
				continue;
			}
		}
		base[i] = *delta << 15;
	}

	if (!(entity->anim.animFlag & 2)) {
		posData = entity->posData;
		input.vx = base[0];
		input.vy = base[1];
		input.vz = base[2];
		ApplyMatrixLV(&posData->posMatrix.coord, &input, &result);

		if ((entity->anim.animFlag & 8) && result.vz < 0)
			result.vz = 0;
		if ((entity->anim.animFlag & 0x10) && result.vx < 0)
			result.vx = 0;
		if ((entity->anim.animFlag & 0x20) && result.vz > 0)
			result.vz = 0;
		if ((entity->anim.animFlag & 0x40) && result.vx > 0)
			result.vx = 0;

		anim->locX = anim->locX + result.vx;
		anim->locY = anim->locY + result.vy;
		anim->locZ = anim->locZ + result.vz;

		posData->location.vx = anim->locX >> 15;
		posData->location.vy = anim->locY >> 15;
		posData->location.vz = anim->locZ >> 15;
	}
}
