#=========================================================================
TARGETS_C := test_mspace
TARGETS_C += test_dlmalloc

TARGETS_C := $(foreach i,$(TARGETS_C),$(TARGETS_DIR)/$(i))

CFLAGS := -DANDROID_SMP=0 -m32
#=========================================================================	
LOCAL_LDLIBS := -lm -lpthread -m32
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
