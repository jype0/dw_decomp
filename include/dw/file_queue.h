#ifndef DW_FILE_QUEUE_H
#define DW_FILE_QUEUE_H

#include <libcd.h>

#include <dw/types.h>

typedef int32_t (*FileRequestCallback)(void *param);

void initializeFileReadQueue(void);

void tickFileReadQueue(int32_t instanceId);

void setFileReadCallback2(FileRequestCallback callback, void *param);

int32_t addFileReadRequestPath(char *path, uint8_t *buffer,
			       uint8_t *isRunning,
			       FileRequestCallback callback,
			       void *callbackParam);
int32_t addFileReadRequest(char *path, uint8_t *buffer, uint8_t *isRunning,
			   FileRequestCallback callback, void *callbackParam,
			   CdlLOC *loc, int32_t size);
int32_t addFileReadRequestSection(char *path, uint8_t *buffer, int32_t offset,
				  int32_t sectors, uint8_t *isRunning,
				  FileRequestCallback callback,
				  void *callbackParam);
int32_t addFileReadRequestLookup(char *path, uint8_t *buffer,
				 uint8_t *isRunning,
				 FileRequestCallback callback,
				 void *callbackParam);

#endif
