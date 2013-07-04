#=========================================================================
TARGETS := $(TARGETS_DIR)/test_so1
TARGETS_1 := $(TARGETS_DIR)/test_so1_1
TARGETS_LIB := $(TARGETS_DIR)/libtest_so1.so

SRCS_C := test_so1.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS := $(OBJS_C) $(OBJS_CXX)

SRCS1_C := test_so1_1.c
OBJS1_C := $(subst .c,.o,$(SRCS1_C))
OBJS1_C := $(foreach i,$(OBJS1_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS1 := $(OBJS1_C) $(OBJS1_CXX)

LIB_SRCS_C := libtest_so1.c
LIB_OBJS_C := $(subst .c,.o,$(LIB_SRCS_C))
LIB_OBJS_C := $(foreach i,$(LIB_OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

#=========================================================================	
CFLAGS += -fPIC

LOCAL_LDLIBS := 
LOCAL_LIB_LDLIBS :=

#=========================================================================	
$(TARGETS): $(TARGETS_LIB) $(TARGETS_1) $(OBJS) 
	@echo "[LD] $@"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $(OBJS) $(TARGETS_LIB) $(LOCAL_LDLIBS)	

$(TARGETS_1): $(OBJS1)
	@echo "[LD] $@"
	$(_Q)#$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(TARGETS_LIB) $(LOCAL_LDLIBS)	
	$(_Q)$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LOCAL_LDLIBS)	
	
$(TARGETS_LIB): $(LIB_OBJS_C)
	@echo "[LD] $(TARGETS_LIB)"
	$(_Q)$(CC) -shared $(LDFLAGS) -o $@ $^ $(LOCAL_LIB_LDLIBS)
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
