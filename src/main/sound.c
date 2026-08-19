#include <libetc.h>
#include <libsnd.h>
#include <libspu.h>
#include <string.h>

#include <dw/file.h>
#include <dw/main.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>

#include "common.h"

uint8_t VHB_HEADER_SS[0x1000];
uint8_t VHB_HEADER_SL[0x1000];
uint8_t VHB_HEADER_FAALL[0x2000];
uint8_t VHB_HEADER_VLALL[0x1000];
uint8_t VHB_HEADER_VBALL_0[0x1000];
uint8_t VHB_HEADER_VBALL_1[0x1000];
uint8_t VHB_HEADER_VBALL_2[0x1000];
uint8_t VHB_HEADER_VBALL_3[0x1000];
uint8_t VHB_HEADER_ESALL_SB[0x1800];

void *VHB_HEADER_ADDR[NUM_SOUND_BUFFERS] = {
	VHB_HEADER_SS,
	VHB_HEADER_SL,
	VHB_HEADER_FAALL,
	VHB_HEADER_VLALL,
	VHB_HEADER_VBALL_0,
	VHB_HEADER_VBALL_1,
	VHB_HEADER_VBALL_2,
	VHB_HEADER_VBALL_3,
	VHB_HEADER_ESALL_SB,
	NULL,
};

unsigned long VHB_SOUNDBUFFER_START[NUM_SOUND_BUFFERS] = {
	0x1010,
	0xe800,
	0x1b800,
	0x2a000,
	0x31800,
	0x34000,
	0x36800,
	0x39000,
	0x3b800,
	0x0,
};

uint8_t DIGIMON_VLALL_SOUND_ID[NUM_PARTNER_DIGIMON] = {
	0x00,	/* main character */
	0x00,	/* Botamon */
	0x00,	/* Koromon */
	0x01,	/* Agumon */
	0x01,	/* Betamon */
	0x02,	/* Greymon */
	0x03,	/* Devimon */
	0x04,	/* Airdramon */
	0x02,	/* Tyrannomon */
	0x0b,	/* Meramon */
	0x05,	/* Seadramon */
	0x06,	/* Numemon */
	0x0a,	/* MetalGreymon */
	0x00,	/* Mamemon */
	0x0f,	/* Monzaemon */
	0x00,	/* Punimon */
	0x00,	/* Tsunomon */
	0x01,	/* Gabumon */
	0x01,	/* Elecmon */
	0x08,	/* Kabuterimon */
	0x0b,	/* Angemon */
	0x04,	/* Birdramon */
	0x07,	/* Garurumon */
	0x03,	/* Frigimon */
	0x05,	/* Whamon */
	0x0e,	/* Vegiemon */
	0x02,	/* SkullGreymon */
	0x00,	/* MetalMamemon */
	0x0a,	/* Vademon */
	0x00,	/* Poyomon */
	0x00,	/* Tokomon */
	0x01,	/* Patamon */
	0x01,	/* Kunemon */
	0x09,	/* Unimon */
	0x03,	/* Ogremon */
	0x06,	/* Shellmon */
	0x09,	/* Centarumon */
	0x00,	/* Bakemon */
	0x07,	/* Drimogemon */
	0x06,	/* Sukamon */
	0x0a,	/* Andromon */
	0x0a,	/* Giromon */
	0x0c,	/* Etemon */
	0x00,	/* Yuramon */
	0x00,	/* Tanemon */
	0x01,	/* Biyomon */
	0x01,	/* Palmon */
	0x07,	/* Monochromon */
	0x03,	/* Leomon */
	0x05,	/* Coelamon */
	0x04,	/* Kokatorimon */
	0x08,	/* Kuwagamon */
	0x03,	/* Mojyamon */
	0x03,	/* Nanimon */
	0x02,	/* Megadramon */
	0x00,	/* Piximon */
	0x01,	/* Digitamamon */
	0x01,	/* Penguinmon */
	0x00,	/* Ninjamon */
	0x04,	/* Phoenixmon */
	0x08,	/* H-Kabuterimon */
	0x05,	/* MegaSeadramon */
	0x07,	/* WereGarurumon */
	0x03,	/* Panjyamon */
	0x02,	/* Gigadramon */
	0x0c,	/* MetalEtemon */
};

uint8_t DIGIMON_VBALL_SOUND_ID[NUM_DIGIMON] = {
	0x00,	/* main character */
	0x01,	/* Botamon */
	0x02,	/* Koromon */
	0x03,	/* Agumon */
	0x04,	/* Betamon */
	0x05,	/* Greymon */
	0x06,	/* Devimon */
	0x07,	/* Airdramon */
	0x08,	/* Tyrannomon */
	0x09,	/* Meramon */
	0x0a,	/* Seadramon */
	0x0b,	/* Numemon */
	0x0c,	/* MetalGreymon */
	0x0d,	/* Mamemon */
	0x0e,	/* Monzaemon */
	0x01,	/* Punimon */
	0x02,	/* Tsunomon */
	0x0f,	/* Gabumon */
	0x10,	/* Elecmon */
	0x11,	/* Kabuterimon */
	0x12,	/* Angemon */
	0x13,	/* Birdramon */
	0x14,	/* Garurumon */
	0x15,	/* Frigimon */
	0x16,	/* Whamon */
	0x17,	/* Vegiemon */
	0x18,	/* SkullGreymon */
	0x19,	/* MetalMamemon */
	0x1a,	/* Vademon */
	0x01,	/* Poyomon */
	0x02,	/* Tokomon */
	0x1b,	/* Patamon */
	0x1c,	/* Kunemon */
	0x1d,	/* Unimon */
	0x1e,	/* Ogremon */
	0x1f,	/* Shellmon */
	0x20,	/* Centarumon */
	0x21,	/* Bakemon */
	0x22,	/* Drimogemon */
	0x23,	/* Sukamon */
	0x24,	/* Andromon */
	0x25,	/* Giromon */
	0x26,	/* Etemon */
	0x01,	/* Yuramon */
	0x02,	/* Tanemon */
	0x27,	/* Biyomon */
	0x28,	/* Palmon */
	0x29,	/* Monochromon */
	0x2a,	/* Leomon */
	0x2b,	/* Coelamon */
	0x2c,	/* Kokatorimon */
	0x2d,	/* Kuwagamon */
	0x2e,	/* Mojyamon */
	0x2f,	/* Nanimon */
	0x30,	/* Megadramon */
	0x31,	/* Piximon */
	0x32,	/* Digitamamon */
	0x33,	/* Penguinmon */
	0x34,	/* Ninjamon */
	0x35,	/* Phoenixmon */
	0x36,	/* H-Kabuterimon */
	0x37,	/* MegaSeadramon */
	0x00,	/* WereGarurumon */
	0x2a,	/* Panjyamon */
	0x30,	/* Gigadramon */
	0x26,	/* MetalEtemon */
	0x38,	/* Myotismon */
	0x39,	/* Yanmamon */
	0x3a,	/* Gotsumon */
	0x3b,	/* Flarerizamon */
	0x3c,	/* WaruMonzaemon */
	0x03,	/* SnowAgumon */
	0x1e,	/* Hyogamon */
	0x23,	/* PlatinumSukamon */
	0x1c,	/* Dokunemon */
	0x1d,	/* ShimaUnimon */
	0x3d,	/* Tankmon */
	0x3e,	/* RedVegiemon */
	0x3f,	/* J-Mojyamon */
	0x22,	/* NiseDrimogemon */
	0x40,	/* Goburimon */
	0x41,	/* MudFrigimon */
	0x0f,	/* Psychemon */
	0x04,	/* ModokiBetamon */
	0x03,	/* ToyAgumon */
	0x12,	/* Piddomon */
	0x28,	/* Aruraumon */
	0x0b,	/* Geremon */
	0x29,	/* Vermilimon */
	0x1e,	/* Fugamon */
	0x25,	/* Tekkamon */
	0x1f,	/* MoriShellmon */
	0x24,	/* Guardromon */
	0x33,	/* Muchomon */
	0x3a,	/* Icemon */
	0x2c,	/* Akatorimon */
	0x1b,	/* Tsukaimon */
	0x40,	/* Sharmamon */
	0x03,	/* ClearAgumon */
	0x17,	/* Weedmon */
	0x06,	/* IceDevimon */
	0x3b,	/* Darkrizamon */
	0x39,	/* SandYanmamon */
	0x40,	/* SnowGoburimon */
	0x09,	/* BlueMeramon */
	0x14,	/* Gururumon */
	0x13,	/* Saberdramon */
	0x21,	/* Soulmon */
	0x15,	/* Rockmon */
	0x42,	/* Otamamon */
	0x43,	/* Gekomon */
	0x44,	/* Tentomon */
	0x45,	/* WaruSeadramon */
	0x3a,	/* Meteormon */
	0x46,	/* ? */
	0x30,	/* Machinedramon */
	0x00,	/* Analogman */
	0x00,	/* Jijimon */
	0x00,	/* Market Manager */
	0x00,	/* ShogunGekomon */
	0x00,	/* King of Sukamon */
	0x00,	/* Cherrymon */
	0x00,	/* Hagurumon */
	0x00,	/* Tinmon */
	0x00,	/* Master Tyrannomon */
	0x00,	/* Goburimon */
	0x00,	/* Brachiomon */
	0x00,	/* DemiMeramon */
	0x04,	/* Betamon */
	0x05,	/* Greymon */
	0x06,	/* Devimon */
	0x07,	/* Airdramon */
	0x08,	/* Tyrannomon */
	0x09,	/* Meramon */
	0x0a,	/* Seadramon */
	0x0b,	/* Numemon */
	0x0c,	/* MetalGreymon */
	0x0d,	/* Mamemon */
	0x0e,	/* Monzaemon */
	0x0f,	/* Gabumon */
	0x10,	/* Elecmon */
	0x11,	/* Kabuterimon */
	0x12,	/* Angemon */
	0x13,	/* Birdramon */
	0x14,	/* Garurumon */
	0x15,	/* Frigimon */
	0x16,	/* Whamon */
	0x17,	/* Vegiemon */
	0x18,	/* SkullGreymon */
	0x19,	/* MetalGreymon */
	0x1a,	/* Vademon */
	0x1b,	/* Patamon */
	0x1c,	/* Kunemon */
	0x1d,	/* Unimon */
	0x1e,	/* Ogremon */
	0x1f,	/* Shellmon */
	0x20,	/* Centarumon */
	0x21,	/* Bakemon */
	0x22,	/* Drimogemon */
	0x23,	/* Sukamon */
	0x24,	/* Andromon */
	0x25,	/* Giromon */
	0x26,	/* Etemon */
	0x27,	/* Biyomon */
	0x28,	/* Palmon */
	0x29,	/* Monochromon */
	0x2a,	/* Leomon */
	0x2b,	/* Coelamon */
	0x2c,	/* Kokatorimon */
	0x2d,	/* Kuwagamon */
	0x2e,	/* Mojyamon */
	0x2f,	/* Nanimon */
	0x30,	/* Megadramon */
	0x31,	/* Piximon */
	0x32,	/* Digitamamon */
	0x34,	/* Ninjamon */
	0x33,	/* Penguinmon */
	0x38,	/* Myotismon */
	0x05,	/* Greymon */
	0x0c,	/* MetalGreymon */
};

MapSoundPara MAP_SOUND_PARA[22] = {
	{ 0x0000, 0x00e8 },
	{ 0x00e8, 0x0064 },
	{ 0x014c, 0x00de },
	{ 0x022a, 0x004c },
	{ 0x0276, 0x00f8 },
	{ 0x036e, 0x00e6 },
	{ 0x0454, 0x00ee },
	{ 0x0542, 0x0046 },
	{ 0x0588, 0x00de },
	{ 0x0666, 0x0086 },
	{ 0x06ec, 0x0038 },
	{ 0x0724, 0x00f8 },
	{ 0x081c, 0x00ee },
	{ 0x090a, 0x00a4 },
	{ 0x09ae, 0x009e },
	{ 0x0a4c, 0x0062 },
	{ 0x0aae, 0x0062 },
	{ 0x0b10, 0x00b2 },
	{ 0x0bc2, 0x003e },
	{ 0x0c00, 0x00bc },
	{ 0x0cbc, 0x007e },
	{ 0x0d3a, 0x00e2 },
};

int32_t FREE_CHANNEL_INDEX;
int16_t SEQ_ACCESS_NUM;
int16_t CURRENT_SEQ_FONT;
int16_t CURRENT_SEQ_TRACK;

char SEQ_TABLE[0xb0];
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
	SEQ_ACCESS_NUM =
		SsSeqOpen((unsigned long *)&((uint8_t *)buf)[trackOffset], 2);
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

	if (readVHBFile(0, "SOUND\\SS", GENERAL_BUFFER) == -1) {
		return 0;
	}

	if (readVHBFile(1, "SOUND\\SL", GENERAL_BUFFER) == -1) {
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

	for (i = 0; i < NUM_SOUND_BUFFERS; ++i) {
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

	if (readVHBFile(8, "SOUND\\SB", GENERAL_BUFFER) == -1) {
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
