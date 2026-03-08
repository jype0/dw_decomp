# See LICENSE file for copyright and license details.

.EXTRA_PREREQS := $(abspath $(lastword $(MAKEFILE_LIST)))

-include local.mk

ifeq ($(MWCCWRAP),)
	$(error MWCCWRAP path not set)
endif

TOOLCHAIN ?= mipsel-linux-gnu-

BUILDDIR := build

ELF := $(BUILDDIR)/SLUS_010.32.elf
EXE := $(BUILDDIR)/SLUS_010.32

CC := $(TOOLCHAIN)gcc
LD := $(TOOLCHAIN)ld
OBJCOPY := $(TOOLCHAIN)objcopy

MWCCWRAP_FLAGS += -gccincludes -lang c -Cpp_exceptions off -RTTI off

INC := -Iinclude

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
DEPFLAGS = -MMD
LDFLAGS := -g $(addprefix -T ,$(CPPLDSCRIPT)) -static \
	   -Wl,--no-check-sections -Wl,-Map=% -Wl,--build-id=none

SRC := \
	$(wildcard asm/main/*.s) \
	$(wildcard asm/main/data/*.s) \
	$(wildcard asm/main/data/psyq/*.s) \
	$(wildcard asm/main/psyq/*.s) \
	$(BUILDDIR)/generated/bss.s \
	$(BUILDDIR)/generated/sbss.s

OBJ := $(SRC:%=$(BUILDDIR)/%.o)
DEP := $(OBJ:%.o=%.d)

OVERLAY :=

BTL_SRC := \
	$(wildcard asm/btl/*.s) \
	$(wildcard asm/btl/data/*.s)

BTL_OBJ := $(BTL_SRC:%=$(BUILDDIR)/%.o)
BTL_DEP := $(BTL_OBJ:%.o=%.d)

OBJ += $(BTL_OBJ)
DEP += $(BTL_DEP)

OVERLAY += BTL

DGET_SRC := \
	$(wildcard asm/dget/*.s) \
	$(wildcard asm/dget/data/*.s)

DGET_OBJ := $(DGET_SRC:%=$(BUILDDIR)/%.o)
DGET_DEP := $(DGET_OBJ:%.o=%.d)

OBJ += $(DGET_OBJ)
DEP += $(DGET_DEP)

OVERLAY += DGET

DOO2_SRC := \
	$(wildcard asm/doo2/*.s) \
	$(wildcard asm/doo2/data/*.s)

DOO2_OBJ := $(DOO2_SRC:%=$(BUILDDIR)/%.o)
DOO2_DEP := $(DOO2_OBJ:%.o=%.d)

OBJ += $(DOO2_OBJ)
DEP += $(DOO2_DEP)

OVERLAY += DOO2

DOOA_SRC := \
	$(wildcard asm/dooa/*.s) \
	$(wildcard asm/dooa/data/*.s)

DOOA_OBJ := $(DOOA_SRC:%=$(BUILDDIR)/%.o)
DOOA_DEP := $(DOOA_OBJ:%.o=%.d)

OBJ += $(DOOA_OBJ)
DEP += $(DOOA_DEP)

OVERLAY += DOOA

EAB_SRC := \
	$(wildcard asm/eab/*.s) \
	$(wildcard asm/eab/data/*.s)

EAB_OBJ := $(EAB_SRC:%=$(BUILDDIR)/%.o)
EAB_DEP := $(EAB_OBJ:%.o=%.d)

OBJ += $(EAB_OBJ)
DEP += $(EAB_DEP)

OVERLAY += EAB

ENDI_SRC := \
	$(wildcard asm/endi/*.s) \
	$(wildcard asm/endi/data/*.s)

ENDI_OBJ := $(ENDI_SRC:%=$(BUILDDIR)/%.o)
ENDI_DEP := $(ENDI_OBJ:%.o=%.d)

OBJ += $(ENDI_OBJ)
DEP += $(ENDI_DEP)

OVERLAY += ENDI

EVL_SRC := \
	$(wildcard asm/evl/*.s) \
	$(wildcard asm/evl/data/*.s)

EVL_OBJ := $(EVL_SRC:%=$(BUILDDIR)/%.o)
EVL_DEP := $(EVL_OBJ:%.o=%.d)

OBJ += $(EVL_OBJ)
DEP += $(EVL_DEP)

OVERLAY += EVL

FISH_SRC := \
	$(wildcard asm/fish/*.s) \
	$(wildcard asm/fish/data/*.s)

FISH_OBJ := $(FISH_SRC:%=$(BUILDDIR)/%.o)
FISH_DEP := $(FISH_OBJ:%.o=%.d)

OBJ += $(FISH_OBJ)
DEP += $(FISH_DEP)

OVERLAY += FISH

KAR_SRC := \
	$(wildcard asm/kar/*.s) \
	$(wildcard asm/kar/data/*.s)

KAR_OBJ := $(KAR_SRC:%=$(BUILDDIR)/%.o)
KAR_DEP := $(KAR_OBJ:%.o=%.d)

OBJ += $(KAR_OBJ)
DEP += $(KAR_DEP)

OVERLAY += KAR

MOV_SRC := \
	$(wildcard asm/mov/*.s) \
	$(wildcard asm/mov/data/*.s)

MOV_OBJ := $(MOV_SRC:%=$(BUILDDIR)/%.o)
MOV_DEP := $(MOV_OBJ:%.o=%.d)

OBJ += $(MOV_OBJ)
DEP += $(MOV_DEP)

OVERLAY += MOV

MURD_SRC := \
	$(wildcard asm/murd/*.s) \
	$(wildcard asm/murd/data/*.s)

MURD_OBJ := $(MURD_SRC:%=$(BUILDDIR)/%.o)
MURD_DEP := $(MURD_OBJ:%.o=%.d)

OBJ += $(MURD_OBJ)
DEP += $(MURD_DEP)

OVERLAY += MURD

SHOP_SRC := \
	$(wildcard asm/shop/*.s) \
	$(wildcard asm/shop/data/*.s)

SHOP_OBJ := $(SHOP_SRC:%=$(BUILDDIR)/%.o)
SHOP_DEP := $(SHOP_OBJ:%.o=%.d)

OBJ += $(SHOP_OBJ)
DEP += $(SHOP_DEP)

OVERLAY += SHOP

STD_SRC := \
	$(wildcard asm/std/*.s) \
	$(wildcard asm/std/data/*.s)

STD_OBJ := $(STD_SRC:%=$(BUILDDIR)/%.o)
STD_DEP := $(STD_OBJ:%.o=%.d)

OBJ += $(STD_OBJ)
DEP += $(STD_DEP)

OVERLAY += STD

TRN2_SRC := \
	$(wildcard asm/trn2/*.s) \
	$(wildcard asm/trn2/data/*.s)

TRN2_OBJ := $(TRN2_SRC:%=$(BUILDDIR)/%.o)
TRN2_DEP := $(TRN2_OBJ:%.o=%.d)

OBJ += $(TRN2_OBJ)
DEP += $(TRN2_DEP)

OVERLAY += TRN2

TRN_SRC := \
	$(wildcard asm/trn/*.s) \
	$(wildcard asm/trn/data/*.s)

TRN_OBJ := $(TRN_SRC:%=$(BUILDDIR)/%.o)
TRN_DEP := $(TRN_OBJ:%.o=%.d)

OBJ += $(TRN_OBJ)
DEP += $(TRN_DEP)

OVERLAY += TRN

VS_SRC := \
	$(wildcard asm/vs/*.s) \
	$(wildcard asm/vs/data/*.s)

VS_OBJ := $(VS_SRC:%=$(BUILDDIR)/%.o)
VS_DEP := $(VS_OBJ:%.o=%.d)

OBJ += $(VS_OBJ)
DEP += $(VS_DEP)

OVERLAY += VS

all: $(EXE)

$(BUILDDIR)/%.ld: %.ld
	@mkdir -p $(dir $@)
	$(CPP) -P -x c $(INC) -o $@ $<

$(BUILDDIR)/%_REL.BIN: $(ELF) $(CPPLDSCRIPT)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -j $(@:$(BUILDDIR)/%_REL.BIN=.%) -O binary $< $@

$(ELF): $(OBJ) $(CPPLDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LIB)

$(EXE): $(ELF) $(OVERLAY:%=$(BUILDDIR)/%_REL.BIN)
	@mkdir -p $(dir $@)
	$(OBJCOPY) $(addprefix -R .,$(OVERLAY)) -O binary $< $@

-include $(DEP)

$(BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(MWCCWRAP) $(MWCCWRAP_FLAGS) $(CPPFLAGS) $(DEPFLAGS) -c -o $@ $<
	# Fix ELF header flags
	@printf '01100000' | xxd -r -p | dd of=$@ bs=1 seek=36 count=4 conv=notrunc

$(BUILDDIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -o $@ $<

$(BUILDDIR)/generated/bss.s: config/symbols.txt
	@mkdir -p $(dir $@)
	tools/gen_bss.py $< $@

$(BUILDDIR)/generated/sbss.s: config/symbols.txt
	@mkdir -p $(dir $@)
	tools/gen_sbss.py $< $@

clean:
	rm -f $(ELF) $(EXE) $(OBJ) $(DEP)

.PHONY: all clean
