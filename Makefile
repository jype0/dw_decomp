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

INC := -Iexternal/psyq_headers/include -Iinclude

LDSCRIPT := \
	config/overlay.ld \
	config/main.ld

CPPLDSCRIPT := $(LDSCRIPT:%=$(BUILDDIR)/%)

ARCHFLAGS := -march=r3000 -mtune=r3000 -mabi=32 -EL -mfp32 \
	     -fno-pic -mno-shared -mno-abicalls -mno-llsc \
	     -fno-stack-protector -nostdlib -ffreestanding \
	     -Xassembler -no-pad-sections
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
	$(BUILDDIR)/generated/unk_0x80134CC0.sbss.s

MAIN_BSS := \
	$(BUILDDIR)/generated/unk_0x801353F8.bss.s \
	$(BUILDDIR)/generated/libgs.bss.s \
	$(BUILDDIR)/generated/libgte.bss.s \
	$(BUILDDIR)/generated/model.bss.s \
	$(BUILDDIR)/generated/unk_0x80137A24.bss.s \
	$(BUILDDIR)/generated/butterfly.bss.s \
	$(BUILDDIR)/generated/libmrcd.bss.s \
	$(BUILDDIR)/generated/bubble.bss.s \
	$(BUILDDIR)/generated/battle_ui.bss.s \
	$(BUILDDIR)/generated/unk_0x801555D0.bss.s

MAIN_SRC := \
	$(MAIN_ASM_SRC) \
	$(MAIN_BSS) \
	$(MAIN_SBSS) \
	src/main/aabb.c \
	src/main/bubble.c \
	src/main/butterfly.c \
	src/main/clock.c \
	src/main/efe_table.c \
	src/main/evolution.c \
	src/main/fade.c \
	src/main/file.c \
	src/main/evl.c \
	src/main/kar.c \
	src/main/math.c \
	src/main/file_table.c \
	src/main/particle.c \
	src/main/partner.c \
	src/main/world_object.c \
	src/main/swap.c

MAIN_OBJ := $(MAIN_SRC:%=$(BUILDDIR)/%.o)
MAIN_DEP := $(MAIN_OBJ:%.o=%.d)

OBJ += $(MAIN_OBJ)
DEP += $(MAIN_DEP)

OVERLAY :=

BTL_SRC := \
	$(wildcard $(ASM_DIR)/btl/*.s) \
	$(wildcard $(ASM_DIR)/btl/data/*.s)

BTL_OBJ := $(BTL_SRC:%=$(BUILDDIR)/%.o)
BTL_DEP := $(BTL_OBJ:%.o=%.d)

OBJ += $(BTL_OBJ)
DEP += $(BTL_DEP)

OVERLAY += BTL

DGET_SRC := \
	$(wildcard $(ASM_DIR)/dget/*.s) \
	$(wildcard $(ASM_DIR)/dget/data/*.s)

DGET_OBJ := $(DGET_SRC:%=$(BUILDDIR)/%.o)
DGET_DEP := $(DGET_OBJ:%.o=%.d)

OBJ += $(DGET_OBJ)
DEP += $(DGET_DEP)

OVERLAY += DGET

DOO2_SRC := \
	$(wildcard $(ASM_DIR)/doo2/*.s) \
	$(wildcard $(ASM_DIR)/doo2/data/*.s)

DOO2_OBJ := $(DOO2_SRC:%=$(BUILDDIR)/%.o)
DOO2_DEP := $(DOO2_OBJ:%.o=%.d)

OBJ += $(DOO2_OBJ)
DEP += $(DOO2_DEP)

OVERLAY += DOO2

DOOA_SRC := \
	$(wildcard $(ASM_DIR)/dooa/*.s) \
	$(wildcard $(ASM_DIR)/dooa/data/*.s)

DOOA_OBJ := $(DOOA_SRC:%=$(BUILDDIR)/%.o)
DOOA_DEP := $(DOOA_OBJ:%.o=%.d)

OBJ += $(DOOA_OBJ)
DEP += $(DOOA_DEP)

OVERLAY += DOOA

EAB_SRC := \
	$(wildcard $(ASM_DIR)/eab/*.s) \
	$(wildcard $(ASM_DIR)/eab/data/*.s)

EAB_OBJ := $(EAB_SRC:%=$(BUILDDIR)/%.o)
EAB_DEP := $(EAB_OBJ:%.o=%.d)

OBJ += $(EAB_OBJ)
DEP += $(EAB_DEP)

OVERLAY += EAB

ENDI_SRC := \
	$(wildcard $(ASM_DIR)/endi/*.s) \
	$(wildcard $(ASM_DIR)/endi/data/*.s)

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

MURD_SRC := \
	$(wildcard $(ASM_DIR)/murd/*.s) \
	$(wildcard $(ASM_DIR)/murd/data/*.s)

MURD_OBJ := $(MURD_SRC:%=$(BUILDDIR)/%.o)
MURD_DEP := $(MURD_OBJ:%.o=%.d)

OBJ += $(MURD_OBJ)
DEP += $(MURD_DEP)

OVERLAY += MURD

SHOP_SRC := \
	$(wildcard $(ASM_DIR)/shop/*.s) \
	$(wildcard $(ASM_DIR)/shop/data/*.s)

SHOP_OBJ := $(SHOP_SRC:%=$(BUILDDIR)/%.o)
SHOP_DEP := $(SHOP_OBJ:%.o=%.d)

OBJ += $(SHOP_OBJ)
DEP += $(SHOP_DEP)

OVERLAY += SHOP

STD_SRC := \
	$(wildcard $(ASM_DIR)/std/*.s) \
	$(wildcard $(ASM_DIR)/std/data/*.s)

STD_OBJ := $(STD_SRC:%=$(BUILDDIR)/%.o)
STD_DEP := $(STD_OBJ:%.o=%.d)

OBJ += $(STD_OBJ)
DEP += $(STD_DEP)

OVERLAY += STD

TRN2_SRC := \
	$(wildcard $(ASM_DIR)/trn2/*.s) \
	$(wildcard $(ASM_DIR)/trn2/data/*.s)

TRN2_OBJ := $(TRN2_SRC:%=$(BUILDDIR)/%.o)
TRN2_DEP := $(TRN2_OBJ:%.o=%.d)

OBJ += $(TRN2_OBJ)
DEP += $(TRN2_DEP)

OVERLAY += TRN2

TRN_SRC := \
	$(wildcard $(ASM_DIR)/trn/*.s) \
	$(wildcard $(ASM_DIR)/trn/data/*.s)

TRN_OBJ := $(TRN_SRC:%=$(BUILDDIR)/%.o)
TRN_DEP := $(TRN_OBJ:%.o=%.d)

OBJ += $(TRN_OBJ)
DEP += $(TRN_DEP)

OVERLAY += TRN

VS_SRC := \
	$(wildcard $(ASM_DIR)/vs/*.s) \
	$(wildcard $(ASM_DIR)/vs/data/*.s)

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
	$(OBJDIFF) report generate > $(BUILDDIR)/report.json

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
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
	@$(OBJCOPY) --set-section-alignment .text=4 \
				--set-section-alignment .data=4 \
				--set-section-alignment .rodata=4 \
				--set-section-alignment .bss=4 \
				--set-section-alignment .sbss=4 \
				--set-section-alignment .sdata=4 $@

$(BUILDDIR)/%.rodata.s.o: %.rodata.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) $<
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -Wa,--defsym,_RODATA=1 -o $@ $<

$(BUILDDIR)/%.data.s.o: %.data.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) $<
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -Wa,--defsym,_DATA=1 -o $@ $<

$(BUILDDIR)/%.sdata.s.o: %.sdata.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) $<
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -Wa,--defsym,_SDATA=1 -o $@ $<

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
