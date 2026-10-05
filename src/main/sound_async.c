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

	if (NULL == (filename = strrchr(path, '\\'))) {
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

	if (NULL == (filename = strrchr(path, '\\'))) {
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

int32_t loadSoundCompleteCallback(void *param)
{
	SoundBuffer *sb;
	uint8_t *buffer;
	unsigned long addr;

	sb = &SOUND_BUFFERS[(int32_t)param];
	buffer = sb->buffer;

	switch (LOAD_SOUND_COMPLETE_STATE) {
	case 0:
		sb->vabId = 0;
		memcpy(VHB_HEADER_ADDR[(int32_t)param],
		       &buffer[(((VhbFileHeader *)buffer)->vhOffset >> 2) << 2],
		       ((VhbFileHeader *)buffer)->vbOffset - ((VhbFileHeader *)buffer)->vhOffset);

		SsVabClose((int32_t)param);
		if ((sb->vabId = SsVabOpenHeadSticky(VHB_HEADER_ADDR[(int32_t)param],
						     (int32_t)param,
						     VHB_SOUNDBUFFER_START[(int32_t)param])) < 0) {
			sb->vabId = -1;
			return 0;
		}

		if (sb->vabId != SsVabTransBody(&buffer[((VhbFileHeader *)buffer)->vbOffset], sb->vabId)) {
			sb->vabId = -1;
			return 0;
		}

		LOAD_SOUND_COMPLETE_STATE = 4;
		return 1;
	case 4:
		if (SsVabTransCompleted(0)) {
			LOAD_SOUND_COMPLETE_STATE = 5;
		}
		return 1;
	case 5:
		addr = SsUtGetVBaddrInSB(sb->vabId);
		return 0;
	}
}

void uploadSoundBuffer(int32_t vabId)
{
	SoundBuffer *sb;
	uint8_t *buffer;
	unsigned long addr;

	sb = &SOUND_BUFFERS[vabId];
	buffer = sb->buffer;
	sb->vabId = 0;
	memcpy(VHB_HEADER_ADDR[vabId],
	       &buffer[(((VhbFileHeader *)buffer)->vhOffset >> 2) << 2],
	       ((VhbFileHeader *)buffer)->vbOffset - ((VhbFileHeader *)buffer)->vhOffset);

	SsVabClose(vabId);
	if ((sb->vabId = SsVabOpenHeadSticky(VHB_HEADER_ADDR[vabId], vabId,
					     VHB_SOUNDBUFFER_START[vabId])) < 0) {
		sb->vabId = -1;
		return;
	}

	if (sb->vabId != SsVabTransBody(&buffer[((VhbFileHeader *)buffer)->vbOffset], sb->vabId)) {
		sb->vabId = -1;
		return;
	}

	SsVabTransCompleted(1);
	addr = SsUtGetVBaddrInSB(sb->vabId);
}

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
	int32_t soundId;

	sb = &SOUND_BUFFERS[vabId];
	if ((vabId < 4) || (7 < vabId)) {
		sb->vabId = -1;
	}

	soundId = DIGIMON_VBALL_SOUND_ID[idx];
	loadVHBFile(vabId, "VBALL", GENERAL_BUFFER, soundId * 7, 7);

	return vabId;
}

int32_t loadMapSounds2(int32_t mapSoundId)
{
	if (ACTIVE_MAP_SOUND_ID == mapSoundId) {
		return 1;
	}

	ACTIVE_MAP_SOUND_ID = mapSoundId;

	loadVHBFile(8, "ESALL", GENERAL_BUFFER,
		    MAP_SOUND_PARA[mapSoundId].sectorId / 2,
		    MAP_SOUND_PARA[mapSoundId].sectorCount / 2);

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

	if (sb->vabId == -1) {
		return 0;
	}

	return 1;
}

int32_t loadVLALL(int32_t idx, uint8_t *buffer)
{
	uint32_t soundId;
	int32_t vabId;
	char *path;
	uint8_t *vhbBuffer;
	char pathBuf[64];
	SoundBuffer *sb;
	char *filename;

	readVBALLSection(4, idx);

	if ((idx >= 1) && (idx <= 0x41)) {
		soundId = DIGIMON_VLALL_SOUND_ID[idx];
	} else {
		soundId = 0xf;
	}

	vabId = 3;
	path = "VLALL";
	vhbBuffer = buffer;
	sb = &SOUND_BUFFERS[vabId];

	if (NULL == (filename = strrchr(path, '\\'))) {
		filename = path;
	} else {
		++filename;
	}

	concatStrings2(pathBuf, filename, ".VHB");

	sb->buffer = vhbBuffer;
	sb->buffer = vhbBuffer;

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
