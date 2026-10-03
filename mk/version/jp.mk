EXE_NAME := SLPS_017.97

MWCC_OPT_LEVEL := 0

MAIN_SBSS := \
	$(GEN_DIR)/unk_0x8013DF20.sbss.s \
	$(GEN_DIR)/unk_0x8013DF94.sbss.s \
	$(GEN_DIR)/unk_0x8013E070.sbss.s \
	$(GEN_DIR)/unk_0x8013E0EC.sbss.s \
	$(GEN_DIR)/unk_0x8013E1B0.sbss.s \
	$(GEN_DIR)/unk_0x8013E1FC.sbss.s \
	$(GEN_DIR)/unk_0x8013E26C.sbss.s \
	$(GEN_DIR)/unk_0x8013E34C.sbss.s \
	$(GEN_DIR)/unk_0x8013E37C.sbss.s \
	$(GEN_DIR)/unk_0x8013E438.sbss.s \
	$(GEN_DIR)/unk_0x8013E4D0.sbss.s \
	$(GEN_DIR)/unk_0x8013E59C.sbss.s

MAIN_BSS := \
	$(GEN_DIR)/unk_0x8013E6A8.bss.s \
	$(GEN_DIR)/unk_0x80140CF4.bss.s \
	$(GEN_DIR)/unk_0x801414B8.bss.s \
	$(GEN_DIR)/unk_0x80146048.bss.s \
	$(GEN_DIR)/unk_0x801566A4.bss.s \
	$(GEN_DIR)/unk_0x80167BCC.bss.s \
	$(GEN_DIR)/unk_0x80168460.bss.s \
	$(GEN_DIR)/unk_0x80168920.bss.s

MAIN_GEN_SRC := $(MAIN_BSS) $(MAIN_SBSS)

MAIN_C_SRC := \
	src/main/_psstart.c \
	src/main/aabb.c \
	src/main/anim.c \
	src/main/battle_tick.c \
	src/main/battle_ui.c \
	src/main/btl.c \
	src/main/bubble.c \
	src/main/buff_model.c \
	src/main/butterfly.c \
	src/main/clock.c \
	src/main/door_mapdata.c \
	src/main/drop_shadow.c \
	src/main/efe.c \
	src/main/efe_table.c \
	src/main/entity_text.c \
	src/main/evl.c \
	src/main/evolution.c \
	src/main/fade.c \
	src/main/file.c \
	src/main/file_queue.c \
	src/main/file_table.c \
	src/main/fish.c \
	src/main/font.c \
	src/main/game_menu.c \
	src/main/graphics.c \
	src/main/graphics2.c \
	src/main/inventory.c \
	src/main/item.c \
	src/main/kar.c \
	src/main/main.c \
	src/main/main_menu.c \
	src/main/map.c \
	src/main/map_collision.c \
	src/main/map_object.c \
	src/main/math.c \
	src/main/new_game.c \
	src/main/overworld.c \
	src/main/particle.c \
	src/main/partner.c \
	src/main/partner_impl.c \
	src/main/script_common.c \
	src/main/script_interp.c \
	src/main/script_textbox.c \
	src/main/script_value.c \
	src/main/sound.c \
	src/main/sound_async.c \
	src/main/tamer.c \
	src/main/toilet_data.c \
	src/main/tournament.c \
	src/main/ui.c \
	src/main/utils.c \
	src/main/utils2.c \
	src/main/world_object.c

$(eval $(call unit,MAIN,main))

BTL_C_SRC := \
	src/btl/battle_hud.c \
	src/btl/battle_main.c \
	src/btl/battle_setup.c \
	src/btl/btl_bss_jp.c \
	src/btl/command_menu.c \
	src/btl/command_shout.c

$(eval $(call overlay,BTL,btl))
DGET_C_SRC := \
	src/dget/dget.c

$(eval $(call overlay,DGET,dget))
DOO2_C_SRC := \
	src/doo2/doo2.c \
	src/doo2/doo2_bss_jp.c

$(eval $(call overlay,DOO2,doo2))
DOOA_C_SRC := \
	src/dooa/dooa.c \
	src/dooa/dooa_bss_jp.c

$(eval $(call overlay,DOOA,dooa))
EAB_C_SRC := \
	src/eab/eab.c \
	src/eab/eab_bss_jp.c

$(eval $(call overlay,EAB,eab))
ENDI_C_SRC := \
	src/endi/endi.c \
	src/endi/endi_bss_jp.c

$(eval $(call overlay,ENDI,endi))
EVL_C_SRC := \
	src/evl/evl.c \
	src/evl/evl_bss_jp.c

$(eval $(call overlay,EVL,evl))
FISH_C_SRC := \
	src/fish/fish.c \
	src/fish/fish_bss_jp.c \
	src/fish/fish_model.c

$(eval $(call overlay,FISH,fish))
KAR_C_SRC := \
	src/kar/kar.c \
	src/kar/kar_bss_jp.c

$(eval $(call overlay,KAR,kar))
MOV_C_SRC := \
	src/mov/mov.c

$(eval $(call overlay,MOV,mov))
MURD_C_SRC := \
	src/murd/murd.c \
	src/murd/murd_bss_jp.c

$(eval $(call overlay,MURD,murd))
SHOP_C_SRC := \
	src/shop/script_menu.c

$(eval $(call overlay,SHOP,shop))
STD_C_SRC := \
	src/std/std_hud.c \
	src/std/std_setup.c \
	src/std/std_bss_jp.c

$(eval $(call overlay,STD,std))
TRN2_C_SRC := \
	src/trn2/trn2_def_map108.c \
	src/trn2/trn2_def_map99.c \
	src/trn2/trn2_hp_map107.c \
	src/trn2/trn2_hp_map99.c \
	src/trn2/trn2_hud.c \
	src/trn2/trn2_mp.c \
	src/trn2/trn2_off.c \
	src/trn2/trn2_reward.c \
	src/trn2/trn2_slots.c \
	src/trn2/trn2_bss_jp.c

$(eval $(call overlay,TRN2,trn2))
TRN_C_SRC := \
	src/trn/trn_brain.c \
	src/trn/trn_def.c \
	src/trn/trn_hp.c \
	src/trn/trn_hud.c \
	src/trn/trn_mp.c \
	src/trn/trn_off.c \
	src/trn/trn_reward.c \
	src/trn/trn_slots.c \
	src/trn/trn_speed.c \
	src/trn/trn_bss_jp.c

$(eval $(call overlay,TRN,trn))
VS_C_SRC := \
	src/vs/vs_hud.c \
	src/vs/vs_main.c

$(eval $(call overlay,VS,vs))

UNDEFINED_SYMS := $(foreach u,main $(shell echo $(OVERLAY) | tr A-Z a-z), \
	$(GEN_DIR)/undefined_funcs_auto_$(u).ld \
	$(GEN_DIR)/undefined_syms_auto_$(u).ld)
