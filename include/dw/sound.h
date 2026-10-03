#ifndef DW_SOUND_H
#define DW_SOUND_H

#include <dw/types.h>

#define NUM_SOUND_BUFFERS	10

typedef struct {
	uint16_t sectorId;
	uint16_t sectorCount;
} MapSoundPara;

extern void *VHB_HEADER_ADDR[];
extern unsigned long VHB_SOUNDBUFFER_START[];
extern int32_t ACTIVE_MAP_SOUND_ID;
extern MapSoundPara MAP_SOUND_PARA[];
extern uint8_t DIGIMON_VBALL_SOUND_ID[];
extern uint8_t DIGIMON_VLALL_SOUND_ID[];

int32_t initializeMusic(void);
void finalizeMusic(void);

int32_t loadMapSounds(int32_t mapSoundId);
int32_t VSLoadSounds(void);
int32_t loadDigimonSounds(int32_t vabId, int32_t type);
int32_t loadPartnerSounds(int32_t type);

uint32_t playSound(int32_t vabId, int32_t val);
uint32_t playSound2(int32_t vabId, int32_t val);
void stopSound(void);
void thunkStopSoundMask(uint32_t mask);

int32_t playMusic(int32_t font, int32_t track);
void stopBGM(void);

#endif
