#ifndef DW_PARTICLE_H
#define DW_PARTICLE_H

#include <dw/types.h>
#include <libgte.h>

#define NUM_HEALING_PARTICLES 20
#define NUM_HEALING_INSTANCES 1

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
    ParticleObjEntry entries1[NUM_HEALING_PARTICLES];
    ParticleObjEntry entries2[NUM_HEALING_PARTICLES];
} HealingParticle;

typedef struct {
    uint8_t r,g,b;
} RGB8;

extern HealingParticle HEALING_PARTICLES[NUM_HEALING_INSTANCES];


extern RGB8 PARTICLE_COLOR3[8];
extern RGB8 PARTICLE_COLOR2[18];
extern RGB8 PARTICLE_COLOR1[18];


void tickHealingParticles(int32_t instance);

void renderHealingParticles(int32_t instance);

void initializeHealingParticles();

int32_t addHealingParticleEffect(Entity *entity, int16_t hasParticle2);

#endif