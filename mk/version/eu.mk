EXE_NAME := SLES_029.14

PSYQ_INCLUDE := external/psyq_headers/mw_lib43/include

MWCC_OPT_LEVEL := 0

MAIN_SBSS := \
	$(GEN_DIR)/unk_0x8013BD18.sbss.s \
	$(GEN_DIR)/unk_0x8013BD90.sbss.s \
	$(GEN_DIR)/unk_0x8013BFFC.sbss.s \
	$(GEN_DIR)/unk_0x8013C020.sbss.s \
	$(GEN_DIR)/unk_0x8013C10C.sbss.s \
	$(GEN_DIR)/unk_0x8013C3E8.sbss.s \
	$(GEN_DIR)/unk_0x8013C48A.sbss.s

MAIN_GEN_SRC := $(MAIN_SBSS)

MAIN_C_SRC := \
	src/main/door_mapdata.c \
	src/main/fade.c \
	src/main/file.c \
	src/main/kar.c \
	src/main/line.c \
	src/main/math.c \
	src/main/rng.c \
	src/main/sound_async.c \
	src/main/toilet_data.c \
	src/main/tournament.c \
	src/main/vecmath.c

$(eval $(call unit,MAIN,main))

BTL_C_SRC := \
	src/btl/battle_setup.c \
	src/btl/command_menu.c

$(eval $(call overlay,BTL,btl))

$(eval $(call overlay,DGET,dget))

DOO2_C_SRC := \
	src/doo2/doo2.c

$(eval $(call overlay,DOO2,doo2))

$(eval $(call overlay,DOOA,dooa))

EAB_C_SRC := \
	src/eab/eab.c

$(eval $(call overlay,EAB,eab))

$(eval $(call overlay,ENDI,endi))
$(eval $(call overlay,EVL,evl))

FISH_C_SRC := \
	src/fish/fish_model.c

$(eval $(call overlay,FISH,fish))

$(eval $(call overlay,KAR,kar))
$(eval $(call overlay,MOV,mov))
$(eval $(call overlay,MURD,murd))
$(eval $(call overlay,SHOP,shop))
$(eval $(call overlay,STD,std))

TRN2_C_SRC := \
	src/trn2/trn2_hp_map99.c \
	src/trn2/trn2_mp.c

$(eval $(call overlay,TRN2,trn2))

$(eval $(call overlay,TRN,trn))
$(eval $(call overlay,VS,vs))

UNDEFINED_SYMS := $(foreach u,main $(shell echo $(OVERLAY) | tr A-Z a-z), \
	$(GEN_DIR)/undefined_funcs_auto_$(u).ld \
	$(GEN_DIR)/undefined_syms_auto_$(u).ld)
