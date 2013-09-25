#=========================================================================
TARGETS := $(TARGETS_DIR)/gtk-test

SRCS_C := gtk-test.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

OBJS := $(OBJS_C) $(OBJS_CXX)

#=========================================================================	
LOCAL_LDLIBS := 
CFLAGS := `pkg-config --libs --cflags gtk+-2.0`
LDFLAGS := `pkg-config --libs --cflags gtk+-2.0`

#=========================================================================	
$(TARGETS): $(OBJS)
	@echo "[LD] $(TARGETS)"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $< $(LOCAL_LDLIBS)	
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
