#=========================================================================
#TARGETS_C := $(TARGETS_DIR)/pulse-test
TARGETS := $(TARGETS_DIR)/pulse-test

SRCS_C := $(wildcard *.c)
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

OBJS := $(OBJS_C) $(OBJS_CXX)

#=========================================================================	
LOCAL_PULSE_ROOT := /usr/lib/$(shell uname -m)-linux-gnu
LOCAL_PULSE_ROOT := $(shell find $(LOCAL_PULSE_ROOT) -name libpulsecommon*)
LOCAL_PULSE_ROOT := $(subst /libpulsecommon,/ libpulsecommon,$(LOCAL_PULSE_ROOT))  
LOCAL_PULSE_PATH := $(word 1, $(LOCAL_PULSE_ROOT))
LOCAL_PULSECOMMON_LIB := $(word 2, $(LOCAL_PULSE_ROOT))
LOCAL_PULSECOMMON_LIB := $(word 2, $(subst libpulsecommon,lib pulsecommon, $(LOCAL_PULSECOMMON_LIB)))
LOCAL_PULSECOMMON_LIB := $(word 1, $(subst .so, .so, $(LOCAL_PULSECOMMON_LIB)))

LOCAL_LDLIBS := -L$(LOCAL_PULSE_PATH)
LOCAL_LDLIBS += -lpulse -lpulse-simple

LOCAL_LDLIBS += -l$(LOCAL_PULSECOMMON_LIB)

#=========================================================================	
$(TARGETS): $(OBJS)
	@echo "[LD] $(TARGETS)"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $^ $(LOCAL_LDLIBS)	
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
