#ifndef DW_EVOLUTION_H
#define DW_EVOLUTION_H

#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>

int16_t getFreshEvolutionTarget(int32_t currentDigimon);
int16_t getInTrainingEvolutionTarget(int32_t currentDigimon);
int16_t getRookieEvolutionTarget(int32_t currentDigimon);
int16_t getChampionEvolutionTarget(int32_t currentDigimon);
int32_t handleSpecialEvolutions(int32_t mode, Entity *entity);
void reincarnatePartner(int32_t unused, Stats *stats, PartnerPara *partner,
                        int32_t digimonId);

#endif
