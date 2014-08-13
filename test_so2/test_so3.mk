#=========================================================================
TARGETS := $(TARGETS_DIR)/test_so3
TARGETS_LIB := $(TARGETS_DIR)/libtest_so3.so

SRCS_C := test_so3.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS := $(OBJS_C) $(OBJS_CXX)

LIB_SRCS_C := libtest_so3.c
LIB_OBJS_C := $(subst .c,.o,$(LIB_SRCS_C))
LIB_OBJS_C := $(foreach i,$(LIB_OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

#=========================================================================	
CFLAGS += -fPIC

LOCAL_LDLIBS := -ldl
LOCAL_LIB_LDLIBS :=

#=========================================================================	
$(TARGETS): $(TARGETS_LIB) $(OBJS) 
	@echo "[LD] $@"
	$(_Q)$(CC) -DTEST_SO $(LDFLAGS) -o $@ $(OBJS) $(LOCAL_LDLIBS)		
	
$(TARGETS_LIB): $(LIB_OBJS_C)
	@echo "[LD] $(TARGETS_LIB)"
	$(_Q)$(CC) -shared $(LDFLAGS) -o $@ $^ $(LOCAL_LIB_LDLIBS)
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
