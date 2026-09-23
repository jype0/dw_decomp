#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libpress.h>

#include <dw/mov.h>
#include <dw/types.h>

typedef struct {
	char path[0x20];
	int32_t startFrame;
	int32_t frameCount;
} MovieEntry;

extern int32_t MAIN_D_80139BA0[];

void MAIN_func_80092A44(DISPENV *disp, int32_t x, int32_t y, int32_t w, int32_t h);

void MOV_initEnvironment(MovieEnv *env, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void MOV_onSliceDecoded(void);
void MOV_startStream(CdlFILE *file, void (*callback)(), MovieEntry *movie);
void MOV_startRead(CdlLOC *loc);
int32_t MOV_decodeFrame(MovieEnv *env, MovieEntry *movie);
void MOV_waitForFlip(MovieEnv *env, int32_t unused);
u_long *MOV_readFrame(MovieEnv *env, MovieEntry *movie);

static void *mov_functions[] = {
	MOV_playMovie,
	MOV_readFrame,
	MOV_waitForFlip,
	MOV_decodeFrame,
	MOV_startRead,
	MOV_startStream,
	MOV_onSliceDecoded,
	MOV_initEnvironment,
};

int32_t MOVIE_HEIGHT;
int32_t MOVIE_WIDTH;
int32_t MOVIE_COMPLETED;

MovieEntry MOV_MOVIES[4] = {
	{ "\\MOVIE\\OP1.STR;1", 1, 3637 },
	{ "\\MOVIE\\OP2.STR;1", 1, 3266 },
	{ "\\MOVIE\\ED2.STR;1", 1, 2654 },
	{ "\\MOVIE\\EDR.STR;1", 1, 3447 },
};

void MOV_initEnvironment(MovieEnv *env, int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
	env->runLevelBuffer[0] = MOV_RUN_LEVEL_BUFFER_0;
	env->runLevelBuffer[1] = MOV_RUN_LEVEL_BUFFER_1;
	env->runLevelIndex = 0;
	env->sliceBuffer[0] = MOV_SLICE_BUFFER_0;
	env->sliceBuffer[1] = MOV_SLICE_BUFFER_1;
	env->sliceIndex = 0;
	env->frame[0].x = x0;
	env->frame[0].y = y0;
	env->frame[1].x = x1;
	env->frame[1].y = y1;
	env->frameIndex = 0;
	env->slice.x = x0;
	env->slice.y = y0;
	env->slice.w = 24;
	env->flipPending = 0;
}

void MOV_onSliceDecoded(void)
{
	RECT rect;
	int32_t index;

	if (MAIN_D_80139BA0[0] != 0) {
		StCdInterrupt();
		MAIN_D_80139BA0[0] = 0;
	}

	index = MOV_ENV.sliceIndex;
	rect = MOV_ENV.slice;
	MOV_ENV.sliceIndex = (MOV_ENV.sliceIndex != 0) ? 0 : 1;
	MOV_ENV.slice.x = MOV_ENV.slice.x + MOV_ENV.slice.w;
	if (MOV_ENV.slice.x < (MOV_ENV.frame[MOV_ENV.frameIndex].x + MOV_ENV.frame[MOV_ENV.frameIndex].w)) {
		DecDCTout(MOV_ENV.sliceBuffer[MOV_ENV.sliceIndex], (MOV_ENV.slice.w * MOV_ENV.slice.h) / 2);
	} else {
		MOV_ENV.flipPending = 1;
		MOV_ENV.frameIndex = (MOV_ENV.frameIndex != 0) ? 0 : 1;
		MOV_ENV.slice.x = MOV_ENV.frame[MOV_ENV.frameIndex].x;
		MOV_ENV.slice.y = MOV_ENV.frame[MOV_ENV.frameIndex].y;
	}

	LoadImage(&rect, MOV_ENV.sliceBuffer[index]);
}

void MOV_startStream(CdlFILE *file, void (*callback)(), MovieEntry *movie)
{
	DecDCTReset(0);
	DecDCToutCallback(callback);
	StSetRing(MOV_RING_BUFFER, 0x18);
	StSetStream(1, movie->startFrame, -1, NULL, NULL);
	MOV_startRead(&file->pos);
}

void MOV_startRead(CdlLOC *loc)
{
	uint8_t mode;

	mode = CdlModeSpeed;

	do {
		while (CdControl(CdlSetloc, (uint8_t *)loc, NULL) == 0) {
		}
		while (CdControl(CdlSetmode, &mode, NULL) == 0) {
		}
		VSync(3);
	} while (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT | CdlModeSize1) == 0);
}

int32_t MOV_decodeFrame(MovieEnv *env, MovieEntry *movie)
{
	u_long *bs;
	int32_t retries;

	retries = 2000;

	while ((bs = MOV_readFrame(env, movie)) == NULL) {
		if (--retries == 0) {
			return -1;
		}
	}

	env->runLevelIndex = (env->runLevelIndex != 0) ? 0 : 1;
	DecDCTvlc2(bs, env->runLevelBuffer[env->runLevelIndex], MOV_VLC_TABLE);
	StFreeRing(bs);

	return 0;
}

void MOV_waitForFlip(MovieEnv *env, int32_t unused)
{
	uint32_t timeout;

	timeout = 0x800000;
	/* Force variable onto the stack */
	(void)&timeout;

	while (env->flipPending == 0) {
		if (--timeout == 0) {
			env->flipPending = 1;
			env->frameIndex = (env->frameIndex != 0) ? 0 : 1;
			env->slice.x = env->frame[env->frameIndex].x;
			env->slice.y = env->frame[env->frameIndex].y;
		}
	}

	env->flipPending = 0;
}

u_long *MOV_readFrame(MovieEnv *env, MovieEntry *movie)
{
	u_long *addr;
	StHEADER *header;
	RECT rect;
	int32_t retries;

	retries = 2000;

	while (StGetNext(&addr, (u_long **)&header) != 0) {
		if (--retries == 0) {
			return NULL;
		}
	}

	if (header->frameCount >= (uint32_t)movie->frameCount) {
		MOVIE_COMPLETED = 1;
	}

	if ((MOVIE_WIDTH != header->width) || (MOVIE_HEIGHT != header->height)) {
		setRECT(&rect, 0, 0, 0x1e0, 0x1e0);
		ClearImage(&rect, 0, 0, 0);
		MOVIE_WIDTH = header->width;
		MOVIE_HEIGHT = header->height;
	}

	setWH(&env->frame[0], env->frame[1].w = (MOVIE_WIDTH * 3) / 2, env->frame[1].h = MOVIE_HEIGHT);
	env->slice.h = MOVIE_HEIGHT;

	return addr;
}

int32_t MOV_playMovie(int32_t movieId)
{
	DISPENV disp;
	DRAWENV draw;
	CdlFILE file;
	CdlLOC loc;
	RECT rect;
	MovieEntry *movie;

	movie = &MOV_MOVIES[movieId];

	if (CdSearchFile(&file, movie->path) == NULL) {
		return 0;
	}

	MOV_initEnvironment(&MOV_ENV, 0, 32, 0, 272);
	MOV_startStream(&file, MOV_onSliceDecoded, movie);
	DecDCTvlcBuild(MOV_VLC_TABLE);

	while (MOV_decodeFrame(&MOV_ENV, movie) == -1) {
		loc = file.pos;
		MOV_startRead(&loc);
	}

	MOVIE_COMPLETED = 0;
	setRECT(&rect, 0, 0, 0x1e0, 0x1e0);
	ClearImage(&rect, 0, 0, 0);
	DrawSync(0);

	do {
		DecDCTin(MOV_ENV.runLevelBuffer[MOV_ENV.runLevelIndex], 3);
		DecDCTout(MOV_ENV.sliceBuffer[MOV_ENV.sliceIndex], (MOV_ENV.slice.w * MOV_ENV.slice.h) / 2);
		while (MOV_decodeFrame(&MOV_ENV, movie) == -1) {
			int32_t backloc;

			backloc = StGetBackloc(&loc);
			if ((backloc <= 0) || (movie->frameCount < backloc)) {
				loc = file.pos;
			}
			MOV_startRead(&loc);
		}
		MOV_waitForFlip(&MOV_ENV, 0);
		VSync(0);
		MAIN_func_80092A44(&disp, 0, ((MOV_ENV.frameIndex != 0) ? 0 : 1) * 240, 0x1e0, 0xf0);
		disp.isrgb24 = 1;
		disp.disp.w = (disp.disp.w * 2) / 3;
		PutDispEnv(&disp);
		PutDrawEnv(&draw);
		SetDispMask(1);
	} while ((MOVIE_COMPLETED != 1) && ((movieId == 3) || ((PadRead(1) & 0x800) == 0)));

	DecDCToutCallback(NULL);
	StUnSetRing();
	CdControlB(CdlPause, NULL, NULL);
	ClearImage(&rect, 0, 0, 0);
	DrawSync(0);

	return (MOVIE_COMPLETED == 0) ? 0 : 1;
}
