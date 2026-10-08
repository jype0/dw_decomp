EXE_NAME := SLES_029.14

PSYQ_INCLUDE := external/psyq_headers/mw_lib43/include

MWCC_OPT_LEVEL := 0

$(eval $(call unit,MAIN,main))

$(eval $(call overlay,BTL,btl))
$(eval $(call overlay,DGET,dget))
$(eval $(call overlay,DOO2,doo2))
$(eval $(call overlay,DOOA,dooa))
$(eval $(call overlay,EAB,eab))
$(eval $(call overlay,ENDI,endi))
$(eval $(call overlay,EVL,evl))
$(eval $(call overlay,FISH,fish))
$(eval $(call overlay,KAR,kar))
$(eval $(call overlay,MOV,mov))
$(eval $(call overlay,MURD,murd))
$(eval $(call overlay,SHOP,shop))
$(eval $(call overlay,STD,std))
$(eval $(call overlay,TRN2,trn2))
$(eval $(call overlay,TRN,trn))
$(eval $(call overlay,VS,vs))

UNDEFINED_SYMS := $(foreach u,main $(shell echo $(OVERLAY) | tr A-Z a-z), \
	$(GEN_DIR)/undefined_funcs_auto_$(u).ld \
	$(GEN_DIR)/undefined_syms_auto_$(u).ld)
