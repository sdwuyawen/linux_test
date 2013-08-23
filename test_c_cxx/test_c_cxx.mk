#=========================================================================
#TARGETS_C := $(TARGETS_DIR)/pulse-test
TARGETS := $(TARGETS_DIR)/test_c_cxx

SRCS_C := test_c_cxx.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

SRCS_CXX := test_c_cxx_api.cpp
OBJS_CXX := $(subst .cpp,.o,$(SRCS_CXX))
OBJS_CXX := $(foreach i,$(OBJS_CXX),$(TARGETS_OBJ_DIR)/$(i))

OBJS := $(OBJS_C) $(OBJS_CXX)

#=========================================================================	
LOCAL_LDLIBS := -lstdc++

#=========================================================================	
$(TARGETS): $(OBJS) 
	@echo "[LD] $(TARGETS)"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LOCAL_LDLIBS)	
		
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
