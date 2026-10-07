#include <libgs.h>

#include <dw/model.h>
#include <dw/world_object.h>

extern TMDModel *BUFF_MODEL[];

void tickBuffModel(int32_t instanceId);
void renderBuffModel(void);
void morphBuffModel(int32_t model, int32_t compIdx, int32_t color);
void initializeBuffModel(TMDModel *model);
int32_t initializeBuffModelObject(void);
int32_t removeBuffModelObject(void);

int32_t BUFF_MODEL_MORPH_VALUE[2];
int32_t BUFF_MODEL_FRAME;

static void *buff_model_sbss_order[] = {
	&BUFF_MODEL_FRAME,
	BUFF_MODEL_MORPH_VALUE,
};

static void *buff_model_functions[] = {
	removeBuffModelObject,
	initializeBuffModelObject,
	initializeBuffModel,
	morphBuffModel,
	renderBuffModel,
	tickBuffModel,
};

void tickBuffModel(int32_t instanceId)
{
	morphBuffModel((int32_t)BUFF_MODEL[0], 5, BUFF_MODEL_MORPH_VALUE[BUFF_MODEL_FRAME & 1]);
	BUFF_MODEL_FRAME += 1;
}

void renderBuffModel(void)
{
}

void morphBuffModel(int32_t model, int32_t compIdx, int32_t color)
{
	char *p;
	int32_t i;
	int32_t count;
	struct TMD_STRUCT *obj;
	uint8_t attr;

	model += 12;
	obj = (struct TMD_STRUCT *)model;
	obj += compIdx;
	count = obj->primn;
	p = (char *)obj->primtop;
	for (i = 0; i < count; i++) {
		attr = *(int32_t *)p >> 24;
		if (attr & 4) {
			*(int16_t *)(p + 6) = color;
			if (attr & 8) {
				if (!(attr & 0x10)) {
					p += 0x20;
				} else {
					p += 0x2c;
				}
			} else if (!(attr & 0x10)) {
				p += 0x1c;
			} else {
				p += 0x24;
			}
		}
	}
}

void initializeBuffModel(TMDModel *model)
{
	BUFF_MODEL[0] = model;
	GsMapModelingData((unsigned long *)&BUFF_MODEL[0]->flags);
	BUFF_MODEL_MORPH_VALUE[0] = 0x7acc;
	BUFF_MODEL_MORPH_VALUE[1] = 0x7b0c;
}

int32_t initializeBuffModelObject(void)
{
	BUFF_MODEL_FRAME = 0;
	return addObject(0x501, 0, tickBuffModel,
			 (RenderFunction)renderBuffModel);
}

int32_t removeBuffModelObject(void)
{
	return removeObject(0x501, 0);
}
