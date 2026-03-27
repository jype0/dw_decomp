#include <dw/evolution.h>

#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/params.h>
#include <dw/types.h>

#pragma optimization_level 4

int32_t calculateRequirementScore(int32_t current, int16_t target,
                                  int8_t isMaxCM, int8_t isMaxBattles,
                                  int8_t currentBest);

static void __garbage__() {
  Stats partnerEntity;
  int16_t statsArray[6];
  int16_t statsArray2[6];

  statsArray[0] = statsArray2[0] = partnerEntity.hp / 10;
  statsArray[1] = statsArray2[1] = partnerEntity.mp / 10;
  statsArray[2] = statsArray2[2] = partnerEntity.off;
  statsArray[3] = statsArray2[3] = partnerEntity.def;
  statsArray[4] = statsArray2[4] = partnerEntity.speed;
  statsArray[5] = statsArray2[5] = partnerEntity.brain;
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

    flag = EVO_REQ_DATA[*targetPtr].flags;
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

int32_t calculateRequirementScore(int32_t current, int16_t target,
                                  int8_t isMaxCM, int8_t isMaxBattles,
                                  int8_t currentBest) {
  EvoRequirements *reqs;
  Stats *partnerEntity;
  PartnerPara *partner;
  int8_t reqPoints;
  int8_t isBonusFulfilled;
  int8_t finalScore;
  int16_t statsArray[6];
  int16_t statsArray2[6];
  int16_t reqArray[6];

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

int16_t getRookieEvolutionTarget(int32_t currentDigimon) {
  int32_t i;
  int8_t *targetPtr;
  int32_t currentBestDigimon;
  int16_t bestScore;
  int16_t statCount;
  int16_t statTotal;
  int32_t digimon;

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

    flag = EVO_REQ_DATA[*targetPtr].flags;
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

    flag = EVO_REQ_DATA[*targetPtr].flags;
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

int32_t getNumMasteredMoves(void) {
  int8_t moveCount = 0;
  int32_t offset = 0;
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
