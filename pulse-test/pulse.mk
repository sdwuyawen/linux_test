#=========================================================================
#TARGETS_C := $(TARGETS_DIR)/pulse-test
TARGETS := $(TARGETS_DIR)/pulse-test

LIB_VER := $(filter 3.%,$(shell uname -r))

SRCS_C := $(wildcard *.c)
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

OBJS := $(OBJS_C) $(OBJS_CXX)

#=========================================================================	
LOCAL_LDLIBS := -lpulse -lpulse-simple

ifeq ($(LIB_VER),)
LOCAL_LDLIBS += -lpulsecommon-0.9.21 
else
LOCAL_LDLIBS += -lpulsecommon-1.1 
endif

#=========================================================================	
$(TARGETS): $(OBJS)
	@echo "[LD] $(TARGETS)"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $< $(LOCAL_LDLIBS)	
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
