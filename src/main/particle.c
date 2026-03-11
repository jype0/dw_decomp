#include <libgte.h>
#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/particle.h>

#pragma optimization_level 4

// prototypes for functions used below. relocate when appropriate
void renderFXParticle(SVECTOR *worldPos, int32_t scale, RGB8 *rgb);
int32_t addObject(int32_t, int32_t, void*, void*);
void removeObject(int32_t,int32_t);
int32_t lerp(int32_t, int32_t, int32_t, int32_t, int32_t);
int32_t rand();
int32_t _cos(int32_t);
int32_t _sin(int32_t);


void tickParticleObject(int32_t instance) {
    int32_t i;
    int32_t angle;
    HealingParticle *particle;

    ParticleObjEntry *entries1;
    ParticleObjEntry *entries2;

    ParticleObjEntry *entry1;
    ParticleObjEntry *entry2;

    MATRIX rotMat;
    SVECTOR rotateVec;
    SVECTOR radiusVec;

    particle = &MAIN_D_8013D1A0[instance];

    entries1 = particle->entries1;
    entries2 = particle->entries2;

    particle->frameId++;
    if (particle->frameId >= 44) {
        particle->frameId = -1;
        removeObject(0x817, instance);
        return;
    }

    if (particle->frameId < 20) {
        entry1 = &entries1[particle->frameId];
        entry1->counter = 17;

        rotateVec.vx = 0;
        rotateVec.vy = (particle->frameId * 640) & 0xFFF;
        rotateVec.vz = 0;
        RotMatrixZYX(&rotateVec, &rotMat);

        radiusVec.vx = particle->radius;
        radiusVec.vz = 0;
        radiusVec.vy = 0;
        ApplyMatrixSV(&rotMat, &radiusVec, &entry1->pos);

        entry1->pos.vx += particle->off.vx;
        entry1->pos.vy = particle->off.vy - lerp(0, particle->off.vy, 0, 20, particle->frameId);
        entry1->pos.vz += particle->off.vz;
    }

    entry1 = entries1;
    for (i = 0; i < 20; i++) {
        entry1->counter--;
        entry1->pos.pad = (rand() % 70) + 15;
        entry1++;
    }

    if (particle->frameId < 39) {
        entry2 = &entries2[particle->frameId % 5];
        for (i = 0; i < 4; i++) {
            angle = lerp(0, 512, 0, 13, particle->frameId);
            angle += 512 / (i * 13 / 4);

            entry2->counter = 5;

            entry2->pos.vx = particle->off.vx + (rand() % 60 - 30)
                             - (((particle->radius + 150) * _sin(angle)) >> 12);
            entry2->pos.vz = particle->off.vz + (rand() % 60 - 30)
                             + (((particle->radius + 150) * _cos(angle)) >> 12);

            entry2->pos.vy = lerp((rand() % 60 - 30) + particle->off.vy, 0, 0, 39, particle->frameId);

            entry2 += 5;
        }
    }

    for (i = 0; i < 20; i++) {
        entries2->counter--;
        entries2++;
    }
}

void renderParticleObject(int32_t instance) {
    int32_t i;

    HealingParticle *particle = &MAIN_D_8013D1A0[instance];
    ParticleObjEntry *obj1 = particle->entries1;
    ParticleObjEntry *obj2 = particle->entries2;

    for (i = 0; i < 20; i++) {
        // long cast needed for match
        if (obj1->counter >= (long)0) {
            renderFXParticle(&obj1->pos, obj1->pos.pad, &MAIN_D_80124BC4[obj1->counter]);
            renderFXParticle(&obj1->pos, obj1->pos.pad >> 1, &MAIN_D_80124B8C[obj1->counter]);
        }
        obj1++;
    }

    if (particle->hasParticle2 != 0) {
        for (i = 0; i < 20; i++) {
            if (obj2->counter >= (long)0) {
                renderFXParticle(&obj2->pos, 24, &MAIN_D_80124B78[obj2->counter]);
            }
            obj2++;
        }
    }
    return;
}

void initializeParticleObject() {
    int32_t i;

    for (i = 0; i < 1; i++) {
        MAIN_D_8013D1A0[i].frameId = -1;
    }
}

int32_t addHealingParticleEffect(Entity *entity, int16_t hasParticle2) {
    int32_t instance;
    int32_t i;
    PositionData *posData;
    HealingParticle *particle;
    ParticleObjEntry *entry1;
    ParticleObjEntry *entry2;
    MATRIX *vec;

    for (instance = 0; instance < 1; instance++) {
        if (MAIN_D_8013D1A0[instance].frameId == (-1)) {
            break;
        }
    }

    if (instance == 1) {
        return -1;
    }

    particle = &MAIN_D_8013D1A0[instance];
    particle->frameId = 0;
    posData = entity->posData;
    vec = &posData->posMatrix.workm;
    particle->off.vx = vec->t[0];
    particle->off.vy = -DIGIMON_DATA[entity->type].height - 70;
    particle->off.pad = -particle->off.vy / 10;
    particle->off.vz = vec->t[2];
    particle->radius = (DIGIMON_DATA[entity->type].radius * 12) / 10;
    particle->hasParticle2 = hasParticle2;

    // using the same for-loops didn't match
    for (i = 0; i < 20; i++) {
        particle->entries1[i].counter = -1;
    }

    entry2 = particle->entries2;
    for (i = 0; i < 20; i++) {
        entry2->counter = -1;
        entry2++;
    }

    return addObject(0x817, instance, tickParticleObject, renderParticleObject);
}
