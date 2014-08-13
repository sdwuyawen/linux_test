#=========================================================================
TARGETS := $(TARGETS_DIR)/test_so2
TARGETS_1 := $(TARGETS_DIR)/test_so2_1
TARGETS_LIB := $(TARGETS_DIR)/libtest_so2.so
TARGETS_LIB1 := $(TARGETS_DIR)/libtest_so2_1.so
TARGETS_LIB2 := $(TARGETS_DIR)/libtest_so2_2.so

SRCS_C := test_so2.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS := $(OBJS_C) $(OBJS_CXX)

SRCS1_C := test_so2_1.c
OBJS1_C := $(subst .c,.o,$(SRCS1_C))
OBJS1_C := $(foreach i,$(OBJS1_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS1 := $(OBJS1_C) $(OBJS1_CXX)

LIB_SRCS_C := libtest_so2.c
LIB_OBJS_C := $(subst .c,.o,$(LIB_SRCS_C))
LIB_OBJS_C := $(foreach i,$(LIB_OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

LIB_SRCS1_C := libtest_so2_1.c
LIB_OBJS1_C := $(subst .c,.o,$(LIB_SRCS1_C))
LIB_OBJS1_C := $(foreach i,$(LIB_OBJS1_C),$(TARGETS_OBJ_DIR)/$(i))

LIB_SRCS2_C := libtest_so2_2.c
LIB_OBJS2_C := $(subst .c,.o,$(LIB_SRCS2_C))
LIB_OBJS2_C := $(foreach i,$(LIB_OBJS2_C),$(TARGETS_OBJ_DIR)/$(i))

#=========================================================================	
CFLAGS += -fPIC

LOCAL_LDLIBS := -ldl
LOCAL_LIB_LDLIBS :=

#=========================================================================	
$(TARGETS): $(TARGETS_LIB) $(TARGETS_LIB1) $(TARGETS_LIB2) $(TARGETS_1) $(OBJS) 
	@echo "[LD] $@"
	$(_Q)$(CC) -DTEST_SO $(LDFLAGS) -o $@ $(OBJS) $(TARGETS_LIB) $(LOCAL_LDLIBS)	

$(TARGETS_1): $(OBJS1) 
	@echo "[LD] $@"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LOCAL_LDLIBS)	

$(TARGETS_LIB2): $(LIB_OBJS2_C)
	@echo "[LD] $@"
	$(_Q)$(CC) -shared $(LDFLAGS) -o $@ $^ $(TARGETS_LIB) $(LOCAL_LDLIBS)		
	
$(TARGETS_LIB1): $(LIB_OBJS1_C)
	@echo "[LD] $@"
	$(_Q)$(CC) -shared $(LDFLAGS) -o $@ $^ $(TARGETS_LIB) $(LOCAL_LDLIBS)		
	
$(TARGETS_LIB): $(LIB_OBJS_C)
	@echo "[LD] $(TARGETS_LIB)"
	$(_Q)$(CC) -shared $(LDFLAGS) -o $@ $^ $(LOCAL_LIB_LDLIBS)
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
