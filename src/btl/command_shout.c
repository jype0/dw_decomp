#include <string.h>

#include <libgpu.h>
#include <libgs.h>

#include <dw/btl.h>
#include <dw/script.h>
#include <dw/world_object.h>

extern BtlCommandShout BTL_COMMAND_SHOUT;

int16_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void BTL_removeCommandShout(void);
void BTL_renderCommandShout(void);

char BTL_STR_SHOUT_RUN[] = "Run!";
char BTL_STR_SHOUT_ATTACK[] = "Attack!";
char BTL_STR_SHOUT_CHANGE[] = "Change!";

const char BTL_STR_SHOUT_YOUR_CALL[] = "Your Call!";
const char BTL_STR_SHOUT_MODERATE[] = "Moderate!";
const char BTL_STR_SHOUT_DISTANCE[] = "Distance!";
const char BTL_STR_SHOUT_DEFENSE[] = "Defense!";

const char *BTL_SHOUTS[7] = {
	BTL_STR_SHOUT_RUN,
	BTL_STR_SHOUT_ATTACK,
	BTL_STR_SHOUT_YOUR_CALL,
	BTL_STR_SHOUT_MODERATE,
	BTL_STR_SHOUT_DISTANCE,
	BTL_STR_SHOUT_DEFENSE,
	BTL_STR_SHOUT_CHANGE,
};

void BTL_drawCommandShout(uint32_t command)
{
	RECT rect;
	int16_t screenPos[2];
	int16_t tech;
	int32_t length;

	if (BTL_COMMAND_SHOUT.frame != -1) {
		BTL_removeCommandShout();
	}

	BTL_COMMAND_SHOUT.frame = 0;
	getEntityScreenPos(ENTITY_TABLE[0], 4, screenPos);
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

	BTL_COMMAND_SHOUT.width = length * 12;
	if ((screenPos[0] - (BTL_COMMAND_SHOUT.width / 2)) < -140) {
		screenPos[0] = (BTL_COMMAND_SHOUT.width / 2) - 140;
	}

	if ((screenPos[0] + (BTL_COMMAND_SHOUT.width / 2)) >= 141) {
		screenPos[0] = 140 - (BTL_COMMAND_SHOUT.width / 2);
	}

	if (screenPos[1] < -100) {
		screenPos[1] = -100;
	}

	if (screenPos[1] >= 101) {
		screenPos[1] = 100;
	}

	BTL_COMMAND_SHOUT.x = screenPos[0];
	BTL_COMMAND_SHOUT.y = screenPos[1];
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
	int32_t y;

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

	if (BTL_COMMAND_SHOUT.frame < 4L) {
		sprite.scaley = sprite.scalex = (((BTL_COMMAND_SHOUT.frame * 2) + 2) << 12) / 10;
	} else {
		sprite.scaley = sprite.scalex = 0x1000;
	}

	sprite.x = BTL_COMMAND_SHOUT.x;
	y = BTL_COMMAND_SHOUT.y;
	if (y < -40) {
		sprite.y = y + BTL_SHOUT_DROP_OFFSETS[BTL_COMMAND_SHOUT.frame++];
	} else {
		sprite.y = y + BTL_SHOUT_HOP_OFFSETS[BTL_COMMAND_SHOUT.frame++];
	}

	sprite.rotate = 0;
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 7);

	sprite.b = 0;
	sprite.g = 0;
	sprite.r = 0;
	sprite.x++;
	sprite.y++;
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 7);

	if (BTL_COMMAND_SHOUT.frame >= 20) {
		BTL_removeCommandShout();
	}
}
