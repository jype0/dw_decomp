#include <libsnd.h>
#include <string.h>

#include <dw/file_queue.h>
#include <dw/main.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/types.h>

#include "common.h"

typedef struct {
	uint8_t *buffer;
	int16_t vabId;
	int8_t isLoading;
	int8_t pad;
} SoundBuffer;

SoundBuffer SOUND_BUFFERS[NUM_SOUND_BUFFERS] = {
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
	{ NULL, 0, 1, 0 },
};

char SOUND_SB_PATH[] = "SOUND\\SB";

int32_t LOAD_SOUND_COMPLETE_STATE;

void loadFullVHB(int32_t vabId, char *path, uint8_t *buffer);
void concatStrings2(char *dst, char *s1, char *s2);
int32_t loadSoundFinishCallback(void *param);
void loadVHBFile(int32_t vabId, char *path, uint8_t *buffer, int32_t offset,
		 int32_t sectors);
int32_t loadSoundCompleteCallback(void *param);
void uploadSoundBuffer(int32_t vabId);

static void *sound_async_functions[] = {
	waitForSoundBufferLoading,
	loadVLALL,
	isSoundLoaded,
	loadMapSounds2,
	readVBALLSection,
	loadSB,
	isSoundBufferLoading,
	uploadSoundBuffer,
	loadSoundCompleteCallback,
	loadVHBFile,
	loadSoundFinishCallback,
	concatStrings2,
	loadFullVHB,
};

void loadFullVHB(int32_t vabId, char *path, uint8_t *buffer)
{
	SoundBuffer *sb;
	char *filename;
	char pathBuf[64];

	sb = &SOUND_BUFFERS[vabId];

	if ((filename = strrchr(path, '\\')) == NULL) {
		filename = path;
	} else {
		++filename;
	}

	concatStrings2(pathBuf, filename, ".VHB");
	sb->buffer = buffer;
	addFileReadRequestLookup(pathBuf, buffer, (uint8_t *)&sb->isLoading,
				 loadSoundFinishCallback, (void *)vabId);
}

void concatStrings2(char *dst, char *s1, char *s2)
{
	while (*s1) {
		*dst++ = *s1++;
	}

	while (*s2) {
		*dst++ = *s2++;
	}

	*dst = 0;
}

int32_t loadSoundFinishCallback(void *param)
{
	LOAD_SOUND_COMPLETE_STATE = 0;
	setFileReadCallback2(loadSoundCompleteCallback, param);
}

void loadVHBFile(int32_t vabId, char *path, uint8_t *buffer, int32_t offset,
		 int32_t sectors)
{
	SoundBuffer *sb;
	char *filename;
	char pathBuf[64];

	sb = &SOUND_BUFFERS[vabId];

	if ((filename = strrchr(path, '\\')) == NULL) {
		filename = path;
	} else {
		++filename;
	}

	concatStrings2(pathBuf, filename, ".VHB");

	sb->buffer = buffer;
	addFileReadRequestSection(pathBuf, buffer, offset, sectors,
				  (uint8_t *)&sb->isLoading,
				  loadSoundFinishCallback, (void *)vabId);
}

INCLUDE_ASM("asm/main/nonmatchings/sound_async", loadSoundCompleteCallback);

INCLUDE_ASM("asm/main/nonmatchings/sound_async", uploadSoundBuffer);

int32_t isSoundBufferLoading(int32_t vabId)
{
	SoundBuffer *sb;

	sb = &SOUND_BUFFERS[vabId];

	return sb->isLoading;
}

int32_t loadSB(void)
{
	ACTIVE_MAP_SOUND_ID = -1;

	loadFullVHB(8, SOUND_SB_PATH, GENERAL_BUFFER);

	return 8;
}

int32_t readVBALLSection(int32_t vabId, int32_t idx)
{
	SoundBuffer *sb;

	sb = &SOUND_BUFFERS[vabId];
	if ((vabId < 4) || (7 < vabId)) {
		sb->vabId = -1;
	}

	loadVHBFile(vabId, "VBALL", GENERAL_BUFFER,
		    DIGIMON_VBALL_SOUND_ID[idx] * 7, 7);

	return vabId;
}

int32_t loadMapSounds2(int32_t mapSoundId)
{
	MapSoundPara *para;
	uint32_t sectors;
	uint32_t offset;

	if (ACTIVE_MAP_SOUND_ID == mapSoundId) {
		return 1;
	}

	ACTIVE_MAP_SOUND_ID = mapSoundId;

	para = &MAP_SOUND_PARA[mapSoundId];
	sectors = para->sectorCount / 2;
	offset = MAP_SOUND_PARA[mapSoundId].sectorId / 2;

	loadVHBFile(8, "ESALL", GENERAL_BUFFER, offset, sectors);

	return 8;
}

int32_t isSoundLoaded(int32_t mode, int32_t vabId)
{
	SoundBuffer *sb;

	sb = &SOUND_BUFFERS[vabId];

	if (mode == 0) {
		while (sb->isLoading) {
			tickFileReadQueue(0);
		}
	} else if (sb->isLoading) {
		return -1;
	}

	return (sb->vabId == -1) ? 0 : 1;
}

int32_t loadVLALL(int32_t idx, uint8_t *buffer)
{
	uint32_t soundId;
	char *filename;
	char *path;
	char pathBuf[64];
	SoundBuffer *sb;

	readVBALLSection(4, idx);

	if ((idx >= 1) && (idx <= 0x41)) {
		soundId = DIGIMON_VLALL_SOUND_ID[idx];
	} else {
		soundId = 0xf;
	}

	path = "VLALL";
	sb = &SOUND_BUFFERS[3];

	if ((filename = strrchr(path, '\\')) == NULL) {
		filename = path;
	} else {
		++filename;
	}

	concatStrings2(pathBuf, filename, ".VHB");

	/* Original code stores buffer twice */
	sb->buffer = buffer;
	sb->buffer = buffer;

	addFileReadRequestSection(pathBuf, GENERAL_BUFFER, soundId * 0xf, 0xf,
				  (uint8_t *)&sb->isLoading, NULL, 0);

	return 3;
}

void waitForSoundBufferLoading(int32_t vabId)
{
	SoundBuffer *sb;

	sb = &SOUND_BUFFERS[vabId];
	while (sb->isLoading) {
		tickFileReadQueue(0);
	}

	uploadSoundBuffer(vabId);
}
