#ifndef PARTICLE_H
#define PARTICLE_H

#include <dw/types.h>
#include <libgte.h>


typedef struct {
    int32_t counter;
    SVECTOR pos;
} ParticleObjEntry;

typedef struct {
    int16_t frameId;
    SVECTOR off;
    int16_t radius;
    int16_t hasParticle2;
    int16_t pad;
    ParticleObjEntry entries1[20];
    ParticleObjEntry entries2[20];
} HealingParticle;

typedef struct {
    uint8_t r,g,b;
} RGB8;

extern HealingParticle MAIN_D_8013D1A0[1];


extern RGB8 MAIN_D_80124B78[8];
extern RGB8 MAIN_D_80124B8C[18];
extern RGB8 MAIN_D_80124BC4[18];


void tickParticleObject(int32_t instance);

void renderParticleObject(int32_t idx);

void initializeParticleObject();

int32_t addHealingParticleEffect(Entity *entity, int16_t hasParticle2);

#endif