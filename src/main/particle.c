#include <libgte.h>
#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/particle.h>
#include <dw/world_object.h>


// prototypes for functions used below. relocate when appropriate
void renderFXParticle(SVECTOR *worldPos, int32_t scale, RGB8 *rgb);
int32_t lerp(int32_t, int32_t, int32_t, int32_t, int32_t);
int32_t rand();
int32_t _cos(int32_t);
int32_t _sin(int32_t);


// clang-format off
RGB8 PARTICLE_COLOR3[6] = {
	{ 0x47, 0x51, 0x00 },
	{ 0x60, 0x6b, 0x19 },
	{ 0x7d, 0x86, 0x3d },
	{ 0xb3, 0xbb, 0x7a },
	{ 0xd7, 0xdc, 0xb7 },
	{ 0xff, 0xff, 0xff },
};

RGB8 PARTICLE_COLOR2[18] = {
	{ 0x04, 0x09, 0x03 },
	{ 0x0a, 0x0e, 0x05 },
	{ 0x10, 0x13, 0x06 },
	{ 0x16, 0x18, 0x07 },
	{ 0x1b, 0x1d, 0x09 },
	{ 0x21, 0x23, 0x0a },
	{ 0x27, 0x28, 0x0b },
	{ 0x2d, 0x2d, 0x0d },
	{ 0x33, 0x32, 0x0e },
	{ 0x39, 0x37, 0x0f },
	{ 0x3f, 0x3d, 0x10 },
	{ 0x44, 0x42, 0x12 },
	{ 0x4a, 0x47, 0x13 },
	{ 0x50, 0x4c, 0x14 },
	{ 0x56, 0x51, 0x16 },
	{ 0x5c, 0x57, 0x17 },
	{ 0x62, 0x5c, 0x18 },
	{ 0x68, 0x61, 0x1a },
};

RGB8 PARTICLE_COLOR1[18] = {
	{ 0x02, 0x04, 0x01 },
	{ 0x04, 0x06, 0x02 },
	{ 0x07, 0x09, 0x03 },
	{ 0x0a, 0x0b, 0x03 },
	{ 0x0c, 0x0d, 0x04 },
	{ 0x0f, 0x10, 0x04 },
	{ 0x12, 0x12, 0x05 },
	{ 0x15, 0x15, 0x06 },
	{ 0x17, 0x17, 0x06 },
	{ 0x1a, 0x19, 0x07 },
	{ 0x1d, 0x1c, 0x07 },
	{ 0x1f, 0x1e, 0x08 },
	{ 0x22, 0x21, 0x09 },
	{ 0x25, 0x23, 0x09 },
	{ 0x27, 0x25, 0x0a },
	{ 0x2a, 0x28, 0x0a },
	{ 0x2d, 0x2a, 0x0b },
	{ 0x30, 0x2d, 0x0c },
};
// clang-format on

void tickHealingParticles(int32_t instance) {
    int32_t i;
    HealingParticle *particle;

    ParticleObjEntry *entries1;
    ParticleObjEntry *entries2;
    ParticleObjEntry *entry2;
    ParticleObjEntry *entries3;
    ParticleObjEntry *entries4;
    ParticleObjEntry *entries5;
    int32_t angle;

    ParticleObjEntry *entry1;

    MATRIX rotMat;
    SVECTOR rotateVec;
    SVECTOR radiusVec;

    particle = &HEALING_PARTICLES[instance];

    entries1 = particle->entries1;
    entries2 = particle->entries2;
    entries3 = &particle->entries2[5];
    entries4 = &particle->entries2[10];
    entries5 = &particle->entries2[15];

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
        radiusVec.vy = radiusVec.vz = 0;
        ApplyMatrixSV(&rotMat, &radiusVec, &entry1->pos);

        entry1->pos.vx += particle->off.vx;
        entry1->pos.vy = particle->off.vy - lerp(0, particle->off.vy, 0, 20, particle->frameId);
        entry1->pos.vz += particle->off.vz;
    }

    entry1 = entries1;
    for (i = 0; i < NUM_HEALING_PARTICLES; i++) {
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

    for (i = 0; i < NUM_HEALING_PARTICLES; i++) {
        entries2->counter--;
        entries2++;
    }
}

void renderHealingParticles(int32_t instance) {
    int32_t i;
    HealingParticle *particle;
    ParticleObjEntry *obj1;
    ParticleObjEntry *obj2;
    ParticleObjEntry *obj3;
    ParticleObjEntry *obj4;
    ParticleObjEntry *obj5;

    particle = &HEALING_PARTICLES[instance];
    obj1 = particle->entries1;
    obj2 = particle->entries2;
    obj3 = &particle->entries2[5];
    obj4 = &particle->entries2[10];
    obj5 = &particle->entries2[15];

    for (i = 0; i < NUM_HEALING_PARTICLES; i++) {
        // long cast needed for match
        if (obj1->counter >= (long)0) {
            renderFXParticle(&obj1->pos, obj1->pos.pad, &PARTICLE_COLOR1[obj1->counter]);
            renderFXParticle(&obj1->pos, obj1->pos.pad >> 1, &PARTICLE_COLOR2[obj1->counter]);
        }
        obj1++;
    }

    if (particle->hasParticle2 != 0) {
        for (i = 0; i < NUM_HEALING_PARTICLES; i++) {
            if (obj2->counter >= (long)0) {
                renderFXParticle(&obj2->pos, 24, &PARTICLE_COLOR3[obj2->counter]);
            }
            obj2++;
        }
    }
    return;
}

void initializeHealingParticles() {
    int32_t i;

    for (i = 0; i < NUM_HEALING_INSTANCES; i++) {
        HEALING_PARTICLES[i].frameId = -1;
    }
}

int32_t addHealingParticleEffect(Entity *entity, int32_t hasParticle2) {
    int32_t instance;
    int32_t i;
    HealingParticle *particle;
    MATRIX *mat;
    ParticleObjEntry *entry;

    for (instance = 0; instance < NUM_HEALING_INSTANCES; instance++) {
        if (HEALING_PARTICLES[instance].frameId == (-1)) {
            break;
        }
    }

    if (instance == NUM_HEALING_INSTANCES) {
        return -1;
    }

    particle = &HEALING_PARTICLES[instance];
    particle->frameId = 0;
    mat = &entity->posData->posMatrix.workm;
    particle->off.vx = mat->t[0];
    particle->off.vy = -DIGIMON_DATA[entity->type].height - 70;
    particle->off.pad = -particle->off.vy / 10;
    particle->off.vz = mat->t[2];
    particle->radius = (DIGIMON_DATA[entity->type].radius * 12) / 10;
    particle->hasParticle2 = hasParticle2;

    // using the same for-loops didn't match
    for (i = 0; i < NUM_HEALING_PARTICLES; i++) {
        particle->entries1[i].counter = -1;
    }

    entry = particle->entries2;
    for (i = 0; i < NUM_HEALING_PARTICLES; i++) {
        entry->counter = -1;
        entry++;
    }

    return addObject(0x817, instance, tickHealingParticles, renderHealingParticles);
}
