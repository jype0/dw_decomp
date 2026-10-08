CROSS ?= mipsel-linux-gnu-

CC := $(CROSS)gcc
LD := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy

SPLAT := $(PYTHON) -m splat split

ELF := $(BUILD_DIR)/$(EXE_NAME).elf
EXE := $(BUILD_DIR)/$(EXE_NAME)

INC := -Iinclude/$(TOOLCHAIN) -I$(PSYQ_INCLUDE) -Iinclude

LDSCRIPT := \
	$(CONFIG_DIR)/overlay.ld \
	$(CONFIG_DIR)/main.ld

CPPLDSCRIPT := $(LDSCRIPT:$(CONFIG_DIR)/%=$(BUILD_DIR)/config/%)

ARCHFLAGS := -march=r3000 -mtune=r3000 -mabi=32 -EL -mfp32 -msoft-float \
	     -fno-pic -mno-shared -mno-abicalls -mno-llsc \
	     -fno-stack-protector -nostdlib -ffreestanding \
	     -Xassembler -no-pad-sections
ASFLAGS := -Wa,--sectname-subst
CFLAGS := -g -Wall -Wextra -Werror -std=c99 -Os -G0 -mno-gpopt $(ARCHFLAGS)
CPPFLAGS := -DLANGUAGE_C $(addprefix -D,$(VERSION_MACROS)) $(INC)
DEPFLAGS = -MM -MF $(@:.o=.d) -MT $@
LDFLAGS := -g $(addprefix -T ,$(CPPLDSCRIPT) $(UNDEFINED_SYMS)) -static \
	   -Wl,--no-check-sections -Wl,-Map=% -Wl,--build-id=none \
	   -Wl,--gc-sections -Wl,--print-gc-sections

LINKER_SCRIPTS := $(addprefix $(GEN_DIR)/,\
		  $(addsuffix .ld, main \
		  $(shell echo $(OVERLAY) | tr A-Z a-z)))

all: $(EXE)

generate: $(LINKER_SCRIPTS)

$(BUILD_DIR)/config/%.ld: $(CONFIG_DIR)/%.ld
	@mkdir -p $(dir $@)
	$(CPP) -P -x c $(INC) -DBUILD_DIR=$(BUILD_DIR) -o $@ $<

$(GEN_DIR)/undefined_%.ld: $(GEN_DIR)/undefined_%.txt
	sed -E 's/^(.+) = (.+);$$/PROVIDE(\1 = \2);/' $< > $@

$(BUILD_DIR)/%_REL.BIN: $(ELF) $(CPPLDSCRIPT)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -j $(@:$(BUILD_DIR)/%_REL.BIN=.%) -O binary $< $@

$(ELF): $(OBJ) $(CPPLDSCRIPT) $(UNDEFINED_SYMS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@

$(EXE): $(ELF) $(OVERLAY:%=$(BUILD_DIR)/%_REL.BIN)
	@mkdir -p $(dir $@)
	$(OBJCOPY) $(addprefix -R .,$(OVERLAY)) -O binary $< $@

define assemble
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) $<
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(ASFLAGS) -o $@ $<
	@$(OBJCOPY) --set-section-alignment .text=4 \
				--set-section-alignment .data=4 \
				--set-section-alignment .rodata=4 \
				--set-section-alignment .bss=4 \
				--set-section-alignment .sbss=4 \
				--set-section-alignment .sdata=4 $@
endef

$(BUILD_DIR)/asm/%.s.o: $(ASM_DIR)/%.s
	$(assemble)

$(BUILD_DIR)/generated/%.s.o: $(GEN_DIR)/%.s
	$(assemble)

$(MAIN_BSS) &: $(CONFIG_DIR)/bss.yaml $(CONFIG_DIR)/symbols.txt
	@mkdir -p $(dir $@)
	tools/gen_bss.py $^ $(GEN_DIR)/

$(GEN_DIR)/%.ld: $(CONFIG_DIR)/%.yaml
	@mkdir -p $(dir $@)
	$(SPLAT) $< --disassemble-all --make-full-disasm-for-code
