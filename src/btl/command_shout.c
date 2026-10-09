#include <string.h>

#include <libgpu.h>
#include <libgs.h>

#include <dw/btl.h>
#include <dw/script.h>
#include <dw/version.h>
#include <dw/world_object.h>
#include <text/btl/command_shout.h>

int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void BTL_removeCommandShout(void);
void BTL_renderCommandShout(void);

BtlCommandShout BTL_COMMAND_SHOUT = { -1, 0, 0, 0 };

SHOUTS_TEXT

char *BTL_SHOUTS[7] = {
	BTL_STR_SHOUT_RUN,
	BTL_STR_SHOUT_ATTACK,
	BTL_STR_SHOUT_YOUR_CALL,
	BTL_STR_SHOUT_MODERATE,
	BTL_STR_SHOUT_DISTANCE,
	BTL_STR_SHOUT_DEFENSE,
	BTL_STR_SHOUT_CHANGE,
};

COMMANDS_TEXT

char *BTL_COMMAND_NAMES[8] = {
	BTL_STR_COMMAND_RUN,
	BTL_STR_COMMAND_ATTACK,
	BTL_STR_COMMAND_YOUR_CALL,
	BTL_STR_COMMAND_MODERATE,
	BTL_STR_COMMAND_DISTANCE,
	BTL_STR_COMMAND_DEFENSIVE,
	BTL_STR_COMMAND_CHANGE,
	NULL,
};

BTL_STR_SHOUT_EXCLAMATION_TEXT

void BTL_drawCommandShout(uint32_t command)
{
	RECT rect;
	DVECTOR screenPos;
	int16_t tech;
	int32_t length;

	if (BTL_COMMAND_SHOUT.frame != -1) {
		BTL_removeCommandShout();
	}

	BTL_COMMAND_SHOUT.frame = 0;
	getEntityScreenPos(ENTITY_TABLE[0], 4, &screenPos);
	setRECT(&rect, 0, 204, 168, 12);
	clearTextSubArea(&rect);

	if ((command >= 8) && (command < 12)) {
		tech = entityGetTechFromAnim(ENTITY_TABLE[1], PARTNER_ENTITY.digimonEntity.stats.base.moves[(int32_t)command - 8]);
		drawString(MOVE_NAMES[tech], 0, 204);
		length = strlen(MOVE_NAMES[tech]);
	} else {
		drawString(BTL_SHOUTS[(int32_t)command - 1], 0, 204);
		length = strlen(BTL_SHOUTS[(int32_t)command - 1]);
	}

#if !VERSION_IS(US)
	drawString(BTL_STR_SHOUT_EXCLAMATION, (length / 2) * 12, 204);
	length += 2;
	BTL_COMMAND_SHOUT.width = (length / 2) * 12;
#else
	BTL_COMMAND_SHOUT.width = length * 12;
#endif
	if ((screenPos.vx - (BTL_COMMAND_SHOUT.width / 2)) < -140) {
		screenPos.vx = (BTL_COMMAND_SHOUT.width / 2) - 140;
	}

	if ((screenPos.vx + (BTL_COMMAND_SHOUT.width / 2)) >= 141) {
		screenPos.vx = 140 - (BTL_COMMAND_SHOUT.width / 2);
	}

	if (screenPos.vy < -100) {
		screenPos.vy = -100;
	}

	if (screenPos.vy >= 101) {
		screenPos.vy = 100;
	}

	BTL_COMMAND_SHOUT.x = screenPos.vx;
	BTL_COMMAND_SHOUT.y = screenPos.vy;
	addObject(0x199, 0, NULL, (RenderFunction)BTL_renderCommandShout);
}

void BTL_removeCommandShout(void)
{
	if (BTL_COMMAND_SHOUT.frame != -1) {
		removeObject(0x199, 0);
		BTL_COMMAND_SHOUT.frame = -1;
	}
}

void BTL_renderCommandShout(void)
{
	GsSPRITE sprite;

	sprite.attribute = 0;
	sprite.tpage = getTPage(0, 0, 704, 256);
	sprite.cx = 208;
	sprite.cy = 488;
	sprite.r = sprite.g = sprite.b = 0x80;
	setWH(&sprite, BTL_COMMAND_SHOUT.width, 12);
	sprite.mx = sprite.w / 2;
	sprite.my = 6;
	sprite.u = 0;
	sprite.v = 204;

	if (BTL_COMMAND_SHOUT.frame < 4) {
		sprite.scaley = sprite.scalex = (((BTL_COMMAND_SHOUT.frame * 2) + 2) << 12) / 10;
	} else {
		sprite.scalex = 0x1000;
		sprite.scaley = 0x1000;
	}

	sprite.x = BTL_COMMAND_SHOUT.x;
	if (BTL_COMMAND_SHOUT.y < -40) {
		sprite.y = (int32_t)(int16_t)BTL_COMMAND_SHOUT.y + BTL_SHOUT_DROP_OFFSETS[BTL_COMMAND_SHOUT.frame++];
	} else {
		sprite.y = (int32_t)(int16_t)BTL_COMMAND_SHOUT.y + BTL_SHOUT_HOP_OFFSETS[BTL_COMMAND_SHOUT.frame++];
	}

	sprite.rotate = 0;
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 7);

	sprite.r = sprite.g = sprite.b = 0;
	sprite.x++;
	sprite.y++;
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 7);

	if (BTL_COMMAND_SHOUT.frame >= 20) {
		BTL_removeCommandShout();
	}
}
