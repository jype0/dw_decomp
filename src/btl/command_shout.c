#include <string.h>

#include <libgpu.h>
#include <libgs.h>

#include <dw/btl.h>
#include <dw/script.h>
#include <dw/world_object.h>

extern char *BTL_D_80072DE8[];
extern volatile int32_t BTL_D_80072DAC[];
extern int16_t BTL_D_80072DB0[];
extern int16_t BTL_D_80072DB2[];
extern uint8_t BTL_D_80072DB4[];
extern int8_t BTL_D_80072E54[];
extern int8_t BTL_D_80072E68[];

int16_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void BTL_drawCommandShout(uint32_t arg0);
void BTL_removeCommandShout(void);
void BTL_renderCommandShout(void);

void BTL_drawCommandShout(uint32_t arg0)
{
	RECT rect;
	int16_t screenPos[2];
	int16_t tech;
	int32_t length;

	if (BTL_D_80072DAC[0] != -1) {
		BTL_removeCommandShout();
	}

	BTL_D_80072DAC[0] = 0;
	getEntityScreenPos(ENTITY_TABLE[0], 4, screenPos);
	rect.x = 0;
	rect.y = 0xcc;
	rect.w = 0xa8;
	rect.h = 0xc;
	clearTextSubArea(&rect);

	if ((arg0 >= 8) && (arg0 < 0xc)) {
		tech = entityGetTechFromAnim(ENTITY_TABLE[1], PARTNER_ENTITY.digimonEntity.stats.base.moves[(int32_t)arg0 - 8]);
		drawString(MOVE_NAMES[tech], 0, 0xcc);
		length = strlen(MOVE_NAMES[tech]);
	} else {
		drawString(BTL_D_80072DE8[(int32_t)arg0 - 1], 0, 0xcc);
		length = strlen(BTL_D_80072DE8[(int32_t)arg0 - 1]);
	}

	BTL_D_80072DB4[0] = length * 12;
	if ((screenPos[0] - (BTL_D_80072DB4[0] / 2)) < -0x8c) {
		screenPos[0] = (BTL_D_80072DB4[0] / 2) - 0x8c;
	}

	if ((screenPos[0] + (BTL_D_80072DB4[0] / 2)) >= 0x8d) {
		screenPos[0] = 0x8c - (BTL_D_80072DB4[0] / 2);
	}

	if (screenPos[1] < -0x64) {
		screenPos[1] = -0x64;
	}

	if (screenPos[1] >= 0x65) {
		screenPos[1] = 0x64;
	}

	BTL_D_80072DB0[0] = screenPos[0];
	BTL_D_80072DB2[0] = screenPos[1];
	addObject(0x199, 0, NULL, (RenderFunction)BTL_renderCommandShout);
}

void BTL_removeCommandShout(void)
{
	if (BTL_D_80072DAC[0] != -1) {
		removeObject(0x199, 0);
		BTL_D_80072DAC[0] = -1;
	}
}

void BTL_renderCommandShout(void)
{
	GsSPRITE sprite;
	int32_t y;

	sprite.attribute = 0;
	sprite.tpage = 0x1b;
	sprite.cx = 0xd0;
	sprite.cy = 0x1e8;
	sprite.r = sprite.g = sprite.b = 0x80;
	sprite.w = BTL_D_80072DB4[0];
	sprite.h = 0xc;
	sprite.mx = sprite.w / 2;
	sprite.my = 6;
	sprite.u = 0;
	sprite.v = 0xcc;

	if (BTL_D_80072DAC[0] < 4) {
		sprite.scaley = sprite.scalex = (((BTL_D_80072DAC[0] * 2) + 2) << 12) / 10;
	} else {
		sprite.scaley = sprite.scalex = 0x1000;
	}

	sprite.x = BTL_D_80072DB0[0];
	y = BTL_D_80072DB2[0];
	if (y < -0x28) {
		sprite.y = y + BTL_D_80072E68[BTL_D_80072DAC[0]++];
	} else {
		sprite.y = y + BTL_D_80072E54[BTL_D_80072DAC[0]++];
	}

	sprite.rotate = 0;
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 7);

	sprite.b = 0;
	sprite.g = 0;
	sprite.r = 0;
	sprite.x++;
	sprite.y++;
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 7);

	if (BTL_D_80072DAC[0] >= 0x14) {
		BTL_removeCommandShout();
	}
}
