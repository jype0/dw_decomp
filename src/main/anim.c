#include <dw/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <memory.h>
#include <stdlib.h>
#include <dw/clock.h>
#include <common.h>

/* Function prototypes (required by MWCC -requireprotos) */
int32_t applyMomentum(int32_t offset, int32_t step, int32_t delta, int16_t *value,
                      int8_t flag, int32_t base);
void calculatePosMatrix(PositionData *posData, int32_t unused1,
                        int32_t unused2, int32_t updateTrans);
void setupModelMatrix(PositionData *posData);
void resetMomentumData(MomentumData *momentum);
void readMomentumInstruction(int16_t *delta, int16_t *scale1, int16_t *subDelta,
                             int16_t *subScale, char *subValue, int16_t **instrPtr,
                             int16_t *defValue);
void readMomentumInstructions(MomentumData *base, int16_t **instrPtr);
void animateEntityTexture(Entity *entity, EntityAnim *anim);
void startAnimation(Entity *entity, int32_t animId);
void tickAnimation(Entity *entity);
void tickMomentum(Entity *entity, MomentumData *momentum);
void playSound(int32_t, int32_t);

typedef struct {
    uint32_t useCount;
    void *modelPtr;
    int32_t *animTablePtr;
    void *mmdPtr;
    uint16_t pixelPage;
    uint16_t clutPage;
    uint8_t pixelOffsetX;
    uint8_t pixelOffsetY;
    int16_t modelId;
    int16_t digiType;
    uint16_t pad;
} ModelComponent;

int32_t getEntityType(int32_t digimonType);
ModelComponent *getEntityModelComponent(int32_t instance, int32_t type);
void applyRootMomentum(MomentumData *momentum, Entity *entity);
void tickAnimation(Entity *entity);
void tickMomentum(Entity *entity, MomentumData *momentum);

int32_t applyMomentum(int32_t offset, int32_t step, int32_t delta, int16_t *value,
                      int8_t flag, int32_t base)
{
    if (delta != 0) {
        int32_t v = *value - delta;
        *value = (int16_t)v;
        v = *value;
        if (v <= 0) {
            v += step;
            *value = (int16_t)v;
            return base + offset + flag;
        }
    }
    return base + offset;
}

void calculatePosMatrix(PositionData *posData, int32_t unused1,
                        int32_t unused2, int32_t updateTrans)
{
    unsigned long *flg;

    flg = (unsigned long *)((uint8_t *)posData + 0x10);
    if (updateTrans != 0) {
        TransMatrix((MATRIX *)((uint8_t *)flg + 4),
                    (VECTOR *)((uint8_t *)posData + 0x78));
    }
    RotMatrix((SVECTOR *)((uint8_t *)posData + 0x70),
              (MATRIX *)((uint8_t *)flg + 4));
    ScaleMatrix((MATRIX *)((uint8_t *)flg + 4),
                (VECTOR *)((uint8_t *)posData + 0x60));
    *flg = 0;
}

void setupModelMatrix(PositionData *posData)
{
    unsigned long *flg;
    MATRIX *matrix;

    flg = (unsigned long *)((uint8_t *)posData + 0x10);
    TransMatrix((MATRIX *)((uint8_t *)flg + 4),
                (VECTOR *)((uint8_t *)posData + 0x78));
    matrix = (MATRIX *)((uint8_t *)flg + 4);
    RotMatrix((SVECTOR *)((uint8_t *)posData + 0x70), matrix);
    ScaleMatrix(matrix, (VECTOR *)((uint8_t *)posData + 0x60));
    *flg = 0;
}

void resetMomentumData(MomentumData *momentum)
{
    int16_t zero[3];

    zero[2] = 0;
    zero[1] = 0;
    zero[0] = 0;

    memcpy(&momentum->delta[3], zero, 6);
    memcpy(&momentum->delta[6], zero, 6);
    memcpy(&momentum->delta[0], zero, 6);
    memcpy(&momentum->subDelta[3], zero, 6);
    memcpy(&momentum->subDelta[6], zero, 6);
    memcpy(&momentum->subDelta[0], zero, 6);
}

void readMomentumInstruction(int16_t *delta, int16_t *scale1, int16_t *subDelta,
                             int16_t *subScale, char *subValue, int16_t **instrPtr,
                             int16_t *defValue)
{
    int16_t instruction;

    instruction = *(*instrPtr)++;

    *delta = instruction / *defValue;
    *subDelta = instruction % *defValue;

    if (*subDelta != 0) {
        if (*subDelta > 0) {
            *subValue = 1;
        } else {
            *subValue = -1;
        }
        *subDelta = abs(*subDelta);
        {
            int16_t dp_val = *defValue;
            *scale1 = dp_val;
            *subScale = dp_val;
        }
    }
}

void readMomentumInstructions(MomentumData *base, int16_t **instrPtr)
{
    MomentumData *momentum;
    register int16_t instruction;
    int16_t defValue;
    int16_t *deltaPtr;
    int16_t *scale1Ptr;
    int16_t *subDeltaPtr;
    int16_t *subScalePtr;
    char *subValuePtr;
    int16_t flag;
    int i;

    goto check;
    do {
        instruction = *(*instrPtr)++;
        momentum = &base[instruction & 0x3F];
        defValue = *(*instrPtr)++;

        deltaPtr = &momentum->delta[0];
        scale1Ptr = &momentum->scale1[0];
        subDeltaPtr = &momentum->subDelta[0];
        subScalePtr = &momentum->subScale[0];
        subValuePtr = &momentum->subValue[0];

        flag = 0x4000;
        for (i = 0; i < 9; i++) {
            if (instruction & flag) {
                readMomentumInstruction(
                    deltaPtr + i,
                    scale1Ptr + i,
                    subDeltaPtr + i,
                    subScalePtr + i,
                    subValuePtr + i,
                    instrPtr,
                    &defValue
                );
            }
            flag >>= 1;
        }
    check:
        instruction = *(*instrPtr);
    } while (instruction & 0x8000);
}

void animateEntityTexture(Entity *entity, EntityAnim *anim)
{
    int16_t texX;
    int16_t frame;
    int32_t temp;
    RECT rect;

    if (entity->type == 0x7F) goto tex_2c;
    if (entity->type == 0x45) goto tex_10;
    if (entity->type == 0x65) goto tex_10;
    if (entity->type == 0x8F) goto tex_28;
    if (entity->type == 0x6A) goto tex_28;
    if (entity->type == 0x15) goto tex_28;
    if (entity->type == 0x68) goto tex_1e;
    if (entity->type == 0x85) goto tex_1e;
    if (entity->type != 0x09) return;
    texX = 0x1E;
    goto tex_done;

tex_2c:
    texX = 0x2C;
    goto tex_done;
tex_10:
    texX = 0x10;
    goto tex_done;
tex_28:
    texX = 0x28;
    goto tex_done;
tex_1e:
    texX = 0x1E;

tex_done:
    temp = (int16_t)PLAYTIME_FRAMES & 1;
    if (temp) {
        return;
    }
    frame = (int16_t)PLAYTIME_FRAMES;

    if (frame % 6 == 0) {
        frame = 2;
    } else {
        if (frame % 4 == 0) {
            frame = 1;
        } else {
            frame = 0;
        }
    }

    rect.x = anim->textureX;
    rect.y = anim->textureY + 0x20 + frame * 0x20;
    rect.w = texX;
    rect.h = 0x20;
    MoveImage(&rect, anim->textureX, anim->textureY);
}

void startAnimation(Entity *entity, int32_t animId)
{
    int16_t *animData;
    int16_t *instrPtr;
    MomentumData *momentum;
    PositionData *posData;
    ModelComponent *model;
    int loopFlag;
    int boneCount;
    int entityType;
    int i;

    {
        int32_t *animTable;
        int32_t animOffset;

        animTable = entity->animPtr;
        animOffset = animTable[animId];
        if (animOffset == 0) {
            return;
        }
        animData = (int16_t *)((uint8_t *)animTable + animOffset);
    }
    instrPtr = animData;
    posData = entity->posData;
    momentum = entity->anim.momentum;

    boneCount = DIGIMON_DATA[entity->type].boneCount & 0xFF;
    entityType = getEntityType(entity->type);
    model = getEntityModelComponent(entity->type, entityType);

    entity->anim.textureY = (model->pixelPage - 0x10) * 64;
    entity->anim.textureX = model->pixelOffsetY + 0x100;
    entity->anim.loopEndFrame = 1;
    entity->anim.animId = animId;
    entity->anim.animFrame = 1;
    entity->anim.animFlag = 1;
    entity->anim.loopCount = 0;
    entity->anim.frameCount = *instrPtr;

    if (entity->anim.frameCount & 0x8000) {
        loopFlag = 1;
    } else {
        loopFlag = 0;
    }
    instrPtr++;

    entity->anim.frameCount &= 0x7FFF;

    entity->anim.locX = posData->location.vx << 15;
    entity->anim.locY = posData->location.vy << 15;
    entity->anim.locZ = posData->location.vz << 15;

    resetMomentumData(momentum);
    momentum++;
    setupModelMatrix(posData);
    posData++;

    for (i = 1; i < boneCount; i++) {
        if (loopFlag) {
            posData->scale.vx = *instrPtr++;
            posData->scale.vy = *instrPtr++;
            posData->scale.vz = *instrPtr++;
        } else {
            posData->scale.vx = 0x1000;
            posData->scale.vy = 0x1000;
            posData->scale.vz = 0x1000;
        }

        posData->rotation.vx = *instrPtr++;
        posData->rotation.vy = *instrPtr++;
        posData->rotation.vz = *instrPtr++;

        posData->location.vx = *instrPtr++;
        posData->location.vy = *instrPtr++;
        posData->location.vz = *instrPtr++;

        resetMomentumData(momentum);
        momentum++;
        setupModelMatrix(posData);
        posData++;
    }

    entity->anim.animInstrPtr = instrPtr;
}

void applyRootMomentum(MomentumData *momentum, Entity *entity)
{
    int16_t *delta;
    int16_t *subDelta;
    int16_t *subScale;
    int16_t *scale1;
    char *subValue;
    int32_t baseVals[3];
    int32_t result[3];
    int i;
    int32_t temp;

    scale1 = &momentum->scale1[6];
    subDelta = &momentum->subDelta[6];
    delta = &momentum->delta[6];
    subScale = &momentum->subScale[6];
    subValue = &momentum->subValue[6];

    for (i = 0; i < 3; i++) {
        if (i == 0) {
            if (entity->anim.animId == 0x24 || entity->anim.animId == 0x23) {
                baseVals[i] = 0;
                subDelta++;
                delta++;
                subScale++;
                scale1++;
                continue;
            }
        }
        if (*subDelta == 0) {
            baseVals[i] = *delta << 15;
        } else {
            *subScale -= *subDelta;
            if (*subScale > 0) {
                baseVals[i] = *delta << 15;
            } else {
                temp = *delta + subValue[i];
                baseVals[i] = temp << 15;
                *subScale += scale1[i];
            }
        }
        subDelta++;
        delta++;
        subScale++;
        scale1++;
    }

    if (!(entity->anim.animFlag & 0x2)) {
        ApplyMatrixLV((MATRIX *)((uint8_t *)entity->posData + 0x14),
                      (VECTOR *)baseVals, (VECTOR *)result);

        if ((entity->anim.animFlag & 0x8) && result[2] < 0) result[2] = 0;
        if ((entity->anim.animFlag & 0x10) && result[0] < 0) result[0] = 0;
        if ((entity->anim.animFlag & 0x20) && result[2] > 0) result[2] = 0;
        if ((entity->anim.animFlag & 0x40) && result[0] > 0) result[0] = 0;

        entity->anim.locX += result[0];
        entity->anim.locY += result[1];
        entity->anim.locZ += result[2];

        entity->posData->location.vx = entity->anim.locX >> 15;
        entity->posData->location.vy = entity->anim.locY >> 15;
        entity->posData->location.vz = entity->anim.locZ >> 15;
    }
}

void tickMomentum(Entity *entity, MomentumData *momentumBase)
{
    PositionData *posData;
    int32_t boneCount;
    int32_t updateScale;
    int32_t updateRot;
    int32_t updateLoc;
    int16_t *pDelta;
    int16_t *pScale1;
    int16_t *pSubDelta;
    int16_t *pSubScale;
    int8_t *pSubValue;
    int32_t elemIndex;
    int32_t boneIndex;
    int32_t scaleOffset;
    int32_t rotOffset;

    posData = entity->posData;
    boneCount = DIGIMON_DATA[entity->type].boneCount;

    for (boneIndex = 0; boneIndex < boneCount; boneIndex++) {
        updateScale = 0;
        updateRot = 0;
        updateLoc = 0;

        pScale1 = &momentumBase->scale1[0];
        pSubDelta = &momentumBase->subDelta[0];
        pDelta = &momentumBase->delta[0];
        pSubScale = &momentumBase->subScale[0];
        pSubValue = &momentumBase->subValue[0];
        scaleOffset = 0;
        rotOffset = 0;

        for (elemIndex = 0; elemIndex < 9; elemIndex++) {
            int32_t delta = *pDelta;
            if (delta == 0 && *pSubDelta == 0) {
                goto advance;
            }

            if (elemIndex < 3) {
                updateScale = 1;
                (&posData->scale.vx)[elemIndex] = applyMomentum(
                    delta, *pScale1, *pSubDelta, pSubScale,
                    *pSubValue, (&posData->scale.vx)[elemIndex]);
            } else if (elemIndex < 6) {
                updateRot = 1;
                (&posData->rotation.vx)[elemIndex - 3] = (int16_t)applyMomentum(
                    delta, *pScale1, *pSubDelta, pSubScale,
                    *pSubValue, (&posData->rotation.vx)[elemIndex - 3]);
            } else {
                if (boneIndex == 0) {
                    break;
                }
                updateLoc = 1;
                (&posData->location.vx)[elemIndex - 6] = applyMomentum(
                    delta, *pScale1, *pSubDelta, pSubScale,
                    *pSubValue, (&posData->location.vx)[elemIndex - 6]);
            }

advance:
            pDelta++;
            pScale1++;
            pSubDelta++;
            pSubScale++;
            pSubValue++;
        }

        if (boneIndex == 0) {
            calculatePosMatrix(posData, 1, 1, 0);
            applyRootMomentum(momentumBase, entity);
            setupModelMatrix(posData);
        } else {
            calculatePosMatrix(posData, updateScale, updateRot, updateLoc);
        }

        momentumBase++;
        posData++;
    }
}

static void tickAnimation_loopSound(Entity *entity, int16_t instruction)
{
    int16_t soundId, soundArg;

    soundId = (instruction >> 8) & 0xFF;
    soundArg = instruction & 0xFF;
    if (entity->flatTimer == 1) {
        if (soundId == 4) {
            playSound(entity->anim.field18_0x32, soundArg);
        } else {
            playSound(soundId, soundArg);
        }
    }
}

void tickAnimation(Entity *entity)
{
    EntityAnim *anim;
    MomentumData *momentum;
    int16_t **instrPtrPtr;
    int16_t *framePtr;
    int16_t *instrPtr;
    int16_t instruction;
    int32_t opcode;

    anim = &entity->anim;
    momentum = anim->momentum;
    instrPtrPtr = &anim->animInstrPtr;
    framePtr = &anim->animFrame;

    if (!(anim->animFlag & 1)) {
        return;
    }

    tickMomentum(entity, momentum);

    instruction = **instrPtrPtr;
    if (instruction & 0x1000) {
        anim->loopCount = instruction;
        *instrPtrPtr += 1;
        anim->loopStart = *instrPtrPtr;
        instruction = **instrPtrPtr;
        anim->loopEndFrame = instruction & 0xFFF;
    }

loop:
    instrPtr = *instrPtrPtr;
    instruction = *instrPtr;
    opcode = instruction & 0xF000;

    if (opcode == 0x4000) {
        goto op_4000;
    }
    if (opcode == 0x3000) {
        goto op_3000;
    }
    if (opcode == 0x2000) {
        goto op_2000;
    }
    if (opcode == 0x1000) {
        goto op_1000;
    }
    if (opcode == 0x0000) {
        goto op_0000;
    }
    goto op_end;

op_4000:
    *instrPtrPtr = instrPtr + 1;
    instruction = **instrPtrPtr;
    tickAnimation_loopSound(entity, instruction);
    *instrPtrPtr += 1;
    goto op_end;

op_3000:
    {
        RECT rect;
        int16_t dx, dy;

        *instrPtrPtr = instrPtr + 1;
        rect.x = anim->textureY + (((*instrPtrPtr)[0] >> 8) & 0xFF);
        rect.y = anim->textureX + ((*instrPtrPtr)[0] & 0xFF);
        *instrPtrPtr += 1;
        rect.w = ((*instrPtrPtr)[0] >> 8) & 0xFF;
        rect.h = (*instrPtrPtr)[0] & 0xFF;
        *instrPtrPtr += 1;
        dx = anim->textureY + (((*instrPtrPtr)[0] >> 8) & 0xFF);
        dy = anim->textureX + ((*instrPtrPtr)[0] & 0xFF);
        *instrPtrPtr += 1;
        MoveImage(&rect, dx, dy);
    }
    goto op_end;

op_2000:
    if (anim->loopCount != 0xFF) {
        anim->loopCount--;
    }
    if (anim->loopCount == 0) {
        *instrPtrPtr = instrPtr + 2;
    } else {
        *instrPtrPtr = instrPtr + 1;
        *framePtr = **instrPtrPtr;
        *instrPtrPtr = anim->loopStart;
    }
    goto op_end;

op_1000:
    anim->loopCount = instruction;
    *instrPtrPtr = instrPtr + 1;
    anim->loopStart = *instrPtrPtr;
    goto op_end;

op_0000:
    *instrPtrPtr = instrPtr + 1;
    readMomentumInstructions(momentum, instrPtrPtr);
    /* fall through to op_end */

op_end:
    if (*framePtr == anim->loopEndFrame) {
        goto loop;
    }

    if (*framePtr == anim->frameCount) {
        anim->animFlag &= 0xFE;
    } else {
        (*framePtr)++;
    }

    animateEntityTexture(entity, anim);
}