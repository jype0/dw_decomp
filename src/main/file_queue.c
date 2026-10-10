#include <libcd.h>
#include <string.h>
#include <sys/types.h>

#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/version.h>
#include <dw/world_object.h>

#define FILE_QUEUE_SIZE		32
#define FILE_REQUEST_NAME_LEN	29

typedef struct FileRequest {
	struct FileRequest *next;
	uint8_t *targetBuffer;
	uint8_t *isRunningPtr;
	FileRequestCallback finishCallback;
	void *finishCallbackParam;
	FileRequestCallback waitCallback;
	void *waitCallbackParam;
	int8_t state;
	char filename[FILE_REQUEST_NAME_LEN];
	CdlLOC pos;
	uint32_t size;
} FileRequest;

FileRequest FILE_REQUEST_QUEUE[FILE_QUEUE_SIZE];

static int32_t FILE_REQUEST_CALLBACK2;
static FileRequestCallback FILE_QUEUE_CALLBACK;
static void *FILE_QUEUE_CALLBACK_PARAM;
static FileRequest *FILE_READ_QUEUE_HEAD;
static FileRequest *FILE_READ_QUEUE_TAIL;

void initFileReadQueueTable(void);
void _tickFileReadQueue(int32_t instanceId);
void _renderFileReadQueue(int32_t instanceId);

#if VERSION_IS(EU)
static void *file_queue_functions[] = {
	addFileReadRequestLookup,
	addFileReadRequestSection,
	addFileReadRequest,
	addFileReadRequestPath,
	setFileReadCallback2,
	tickFileReadQueue,
	initializeFileReadQueue,
	_tickFileReadQueue,
	_renderFileReadQueue,
	initFileReadQueueTable,
};

static void *file_queue_sbss_order[] = {
	&FILE_READ_QUEUE_TAIL,
	&FILE_READ_QUEUE_HEAD,
	&FILE_REQUEST_CALLBACK2,
	&FILE_QUEUE_CALLBACK,
	&FILE_QUEUE_CALLBACK_PARAM,
};
#endif

static void file_queue__garbage__(void)
{
	FILE_REQUEST_CALLBACK2 = 0;
	FILE_QUEUE_CALLBACK = NULL;
	FILE_QUEUE_CALLBACK_PARAM = NULL;
	FILE_READ_QUEUE_HEAD = NULL;
	FILE_READ_QUEUE_TAIL = NULL;
}

void initFileReadQueueTable(void)
{
	int32_t i;

	FILE_READ_QUEUE_HEAD = NULL;
	FILE_READ_QUEUE_TAIL = NULL;

	for (i = 0; i < FILE_QUEUE_SIZE; ++i) {
		FILE_REQUEST_QUEUE[i].next = NULL;
		FILE_REQUEST_QUEUE[i].state = -1;
	}
}

void initializeFileReadQueue(void)
{
	initFileReadQueueTable();
	addObject(0x404, 0, _tickFileReadQueue, _renderFileReadQueue);
}

void _tickFileReadQueue(int32_t instanceId)
{
	tickFileReadQueue(0);
}

void _renderFileReadQueue(int32_t instanceId)
{
}

void tickFileReadQueue(int32_t unused)
{
	FileRequest *req;
	int32_t result;
	char pathBuf[64];
	CdlFILE file;
	uint8_t mode;

	if (FILE_READ_QUEUE_HEAD == NULL) {
		return;
	}
	if (0) {}

	req = FILE_READ_QUEUE_HEAD;
	if (0) {}
	if (req->state == 0) {
		req->state = -1;
		if (req->next == NULL) {
			FILE_READ_QUEUE_HEAD = NULL;
			FILE_READ_QUEUE_TAIL = NULL;
		} else {
			if (0) {}
			FILE_READ_QUEUE_HEAD = req->next;
			req->next = NULL;
			req = FILE_READ_QUEUE_HEAD;
		}
	}

	if (req->state == 2) {
		if ((int32_t)req->size < 0) {
			mode = CdlModeSpeed;
			pathBuf[0] = '\\';
			strcpy(&pathBuf[1], req->filename);
			strcat(pathBuf, ";1");
			if (CdSearchFile(&file, pathBuf) == NULL) {
				return;
			}

			req->pos = file.pos;
			req->size = file.size;
		}

		do {
			while (CdControl(CdlSetloc, (u_char *)&req->pos, NULL) == 0);
		} while (CdRead(((req->size + 0x7ff) & ~0x7ff) >> 11,
				(u_long *)req->targetBuffer, CdlModeSpeed) == 0);

		req->state = 1;

		return;
	}

	if (req->state == 1) {
		result = CdReadSync(1, NULL);
		if (result == 0) {
			FILE_REQUEST_CALLBACK2 = 0;
			if (req->finishCallback != NULL) {
				req->finishCallback(req->finishCallbackParam);
			}
			if (FILE_REQUEST_CALLBACK2 == 0) {
				goto done;
			}
			req->state = 10;
			req->waitCallback = FILE_QUEUE_CALLBACK;
			req->waitCallbackParam = FILE_QUEUE_CALLBACK_PARAM;
			goto wait;
		}
		goto err;
	}
wait:
	while (req->state == 10 &&
	       req->waitCallback(req->waitCallbackParam) == 0) {
done:
		req->state = 0;
		if (req->isRunningPtr != NULL) {
			*req->isRunningPtr = 0;
		}
		continue;
err:
		if (result < 0) {
			req->state = 2;
		}
	}
}

void setFileReadCallback2(FileRequestCallback callback, void *param)
{
	FILE_REQUEST_CALLBACK2 = 1;
	FILE_QUEUE_CALLBACK = callback;
	FILE_QUEUE_CALLBACK_PARAM = param;
}

int32_t addFileReadRequestPath(char *path, uint8_t *buffer,
			       uint8_t *isRunning,
			       FileRequestCallback callback,
			       void *callbackParam)
{
	return addFileReadRequest(path, buffer, isRunning, callback,
				  callbackParam, NULL, -1);
}

int32_t addFileReadRequest(char *path, uint8_t *buffer, uint8_t *isRunning,
			   FileRequestCallback callback, void *callbackParam,
			   CdlLOC *loc, int32_t size)
{
	FileRequest *slot;
	int32_t i;
	int32_t result;
	char ch;

	while (1) {
		for (slot = &FILE_REQUEST_QUEUE[0], result = 0;
		     result < FILE_QUEUE_SIZE;
		     ++slot, ++result) {
			if (slot->next != NULL) {
				continue;
			}

			if (slot->state < 0) {
				break;
			}
		}

		if (result != FILE_QUEUE_SIZE)
			break;

		tickFileReadQueue(0);
	}

	for (i = 0; i < FILE_REQUEST_NAME_LEN; ++i) {
		slot->filename[i] = *path;
		if (*path++ == '\0') {
			break;
		}
	}

	if (i == FILE_REQUEST_NAME_LEN) {
		return -1;
	}

	slot->next = NULL;

	if (isRunning != NULL) {
		*isRunning = 1;
	}

	slot->isRunningPtr = isRunning;
	slot->state = 2;
	slot->targetBuffer = buffer;
	slot->finishCallback = callback;
	slot->finishCallbackParam = callbackParam;
	slot->pos = *loc;
	slot->size = size;

	if (FILE_READ_QUEUE_HEAD == NULL) {
		FILE_READ_QUEUE_HEAD = slot;
		FILE_READ_QUEUE_TAIL = slot;
	} else {
		FILE_READ_QUEUE_TAIL->next = slot;
		FILE_READ_QUEUE_TAIL = slot;
	}

	return result;
}

int32_t addFileReadRequestSection(char *path, uint8_t *buffer, int32_t offset,
				  int32_t sectors, uint8_t *isRunning,
				  FileRequestCallback callback,
				  void *callbackParam)
{
	FileLookup lookup;

	if (lookupFileTable(&lookup, path) != 0) {
		CdIntToPos(offset + CdPosToInt(&lookup.pos), &lookup.pos);
		return addFileReadRequest(path, buffer, isRunning, callback,
					  callbackParam, &lookup.pos,
					  sectors << 11);
	}

	return -1;
}

int32_t addFileReadRequestLookup(char *path, uint8_t *buffer,
				 uint8_t *isRunning,
				 FileRequestCallback callback,
				 void *callbackParam)
{
	FileLookup lookup;

	if (lookupFileTable(&lookup, path) != 0) {
		return addFileReadRequest(path, buffer, isRunning, callback,
					  callbackParam, &lookup.pos,
					  lookup.size);
	}

	return -1;
}
