#=========================================================================
#TARGETS_C := $(TARGETS_DIR)/pulse-test
TARGETS := $(TARGETS_DIR)/test_so
TARGETS_LIB := $(TARGETS_DIR)/libtest.so
TARGETS_LIB1 := $(TARGETS_DIR)/libtest1.so

TARGETS_CLEAN := $(TARGETS) $(TARGETS_LIB) $(TARGETS_LIB1)

SRCS_C := test_so.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))
OBJS := $(OBJS_C) $(OBJS_CXX)

LIB_SRCS_CXX := libtest_so.cpp
LIB_OBJS_CXX := $(subst .cpp,.o,$(LIB_SRCS_CXX))
LIB_OBJS_CXX := $(foreach i,$(LIB_OBJS_CXX),$(TARGETS_OBJ_DIR)/$(i))

LIB_SRCS_C := libtest1_so.c
LIB_OBJS_C := $(subst .c,.o,$(LIB_SRCS_C))
LIB_OBJS_C := $(foreach i,$(LIB_OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

#=========================================================================	
CFLAGS += -fPIC

LOCAL_LDLIBS := -ldl
LOCAL_LIB_LDLIBS :=

#=========================================================================	
$(TARGETS): $(TARGETS_LIB) $(TARGETS_LIB1) $(OBJS) 
	@echo "[LD] $(TARGETS)"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LOCAL_LDLIBS)	
	
$(TARGETS_LIB):	$(LIB_OBJS_CXX)
	@echo "[LD] $(TARGETS_LIB)"
	$(_Q)$(CXX) -shared $(LDFLAGS) -o $@ $^ $(LOCAL_LIB_LDLIBS)

$(TARGETS_LIB1): $(LIB_OBJS_C)
	@echo "[LD] $(TARGETS_LIB1)"
	$(_Q)$(CC) -shared $(LDFLAGS) -o $@ $^ $(LOCAL_LIB_LDLIBS)
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
