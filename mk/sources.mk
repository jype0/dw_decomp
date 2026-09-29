define unit
$(1)_ASM_SRC := $$(shell find $$(ASM_DIR)/$(2) -path '*.s' \
		-not -path '$$(ASM_DIR)/$(2)/*matchings*' \
		$$($(1)_ASM_EXCLUDE:%=-not -path '$$(ASM_DIR)/$(2)/%/*') 2> /dev/null)
$(1)_OBJ := $$($(1)_ASM_SRC:$$(ASM_DIR)/%=$$(BUILD_DIR)/asm/%.o) \
	$$($(1)_GEN_SRC:$$(GEN_DIR)/%=$$(BUILD_DIR)/generated/%.o) \
	$$($(1)_C_SRC:%=$$(BUILD_DIR)/%.o)
$(1)_DEP := $$($(1)_OBJ:%.o=%.d)
OBJ += $$($(1)_OBJ)
DEP += $$($(1)_DEP)
endef

define overlay
$(call unit,$(1),$(2))
OVERLAY += $(1)
endef

OBJ :=
DEP :=
OVERLAY :=
