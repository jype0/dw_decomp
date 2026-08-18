.EXTRA_PREREQS := $(abspath $(lastword $(MAKEFILE_LIST)))

-include local.mk

TOOLCHAIN ?= mipsel-linux-gnu-

BUILDDIR := build
ASM_DIR := asm
EXPECTEDDIR := expected

ELF := $(BUILDDIR)/SLUS_010.32.elf
EXE := $(BUILDDIR)/SLUS_010.32

CC := $(TOOLCHAIN)gcc
LD := $(TOOLCHAIN)ld
OBJCOPY := $(TOOLCHAIN)objcopy

PYTHON := python3
SPLAT := $(PYTHON) -m splat split

MWCCWRAP ?= bin/mwccwrap/mwccwrap.exe
MWCCWRAP_FLAGS ?= -dll "bin/cc_mips/cc_mips_40.dll"
MWCCWRAP_FLAGS += -O4 -sdata 8 -Werror -requireprotos -gccincludes \
		  -lang c -Cpp_exceptions off -RTTI off

export MWCIncludes =

WIBO ?= bin/wibo-x86_64

METROWRAP ?= bin/metrowrap/mw
METROWRAP_FLAGS ?= --use-wibo --wibo-path $(WIBO)
METROWRAP_FLAGS += --mwcc-path $(MWCCWRAP) --split-sections \
		 --elf-flags 0x00001001 \
		 --as-march r3000 \
		 --macro-inc-path include/macro.inc

OBJDIFF ?= bin/objdiff-cli-linux-x86_64

INC := -Iexternal/psyq_headers/mw_lib41/include -Iinclude

LDSCRIPT := \
	config/overlay.ld \
	config/main.ld

CPPLDSCRIPT := $(LDSCRIPT:%=$(BUILDDIR)/%)

ARCHFLAGS := -march=r3000 -mtune=r3000 -mabi=32 -EL -mfp32 \
	     -fno-pic -mno-shared -mno-abicalls -mno-llsc \
	     -fno-stack-protector -nostdlib -ffreestanding \
	     -Xassembler -no-pad-sections
ASFLAGS := -Wa,--sectname-subst
CFLAGS := -g -Wall -Wextra -Werror -std=c99 -Os -G0 -mno-gpopt $(ARCHFLAGS)
CPPFLAGS := $(INC)
DEPFLAGS = -MM -MF $(@:.o=.d) -MT $@
LDFLAGS := -g $(addprefix -T ,$(CPPLDSCRIPT)) -static \
	   -Wl,--no-check-sections -Wl,-Map=% -Wl,--build-id=none \
	   -Wl,--gc-sections -Wl,--print-gc-sections

MAIN_ASM_SRC := $(shell find $(ASM_DIR)/main -path '*.s' \
		-not -path '$(ASM_DIR)/main/*matchings*' 2> /dev/null)

MAIN_SBSS := \
	$(BUILDDIR)/generated/unk_0x80134C4C.sbss.s \
	$(BUILDDIR)/generated/unk_0x80134CC0.sbss.s \
	$(BUILDDIR)/generated/unk_0x80134D9C.sbss.s \
	$(BUILDDIR)/generated/unk_0x80134E50.sbss.s \
	$(BUILDDIR)/generated/unk_0x80134E68.sbss.s \
	$(BUILDDIR)/generated/unk_0x80134E90.sbss.s

MAIN_BSS := \
	$(BUILDDIR)/generated/unk_0x801353F8.bss.s \
	$(BUILDDIR)/generated/libgs.bss.s \
	$(BUILDDIR)/generated/libgte.bss.s \
	$(BUILDDIR)/generated/model.bss.s \
	$(BUILDDIR)/generated/unk_0x80137A24.bss.s \
	$(BUILDDIR)/generated/unk_0x8014DBEC.bss.s \
	$(BUILDDIR)/generated/unk_0x80154F80.bss.s \
	$(BUILDDIR)/generated/butterfly.bss.s \
	$(BUILDDIR)/generated/libmrcd.bss.s \
	$(BUILDDIR)/generated/bubble.bss.s \
	$(BUILDDIR)/generated/battle_ui.bss.s \
	$(BUILDDIR)/generated/unk_0x801555D0.bss.s

MAIN_SRC := \
	$(MAIN_ASM_SRC) \
	$(MAIN_BSS) \
	$(MAIN_SBSS) \
	src/main/_psstart.c \
	src/main/aabb.c \
	src/main/anim.c \
	src/main/bubble.c \
	src/main/butterfly.c \
	src/main/clock.c \
	src/main/efe_table.c \
	src/main/evl.c \
	src/main/evolution.c \
	src/main/fade.c \
	src/main/file.c \
	src/main/file_queue.c \
	src/main/file_table.c \
	src/main/fish.c \
	src/main/graphics.c \
	src/main/graphics2.c \
	src/main/item.c \
	src/main/kar.c \
	src/main/main.c \
	src/main/main_menu.c \
	src/main/inventory.c \
	src/main/entity_text.c \
	src/main/efe.c \
	src/main/btl.c \
	src/main/battle_ui.c \
	src/main/map.c \
	src/main/map_collision.c \
	src/main/map_object.c \
	src/main/overworld.c \
	src/main/overworld_evochart_detail.c \
	src/main/overworld_menu_views.c \
	src/main/overworld_evochart_view.c \
	src/main/overworld_status_boxes.c \
	src/main/overworld_moves_text.c \
	src/main/overworld_moves_box.c \
	src/main/overworld_playerinfo_text.c \
	src/main/overworld_evochart_text.c \
	src/main/overworld_medal_text.c \
	src/main/overworld_card_text.c \
	src/main/math.c \
	src/main/model.c \
	src/main/particle.c \
	src/main/partner.c \
	src/main/partner_impl.c \
	src/main/script_common.c \
	src/main/script_instr10.c \
	src/main/script_instr28.c \
	src/main/script_instr64.c \
	src/main/script_music.c \
	src/main/script_anim.c \
	src/main/script_engine.c \
	src/main/script_tick.c \
	src/main/script_ops.c \
	src/main/script_draw.c \
	src/main/sound.c \
	src/main/sound_async.c \
	src/main/tamer.c \
	src/main/tournament.c \
	src/main/ui.c \
	src/main/utils.c \
	src/main/vs.c \
	src/main/world_object.c

MAIN_OBJ := $(MAIN_SRC:%=$(BUILDDIR)/%.o)
MAIN_DEP := $(MAIN_OBJ:%.o=%.d)

OBJ += $(MAIN_OBJ)
DEP += $(MAIN_DEP)

OVERLAY :=

BTL_ASM_SRC := $(shell find $(ASM_DIR)/btl -path '*.s' \
		-not -path '$(ASM_DIR)/btl/*matchings*')

BTL_SRC := \
	$(BTL_ASM_SRC) \
	src/btl/battle_effect.c \
	src/btl/battle_hud.c \
	src/btl/battle_main.c \
	src/btl/battle_setup.c \
	src/btl/command_menu.c \
	src/btl/command_shout.c

BTL_OBJ := $(BTL_SRC:%=$(BUILDDIR)/%.o)
BTL_DEP := $(BTL_OBJ:%.o=%.d)

OBJ += $(BTL_OBJ)
DEP += $(BTL_DEP)

OVERLAY += BTL

DGET_ASM_SRC := $(shell find $(ASM_DIR)/dget -path '*.s' \
		-not -path '$(ASM_DIR)/dget/*matchings*' 2> /dev/null)

DGET_SRC := \
	$(DGET_ASM_SRC) \
	src/dget/dget.c

DGET_OBJ := $(DGET_SRC:%=$(BUILDDIR)/%.o)
DGET_DEP := $(DGET_OBJ:%.o=%.d)

OBJ += $(DGET_OBJ)
DEP += $(DGET_DEP)

OVERLAY += DGET

DOO2_ASM_SRC := $(shell find $(ASM_DIR)/doo2 -path '*.s' \
		-not -path '$(ASM_DIR)/doo2/*matchings*' 2> /dev/null)

DOO2_SRC := \
	$(DOO2_ASM_SRC) \
	src/doo2/doo2.c

DOO2_OBJ := $(DOO2_SRC:%=$(BUILDDIR)/%.o)
DOO2_DEP := $(DOO2_OBJ:%.o=%.d)

OBJ += $(DOO2_OBJ)
DEP += $(DOO2_DEP)

OVERLAY += DOO2

DOOA_ASM_SRC := $(shell find $(ASM_DIR)/dooa -path '*.s' \
		-not -path '$(ASM_DIR)/dooa/*matchings*' 2> /dev/null)

DOOA_SRC := \
	$(DOOA_ASM_SRC) \
	src/dooa/dooa.c

DOOA_OBJ := $(DOOA_SRC:%=$(BUILDDIR)/%.o)
DOOA_DEP := $(DOOA_OBJ:%.o=%.d)

OBJ += $(DOOA_OBJ)
DEP += $(DOOA_DEP)

OVERLAY += DOOA

EAB_ASM_SRC := $(shell find $(ASM_DIR)/eab -path '*.s' \
		-not -path '$(ASM_DIR)/eab/*matchings*' 2> /dev/null)

EAB_SRC := \
	$(EAB_ASM_SRC) \
	src/eab/eab.c

EAB_OBJ := $(EAB_SRC:%=$(BUILDDIR)/%.o)
EAB_DEP := $(EAB_OBJ:%.o=%.d)

OBJ += $(EAB_OBJ)
DEP += $(EAB_DEP)

OVERLAY += EAB

ENDI_ASM_SRC := $(shell find $(ASM_DIR)/endi -path '*.s' \
		-not -path '$(ASM_DIR)/endi/*matchings*' 2> /dev/null)

ENDI_SRC := \
	$(ENDI_ASM_SRC) \
	src/endi/endi.c

ENDI_OBJ := $(ENDI_SRC:%=$(BUILDDIR)/%.o)
ENDI_DEP := $(ENDI_OBJ:%.o=%.d)

OBJ += $(ENDI_OBJ)
DEP += $(ENDI_DEP)

OVERLAY += ENDI

EVL_SRC := \
	$(wildcard $(ASM_DIR)/evl/*.s) \
	$(wildcard $(ASM_DIR)/evl/data/*.s)

EVL_OBJ := $(EVL_SRC:%=$(BUILDDIR)/%.o)
EVL_DEP := $(EVL_OBJ:%.o=%.d)

OBJ += $(EVL_OBJ)
DEP += $(EVL_DEP)

OVERLAY += EVL

FISH_SRC := \
	$(wildcard $(ASM_DIR)/fish/*.s) \
	$(wildcard $(ASM_DIR)/fish/data/*.s)

FISH_OBJ := $(FISH_SRC:%=$(BUILDDIR)/%.o)
FISH_DEP := $(FISH_OBJ:%.o=%.d)

OBJ += $(FISH_OBJ)
DEP += $(FISH_DEP)

OVERLAY += FISH

KAR_SRC := \
	$(wildcard $(ASM_DIR)/kar/*.s) \
	$(wildcard $(ASM_DIR)/kar/data/*.s)

KAR_OBJ := $(KAR_SRC:%=$(BUILDDIR)/%.o)
KAR_DEP := $(KAR_OBJ:%.o=%.d)

OBJ += $(KAR_OBJ)
DEP += $(KAR_DEP)

OVERLAY += KAR

MOV_SRC := \
	$(wildcard $(ASM_DIR)/mov/*.s) \
	$(wildcard $(ASM_DIR)/mov/data/*.s)

MOV_OBJ := $(MOV_SRC:%=$(BUILDDIR)/%.o)
MOV_DEP := $(MOV_OBJ:%.o=%.d)

OBJ += $(MOV_OBJ)
DEP += $(MOV_DEP)

OVERLAY += MOV

MURD_ASM_SRC := $(shell find $(ASM_DIR)/murd -path '*.s' \
		-not -path '$(ASM_DIR)/murd/*matchings*' 2> /dev/null)

MURD_SRC := \
	$(MURD_ASM_SRC) \
	src/murd/murd.c

MURD_OBJ := $(MURD_SRC:%=$(BUILDDIR)/%.o)
MURD_DEP := $(MURD_OBJ:%.o=%.d)

OBJ += $(MURD_OBJ)
DEP += $(MURD_DEP)

OVERLAY += MURD

STD_ASM_SRC := $(shell find $(ASM_DIR)/std -path '*.s' \
		-not -path '$(ASM_DIR)/std/*matchings*')

STD_SRC := \
	$(STD_ASM_SRC) \
	src/std/std_setup.c \
	src/std/std_main.c \
	src/std/std_hud.c \
	src/std/std_effect.c

STD_OBJ := $(STD_SRC:%=$(BUILDDIR)/%.o)
STD_DEP := $(STD_OBJ:%.o=%.d)

OBJ += $(STD_OBJ)
DEP += $(STD_DEP)

OVERLAY += STD

TRN2_ASM_SRC := $(shell find $(ASM_DIR)/trn2 -path '*.s' \
		-not -path '$(ASM_DIR)/trn2/*matchings*')

TRN2_SRC := \
	$(TRN2_ASM_SRC) \
	src/trn2/trn2_hp_map99.c \
	src/trn2/trn2_def_map99.c \
	src/trn2/trn2_hp_map107.c \
	src/trn2/trn2_mp.c \
	src/trn2/trn2_off.c \
	src/trn2/trn2_def_map108.c \
	src/trn2/trn2_reward.c \
	src/trn2/trn2_hud.c \
	src/trn2/trn2_slots.c

TRN2_OBJ := $(TRN2_SRC:%=$(BUILDDIR)/%.o)
TRN2_DEP := $(TRN2_OBJ:%.o=%.d)

OBJ += $(TRN2_OBJ)
DEP += $(TRN2_DEP)

OVERLAY += TRN2

TRN_ASM_SRC := $(shell find $(ASM_DIR)/trn -path '*.s' \
		-not -path '$(ASM_DIR)/trn/*matchings*')

TRN_SRC := \
	$(TRN_ASM_SRC) \
	src/trn/trn_reward.c \
	src/trn/trn_hp.c \
	src/trn/trn_mp.c \
	src/trn/trn_off.c \
	src/trn/trn_def.c \
	src/trn/trn_speed.c \
	src/trn/trn_brain.c \
	src/trn/trn_hud.c \
	src/trn/trn_slots.c

TRN_OBJ := $(TRN_SRC:%=$(BUILDDIR)/%.o)
TRN_DEP := $(TRN_OBJ:%.o=%.d)

OBJ += $(TRN_OBJ)
DEP += $(TRN_DEP)

OVERLAY += TRN

VS_ASM_SRC := $(shell find $(ASM_DIR)/vs -path '*.s' \
		-not -path '$(ASM_DIR)/vs/*matchings*')

VS_SRC := \
	$(VS_ASM_SRC) \
	src/vs/vs_effect.c \
	src/vs/vs_hud.c \
	src/vs/vs_main.c \
	src/vs/vs_scene.c

VS_OBJ := $(VS_SRC:%=$(BUILDDIR)/%.o)
VS_DEP := $(VS_OBJ:%.o=%.d)

OBJ += $(VS_OBJ)
DEP += $(VS_DEP)

OVERLAY += VS

LINKER_SCRIPTS := $(addprefix $(BUILDDIR)/generated/,\
		  $(addsuffix .ld, main \
		  $(shell echo $(OVERLAY) | tr A-Z a-z)))

all: $(EXE)

generate: $(LINKER_SCRIPTS)

regenerate: reset
	$(MAKE) generate

compare:
	@tools/cmp_bins.sh

expected: $(OBJ)
	rm -rf $(EXPECTEDDIR)
	@mkdir -p $(EXPECTEDDIR)
	cp -r $(BUILDDIR)/$(ASM_DIR) $(EXPECTEDDIR)/$(ASM_DIR)
	cp -r $(BUILDDIR)/src $(EXPECTEDDIR)/src

objdiff: expected
	$(PYTHON) tools/objdiff/objdiff_generate.py tools/objdiff/config.yaml

report: objdiff
	$(OBJDIFF) report generate \
		--config combineTextSections=false > $(BUILDDIR)/report.json

$(BUILDDIR)/%.ld: %.ld
	@mkdir -p $(dir $@)
	$(CPP) -P -x c $(INC) -o $@ $<

$(BUILDDIR)/%_REL.BIN: $(ELF) $(CPPLDSCRIPT)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -j $(@:$(BUILDDIR)/%_REL.BIN=.%) -O binary $< $@

$(ELF): $(OBJ) $(CPPLDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@

$(EXE): $(ELF) $(OVERLAY:%=$(BUILDDIR)/%_REL.BIN)
	@mkdir -p $(dir $@)
	$(OBJCOPY) $(addprefix -R .,$(OVERLAY)) -O binary $< $@

-include $(DEP)

$(BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) $<
	$(METROWRAP) -o $@ $(METROWRAP_FLAGS) $(MWCCWRAP_FLAGS) $(CPPFLAGS) $<

$(BUILDDIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) $<
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(ASFLAGS) -o $@ $<
	@$(OBJCOPY) --set-section-alignment .text=4 \
				--set-section-alignment .data=4 \
				--set-section-alignment .rodata=4 \
				--set-section-alignment .bss=4 \
				--set-section-alignment .sbss=4 \
				--set-section-alignment .sdata=4 $@

$(MAIN_SBSS) &: config/sbss.yaml config/symbols.txt
	@mkdir -p $(dir $@)
	tools/gen_bss.py $^ $(BUILDDIR)/generated/

$(MAIN_BSS) &: config/bss.yaml config/symbols.txt
	@mkdir -p $(dir $@)
	tools/gen_bss.py $^ $(BUILDDIR)/generated/

$(BUILDDIR)/generated/%.ld: config/%.yaml
	@mkdir -p $(dir $@)
	$(SPLAT) $< --disassemble-all --make-full-disasm-for-code

clean:
	rm -rf $(BUILDDIR)

reset: clean
	rm -rf $(ASM_DIR)

.PHONY: all clean
