# The text of src/<dir>/<name>.c is in text/<dir>/<name>.yaml. Its C files
# are generated per release and must exist before the C file compiles.
TEXT_YAML := $(shell find text -mindepth 2 -name '*.yaml')
TEXT_SRC := $(TEXT_YAML:text/%.yaml=%)

$(GEN_DIR)/text/%.h: text/%.yaml tools/gen_text.py
	$(PYTHON) tools/gen_text.py $(VERSION) $< $@

$(TEXT_SRC:%=$(BUILD_DIR)/src/%.c.o): $(BUILD_DIR)/src/%.c.o: \
	$(GEN_DIR)/text/%.h
