#include <libetc.h>
#include <libsnd.h>
#include <libspu.h>
#include <string.h>

#include <dw/file.h>
#include <dw/main.h>
#include <dw/sound.h>
#include <dw/types.h>

#include "common.h"

extern char MAIN_D_80127E88[];
extern char MAIN_D_80127E94[];
extern char MAIN_D_80127EA0[];

int32_t FREE_CHANNEL_INDEX;
int16_t SEQ_ACCESS_NUM;
int16_t CURRENT_SEQ_FONT;
int16_t CURRENT_SEQ_TRACK;

char SEQ_TABLE[0xB0];
uint8_t SEQ_BUFFER[0x5678];

char VHB_EXT[] = ".VHB";

int32_t ACTIVE_MAP_SOUND_ID = -1;

int32_t readVHBFile(int32_t vabId, char *filename, uint8_t *buffer);
int32_t readVHBFileSectors(int32_t vabId, char *filename, uint8_t *buffer,
			   uint32_t offset, int32_t sectors);
int32_t loadMusicFont(int32_t font);
uint32_t getNextFreeChannel(int32_t arg);
uint32_t startSound(int32_t vabId, char prog, char note);
void seqOpen(void);
void seqPlay(void);
void seqStop(void);
void seqClose(void);
void concatStrings3(char *dst, char *src1, char *src2);
void _stopSoundMask(uint32_t mask);

static void *sound_functions[] = {
	concatStrings3,
	loadMusicFont,
	seqPlay,
	seqOpen,
	readVHBFileSectors,
	seqClose,
	seqStop,
	readVHBFile,
};

static void *sound_bss[] = {
	SEQ_BUFFER,
	SEQ_TABLE,
	&CURRENT_SEQ_TRACK,
	&CURRENT_SEQ_FONT,
	&SEQ_ACCESS_NUM,
	&FREE_CHANNEL_INDEX,
};

int32_t readVHBFile(int32_t vabId, char *filename, uint8_t *buffer)
{
	uint32_t *words;
	short vabid;
	char *str;
	char pathBuf[64];

	words = (uint32_t *)buffer;

	if ((str = strrchr(filename, '\\')) == NULL) {
		str = filename;
	} else {
		++str;
	}

	concatStrings3(pathBuf, str, VHB_EXT);
	readFile(pathBuf, buffer);
	memcpy(VHB_HEADER_ADDR[vabId], &buffer[(words[0] >> 2) << 2],
	       words[1] - words[0]);

	SsVabClose(vabId);
	if ((vabid = SsVabOpenHeadSticky(VHB_HEADER_ADDR[vabId], vabId,
				    VHB_SOUNDBUFFER_START[vabId])) < 0) {
		return -1;
	}

	if (SsVabTransBody(&buffer[words[1]], vabid) != vabid) {
		return -1;
	}

	SsVabTransCompleted(1);
	SsUtGetVBaddrInSB(vabid);

	return vabid;
}

void seqStop(void)
{
	if (SEQ_ACCESS_NUM == -1) {
		return;
	}

	SsSeqStop(SEQ_ACCESS_NUM);
}

void seqClose(void)
{
	if (SEQ_ACCESS_NUM == -1) {
		return;
	}

	SsSeqClose(SEQ_ACCESS_NUM);
	SEQ_ACCESS_NUM = -1;
}

int32_t readVHBFileSectors(int32_t vabId, char *filename, uint8_t *buffer,
			   uint32_t offset, int32_t sectors)
{
	uint32_t *words;
	short vabid;
	char *str;
	char pathBuf[64];

	words = (uint32_t *)buffer;

	if ((str = strrchr(filename, '\\')) == NULL) {
		str = filename;
	} else {
		++str;
	}

	concatStrings3(pathBuf, str, VHB_EXT);
	readFileSectors(pathBuf, buffer, offset, sectors);
	memcpy(VHB_HEADER_ADDR[vabId], &buffer[(words[0] >> 2) << 2],
	       words[1] - words[0]);

	SsVabClose(vabId);
	if ((vabid = SsVabOpenHeadSticky(VHB_HEADER_ADDR[vabId], vabId,
					 VHB_SOUNDBUFFER_START[vabId])) < 0) {
		return -1;
	}

	if (SsVabTransBody(&buffer[words[1]], vabid) != vabid) {
		return -1;
	}

	SsVabTransCompleted(1);
	SsUtGetVBaddrInSB(vabid);

	return vabid;
}

void seqOpen(void)
{
	uint32_t *buf;
	uint32_t trackCount;
	int32_t trackOffset;

	buf = (uint32_t *)SEQ_BUFFER;
	trackCount = buf[0] >> 2;
	if (trackCount <= CURRENT_SEQ_TRACK) {
		CURRENT_SEQ_TRACK = 0;
	}

	trackOffset = buf[CURRENT_SEQ_TRACK];
	SEQ_ACCESS_NUM = SsSeqOpen((unsigned long *)&((uint8_t *)buf)[trackOffset], 2);
}

void seqPlay(void)
{
	if (SEQ_ACCESS_NUM == -1) {
		return;
	}

	SsSeqSetVol(SEQ_ACCESS_NUM, 0x50, 0x50);

	if (((CURRENT_SEQ_FONT == 0x1d) && (CURRENT_SEQ_TRACK != 1)) ||
	    ((CURRENT_SEQ_FONT == 0x1e) && (CURRENT_SEQ_TRACK != 2)) ||
	    ((CURRENT_SEQ_FONT == 0x1f) && (CURRENT_SEQ_TRACK != 2)) ||
	    ((CURRENT_SEQ_FONT == 0x20) && (CURRENT_SEQ_TRACK != 2))) {
		SsSeqPlay(SEQ_ACCESS_NUM, 1, 1);
	} else {
		SsSeqPlay(SEQ_ACCESS_NUM, 1, 0);
	}
}

void concatStrings3(char *dst, char *src1, char *src2)
{
	while (*src1 != 0) {
		*dst++ = *src1++;
	}
	while (*src2 != 0) {
		*dst++ = *src2++;
	}
	*dst = 0;
}

int32_t initializeMusic(void)
{
	SpuReverbAttr attr;

	ResetCallback();
	SsInit();
	SsSetTableSize(SEQ_TABLE, 1, 1);
	SsSetTickMode(2);

	if (readVHBFile(0, MAIN_D_80127E88, GENERAL_BUFFER) == -1) {
		return 0;
	}

	if (readVHBFile(1, MAIN_D_80127E94, GENERAL_BUFFER) == -1) {
		return 0;
	}

	SsStart();
	SsSetMVol(127, 127);

	SEQ_ACCESS_NUM = -1;
	CURRENT_SEQ_FONT = -1;

	attr.mask = 7;
	attr.mode = 0x103;
	attr.depth.right = 0x7000;
	attr.depth.left = 0x7000;
	SpuSetReverbModeParam(&attr);
	SpuClearReverbWorkArea(3);
	SpuSetReverbDepth(&attr);
	SpuSetReverbVoice(1, 0x7fffff);
	SpuSetReverb(1);

	return 1;
}

void finalizeMusic(void)
{
	int32_t i;

	seqStop();
	seqClose();

	for (i = 0; i < 10; ++i) {
		SsVabClose(i);
	}

	SsEnd();
	SsQuit();
}

uint32_t getNextFreeChannel(int32_t arg)
{
	int32_t i;
	int32_t val;

	for (i = 0; i < 14; ++i) {
		val = (i + FREE_CHANNEL_INDEX) % 14 + 10;
		if (SpuGetKeyStatus(1 << val) != 1) {
			break;
		}
	}

	if (i == 14) {
		val = FREE_CHANNEL_INDEX + 10;
	}

	FREE_CHANNEL_INDEX = (val - 9) % 14;

	return val;
}

INCLUDE_ASM("asm/main/nonmatchings/sound", startSound);

void _stopSoundMask(uint32_t mask)
{
	int32_t i;

	for (i = 0; i < 0x18; ++i) {
		if ((mask & (1 << i)) != 0) {
			SsUtAutoVol(i, 0x7f, 0, 6);
			SsUtKeyOffV(i);
		}
	}
}

uint32_t playSound(int32_t vabId, int32_t val)
{
	if ((vabId == 0) || (vabId == 1) || (vabId == 8)) {
		return startSound(vabId, val / 16, (val % 16) + 0x3c);
	} else if ((2 < vabId) && (vabId < 8)) {
		return startSound(vabId, 0, val);
	}
}

void stopSound(void)
{
	int32_t i;

	SsUtAllKeyOff(0);

	for (i = 10; i < 0x18; ++i) {
		if (SpuGetKeyStatus(1 << i) == 1) {
			SsUtAutoVol(i, 0x7f, 0, 6);
			SsUtKeyOffV(i);
		}
	}
}

uint32_t playSound2(int32_t vabId, int32_t val)
{
	return startSound(vabId, val / 16, (val % 16) + 0x3c);
}

void stopSoundMask(uint32_t mask)
{
	_stopSoundMask(mask);
}

int32_t loadPartnerSounds(int32_t type)
{
	int32_t soundId;

	if ((type > 0) && (type < 66)) {
		soundId = DIGIMON_VLALL_SOUND_ID[type];
	} else {
		soundId = 15;
	}

	if (readVHBFileSectors(3, "VLALL", GENERAL_BUFFER, soundId * 15,
			       15) == -1) {
		return 0;
	}

	return loadDigimonSounds(4, type);
}

int32_t loadDigimonSounds(int32_t vabId, int32_t type)
{
	if ((vabId < 4) || (7 < vabId)) {
		return 0;
	}

	if (readVHBFileSectors(vabId, "VBALL", GENERAL_BUFFER,
			       DIGIMON_VBALL_SOUND_ID[type] * 7, 7) == -1) {
		return 0;
	}

	return 1;
}

int32_t loadVSSounds(void)
{
	ACTIVE_MAP_SOUND_ID = -1;

	if (readVHBFile(8, MAIN_D_80127EA0, GENERAL_BUFFER) == -1) {
		return 0;
	}

	return 1;
}

int32_t loadMapSounds(int32_t mapSoundId)
{
	if (ACTIVE_MAP_SOUND_ID == mapSoundId) {
		return 1;
	}

	ACTIVE_MAP_SOUND_ID = mapSoundId;

	if (readVHBFileSectors(8, "ESALL", GENERAL_BUFFER,
			       MAP_SOUND_PARA[mapSoundId].sectorId / 2,
			       MAP_SOUND_PARA[mapSoundId].sectorCount / 2) == -1) {
		return 0;
	}

	return 1;
}

int32_t loadMusicFont(int32_t font)
{
	uint32_t start;
	uint32_t end;

	if (readVHBFileSectors(2, "FAALL", GENERAL_BUFFER,
			       (font - 1) * 0x27, 0x27) == -1) {
		return 0;
	}

	start = ((uint32_t *)GENERAL_BUFFER)[2];
	end = ((uint32_t *)GENERAL_BUFFER)[3];
	memcpy(SEQ_BUFFER, &((uint8_t *)GENERAL_BUFFER)[(start >> 2) << 2],
	       end - start);

	return 1;
}

int32_t playMusic(int32_t font, int32_t track)
{
	if ((font <= 0) || (0x21 < font)) {
		return 0;
	}

	if ((font == CURRENT_SEQ_FONT) && (track == CURRENT_SEQ_TRACK)) {
		return 1;
	}

	seqStop();
	seqClose();

	if (CURRENT_SEQ_FONT != font) {
		loadMusicFont(font);
		CURRENT_SEQ_FONT = font;
	}

	CURRENT_SEQ_TRACK = track;

	seqOpen();
	seqPlay();

	return 1;
}

void stopBGM(void)
{
	seqStop();
	seqClose();
	CURRENT_SEQ_TRACK = -1;
}
