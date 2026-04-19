#include "dw/partner.h"
#include <dw/evolution.h>

#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/types.h>


int32_t getTamerState(void);
void setTamerState(int32_t state);
void removeEntity(int32_t type, int32_t entityId);
void thunkUnloadModel(int32_t id, int32_t modelType);
void unloadModel(int32_t id, int32_t modelType);
void initializeReincarnatedPartner(int32_t type, int32_t posX, int32_t posY,
                                   int32_t posZ, int32_t rotX, int32_t rotY,
                                   int32_t rotZ);
int32_t hasDigimonRaised(int32_t digimonId);
void setDigimonRaised(int32_t type);

extern uint8_t CURRENT_SCREEN;
extern int32_t NANIMON_TRIGGER;
extern int16_t EVOLUTION_TARGET;

extern Stats DEATH_STATS;

int32_t calculateRequirementScore(int32_t current, int16_t target,
                                  int8_t isMaxCM, int8_t isMaxBattles,
                                  int8_t currentBest);
int32_t getNumMasteredMoves(void);

static void __garbage__(Stats* stats) { // NOLINT
  int16_t statsArray[6];
  int16_t statsArray2[6];
  statsArray[0] = statsArray2[0] = stats->hp / 10;
  statsArray[1] = statsArray2[1] = stats->mp / 10;
  statsArray[2] = statsArray2[2] = stats->off;
  statsArray[3] = statsArray2[3] = stats->def;
  statsArray[4] = statsArray2[4] = stats->speed;
  statsArray[5] = statsArray2[5] = stats->brain;
}

int16_t getFreshEvolutionTarget(int32_t currentDigimon) {
  int32_t result;

  if (currentDigimon == 0x01)
    result = 0x02;
  if (currentDigimon == 0x0f)
    result = 0x10;
  if (currentDigimon == 0x1d)
    result = 0x1e;
  if (currentDigimon == 0x2b)
    result = 0x2c;

  PARTNER_PARA.evoTimer = 0;
  return result; // possible undefined behavior: 'result' may be uninitialized
}

int16_t getInTrainingEvolutionTarget(int32_t currentDigimon) {
  int32_t i;
  int8_t *targetPtr;
  int32_t currentBest;
  int8_t bestScore;

  currentBest = -1;
  bestScore = 0;
  targetPtr = EVO_PATHS_DATA[currentDigimon - 1].to;

  for (i = 0; i < 6; i++) {
    int32_t flag;
    int8_t requirementScore;

    if (*targetPtr == -1) {
      targetPtr++;
      continue;
    }

    flag = EVO_REQ_DATA[(int32_t)*targetPtr].flags;
    requirementScore = calculateRequirementScore(currentDigimon, *targetPtr,
                                                 ((flag & 0x10) >> 4),
                                                 (flag & 1), currentBest);

    if (bestScore < requirementScore && requirementScore >= 3) {
      currentBest = *targetPtr;
      bestScore = requirementScore;
    }
    targetPtr++;
  }

  return currentBest;
}

int16_t getRookieEvolutionTarget(int32_t currentDigimon) {
  int32_t i;
  int8_t *targetPtr;
  int32_t currentBestDigimon;
  int16_t bestScore;
  int16_t statCount;
  int16_t statTotal;

  targetPtr = EVO_PATHS_DATA[currentDigimon - 1].to;
  currentBestDigimon = -1;
  bestScore = statTotal = statCount = 0U;

  for (i = 0; i < 6; i++) {
    int32_t requirementScore;
    int32_t flag;

    if (*targetPtr == -1) {
      targetPtr++;
      continue;
    }

    flag = EVO_REQ_DATA[(int32_t)*targetPtr].flags;
    requirementScore = calculateRequirementScore(
        currentDigimon, *targetPtr, ((flag & 0x10) >> 4), (flag & 1),
        currentBestDigimon);

    if (requirementScore >= 3) {
      int32_t targetId = *targetPtr;

      if (EVO_REQ_DATA[targetId].hp != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.hp / 10;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].mp != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.mp / 10;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].offense != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.off;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].defense != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.def;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].speed != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.speed;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].brain != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.brain;
        statCount++;
      }

      statTotal /= statCount;
      if (bestScore < statTotal) {
        currentBestDigimon = *targetPtr;
        bestScore = statTotal;
        statTotal = statCount = 0;
      }
    }
    targetPtr++;
  }

  if (currentBestDigimon == -1 && PARTNER_PARA.evoTimer >= 96)
    currentBestDigimon = 11;

  return currentBestDigimon;
}

int16_t getChampionEvolutionTarget(int32_t currentDigimon) {
  int32_t i;
  int8_t *targetPtr;
  int32_t currentBestDigimon;
  int16_t bestScore;
  int16_t statCount;
  int16_t statTotal;

  targetPtr = EVO_PATHS_DATA[currentDigimon - 1].to;
  currentBestDigimon = -1;
  bestScore = statTotal = statCount = 0U;

  for (i = 0; i < 6; i++) {
    int32_t requirementScore;
    int32_t flag;

    if (*targetPtr == -1) {
      targetPtr++;
      continue;
    }

    flag = EVO_REQ_DATA[(int32_t)*targetPtr].flags;
    requirementScore = calculateRequirementScore(
        currentDigimon, *targetPtr, ((flag & 0x10) >> 4), (flag & 1),
        currentBestDigimon);

    if (requirementScore >= 3) {
      int32_t targetId = *targetPtr;

      if (EVO_REQ_DATA[targetId].hp != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.hp / 10;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].mp != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.mp / 10;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].offense != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.off;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].defense != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.def;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].speed != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.speed;
        statCount++;
      }
      if (EVO_REQ_DATA[targetId].brain != -1) {
        statTotal += PARTNER_ENTITY.digimonEntity.stats.brain;
        statCount++;
      }

      statTotal /= statCount;
      if (bestScore < statTotal) {
        currentBestDigimon = *targetPtr;
        bestScore = statTotal;
        statTotal = statCount = 0;
      }
    }
    targetPtr++;
  }

  return currentBestDigimon;
}

int32_t handleSpecialEvolutions(int32_t mode, Entity *entity) {
  int32_t type;
  int16_t level;
  int16_t def;
  int16_t happiness;
  int16_t discipline;
  int16_t evotimer;
  int16_t battles;
  int16_t tiredness;
  int16_t rand;
  int16_t evoTarget;

  if (getTamerState() != 0)
    return -1;

  type = entity->type;
  level = DIGIMON_DATA[type].level;
  def = PARTNER_ENTITY.digimonEntity.stats.def;
  discipline = PARTNER_PARA.discipline;
  happiness = PARTNER_PARA.happiness;
  evotimer = PARTNER_PARA.evoTimer;
  battles = PARTNER_PARA.battles;
  tiredness = PARTNER_PARA.tiredness;
  rand = random(100);
  evoTarget = -1;

  switch (mode) {
  case 2: {
    if (rand < 30) {
      if ((type == 0x15 || type == 0x0a) && discipline == 100 &&
          happiness == 100 && tiredness == 0) {
        evoTarget = 7;
      }
      if (type == 0x19 && battles >= 50 && discipline == 100) {
        evoTarget = 58;
      }
      if (type == 0x26 && def >= 500 && discipline == 100) {
        evoTarget = 47;
      }
    }
    if (rand < 50 &&
        (type == 0x02 || type == 0x1E || type == 0x10 || type == 0x2c) &&
        CURRENT_SCREEN == 1) {
      evoTarget = 32;
    }
    break;
  }
  case 3: {
    if ((type == 0x18 || type == 0x23) && level == 4 && evotimer == 200 &&
        rand < 30) {
      evoTarget = 49;
    }
    if (NANIMON_TRIGGER == 1) {
      evoTarget = 53;
      NANIMON_TRIGGER = 0;
    }
    if (level == 4 && evotimer >= 240 && rand < 50) {
      evoTarget = 28;
    }
    break;
  }
  }

  if (evoTarget != -1) {
    EVOLUTION_TARGET = evoTarget;
    setTamerState(6);
    setPartnerState(0xd);
  }

  return evoTarget;
}

int32_t calculateRequirementScore(int32_t current, int16_t target,
                                  int8_t isMaxCM, int8_t isMaxBattles,
                                  int8_t currentBest_) {
  EvoRequirements *reqs;
  Stats *partnerEntity;
  PartnerPara *partner;
  int8_t reqPoints;
  int8_t isBonusFulfilled;
  int16_t statsArray[6];
  int16_t statsArray2[6];
  int16_t reqArray[6];
  int32_t currentBest = currentBest_;

  reqs = &EVO_REQ_DATA[target];
  partnerEntity = &PARTNER_ENTITY.digimonEntity.stats;
  partner = &PARTNER_PARA;
  reqPoints = 0;

  if (isMaxCM == 0) {
    if (partner->careMistakes >= reqs->care)
      reqPoints += 1;
  } else if (partner->careMistakes <= reqs->care) {
    reqPoints += 1;
  }

  if (reqs->weight - 5 <= partner->weight &&
      partner->weight <= reqs->weight + 5)
    reqPoints += 1;

  if (DIGIMON_DATA[target].level == 3U) {
    int32_t i;
    int32_t highestStat;

    statsArray[0] = statsArray2[0] = partnerEntity->hp / 10;
    statsArray[1] = statsArray2[1] = partnerEntity->mp / 10;
    statsArray[2] = statsArray2[2] = partnerEntity->off;
    statsArray[3] = statsArray2[3] = partnerEntity->def;
    statsArray[4] = statsArray2[4] = partnerEntity->speed;
    statsArray[5] = statsArray2[5] = partnerEntity->brain;
    reqArray[0] = reqs->hp;
    reqArray[1] = reqs->mp;
    reqArray[2] = reqs->offense;
    reqArray[3] = reqs->defense;
    reqArray[4] = reqs->speed;
    reqArray[5] = reqs->brain;

    for (i = 0; i < 6; i++) {
      int32_t j;
      int8_t isHighestStat = 1;

      for (j = 0; j < 6; j++) {
        if (statsArray[i] < statsArray[j])
          isHighestStat = 0;
      }
      if (isHighestStat == 1)
        highestStat = i;
    }
    if (reqArray[highestStat] == 1) {
      reqPoints += 1;
    }
  } else {
    if (partnerEntity->hp / 10 >= reqs->hp &&
        partnerEntity->mp / 10 >= reqs->mp &&
        partnerEntity->off >= reqs->offense &&
        partnerEntity->def >= reqs->defense &&
        partnerEntity->speed >= reqs->speed &&
        partnerEntity->brain >= reqs->brain) {
      reqPoints += 1;
    }
  }

  isBonusFulfilled = 0;
  if (reqs->digimon != -1 && current == reqs->digimon)
    isBonusFulfilled = 1;
  if (reqs->discipline != -1 && reqs->discipline <= partner->discipline)
    isBonusFulfilled = 1;
  if (reqs->happiness != -1 && reqs->happiness <= partner->happiness)
    isBonusFulfilled = 1;
  if (reqs->battles != -1) {
    if (isMaxBattles == 0) {
      if (reqs->battles <= partner->battles)
        isBonusFulfilled = 1;
    } else if (reqs->battles >= partner->battles) {
      isBonusFulfilled = 1;
    }
  }
  if (reqs->techs != -1) {
    int8_t numMoves = getNumMasteredMoves();
    if (numMoves >= reqs->techs)
      isBonusFulfilled = 1;
  }

  reqPoints += isBonusFulfilled;

  if (reqPoints >= 3 && currentBest != -1) {
    int32_t isTargetRaised;
    int32_t isCurrentBestRaised;

    isTargetRaised = hasDigimonRaised(EVO_GAINS_DATA[target].targetDigimon);
    isCurrentBestRaised =
        hasDigimonRaised(EVO_GAINS_DATA[currentBest].targetDigimon);

    if (isTargetRaised == 1 && isCurrentBestRaised == 0)
      reqPoints = 0;

    if (isTargetRaised == 0 && isCurrentBestRaised == 1)
      reqPoints++;
  }

  return reqPoints;
}

int32_t getNumMasteredMoves(void) {
  int8_t moveCount = 0;
  int32_t i;
  int32_t j;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 32; j++) {
      if (PARTNER_ENTITY.learnedMoves[i] & 1 << j)
        moveCount++;
    }
  }

  return moveCount;
}

void reincarnatePartner(int32_t unused, Stats *stats, PartnerPara *partner,
                        int32_t digimonId) {
  int32_t id;
  EvoStatsGains *ptr;
  int32_t previousId;

  DEATH_STATS = *stats;
  ptr = &EVO_GAINS_DATA[digimonId];
  id = ptr->targetDigimon;

  PARTNER_ENTITY.digimonEntity.stats.moves[0] = 0x2e;
  PARTNER_ENTITY.digimonEntity.stats.moves[1] = 0xff;
  PARTNER_ENTITY.digimonEntity.stats.moves[2] = 0xff;
  PARTNER_ENTITY.digimonEntity.stats.moves[3] = 0xff;
  previousId = PARTNER_ENTITY.digimonEntity.entity.type;
  removeEntity(previousId, 1);
  ENTITY_TABLE[1] = NULL;
  thunkUnloadModel(previousId, 3);
  initializeReincarnatedPartner(id, 0, 0, 0, 0, 0, 0);
  PARTNER_ENTITY.lives = 3;
  setDigimonRaised(id);
}
