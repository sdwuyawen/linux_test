#=========================================================================
TARGETS := $(TARGETS_DIR)/gtk-rect

SRCS_C := gtk-rect.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

OBJS := $(OBJS_C) $(OBJS_CXX)

#=========================================================================	
CFLAGS := `pkg-config --cflags gtk+-2.0`
LOCAL_LDLIBS := `pkg-config --libs gtk+-2.0`

#=========================================================================	
$(TARGETS): $(OBJS)
	@echo "[LD] $(TARGETS)"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $< $(LOCAL_LDLIBS)	
	
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
