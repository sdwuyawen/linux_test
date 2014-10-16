#=========================================================================
TARGETS := $(TARGETS_DIR)/test_so2
TARGETS_1 := $(TARGETS_DIR)/test_so2_1
TARGETS_2 := $(TARGETS_DIR)/test_so2_2
TARGETS_LIB := $(TARGETS_DIR)/libtest_so2.so
TARGETS_LIB1 := $(TARGETS_DIR)/libtest_so2_1.so
TARGETS_LIB2 := $(TARGETS_DIR)/libtest_so2_2.so

TARGETS_CLEAN := $(TARGETS) $(TARGETS_1) $(TARGETS_2) $(TARGETS_LIB) $(TARGETS_LIB1) $(TARGETS_LIB2) 

SRCS_C := test_so2.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS := $(OBJS_C) $(OBJS_CXX)

SRCS1_C := test_so2_1.c
OBJS1_C := $(subst .c,.o,$(SRCS1_C))
OBJS1_C := $(foreach i,$(OBJS1_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS1 := $(OBJS1_C) $(OBJS1_CXX)

SRCS2_C := test_so2_2.c
OBJS2_C := $(subst .c,.o,$(SRCS2_C))
OBJS2_C := $(foreach i,$(OBJS2_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS2 := $(OBJS2_C) $(OBJS2_CXX)

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

#LOCAL_EXT_LIB := y

define compiler_shared
$(1) : $(2)
	@echo "[LD] $(1)"	
	$(if $(LOCAL_EXT_LIB),
		$(_Q)$(CC) -shared $(LDFLAGS) -o $(1) $(2) $(TARGETS_LIB) $(LOCAL_LDLIBS), 
		$(_Q)$(CC) -shared $(LDFLAGS) -o $(1) $(2) $(LOCAL_LDLIBS))
endef

#=========================================================================	
$(TARGETS): $(TARGETS_LIB) $(TARGETS_LIB1) $(TARGETS_LIB2) $(TARGETS_1) $(TARGETS_2) $(OBJS) 
	@echo "[LD] $@"
	$(_Q)$(CC) -DTEST_SO $(LDFLAGS) -o $@ $(OBJS) $(TARGETS_LIB) $(LOCAL_LDLIBS)	

$(TARGETS_1): $(OBJS1) 
	@echo "[LD] $@"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LOCAL_LDLIBS)	
	
$(TARGETS_2): $(OBJS2) 
	@echo "[LD] $@"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $(OBJS2) $(LOCAL_LDLIBS)		
	
$(TARGETS_LIB): $(LIB_OBJS_C)
	@echo "[LD] $(TARGETS_LIB)"
	$(_Q)$(CC) -shared $(LDFLAGS) -o $@ $^ $(LOCAL_LIB_LDLIBS)

$(eval $(call compiler_shared,$(TARGETS_LIB1),$(LIB_OBJS1_C)))
$(eval $(call compiler_shared,$(TARGETS_LIB2),$(LIB_OBJS2_C)))	
		
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
