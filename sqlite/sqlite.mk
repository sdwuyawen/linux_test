#=========================================================================
TARGETS := $(TARGETS_DIR)/test_sqlite

SRCS_C := $(wildcard *.c)
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

OBJS := $(OBJS_C) $(OBJS_CXX)

CFLAGS := -DSQLITE_OMIT_LOAD_EXTENSION
#=========================================================================	
LOCAL_LDLIBS := -lm -lpthread

#=========================================================================	
$(TARGETS): $(OBJS)
	@echo "[LD] $(TARGETS)"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $^ $(LOCAL_LDLIBS)	
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
