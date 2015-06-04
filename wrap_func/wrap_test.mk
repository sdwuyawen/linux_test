#=========================================================================
TARGETS := $(TARGETS_DIR)/wrap_test

TARGETS_CLEAN := $(TARGETS)

SRCS_C := wrap_test.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

SRCS_CXX := wrap_test1.cpp
OBJS_CXX := $(subst .cpp,.o,$(SRCS_CXX))
OBJS_CXX := $(foreach i,$(OBJS_CXX),$(TARGETS_OBJ_DIR)/$(i))
OBJS := $(OBJS_C) $(OBJS_CXX)

#=========================================================================	
#CFLAGS += -fPIC
LDFLAGS += -Wl,--wrap,malloc
LDFLAGS += -Wl,--wrap,free
LOCAL_LDLIBS := -L./ -lwrap_lib -lstdc++

#=========================================================================	
#$(TARGETS): $(TARGETS_LIB) $(OBJS) 
$(TARGETS): wrap_lib $(OBJS) 
	@echo "[LD] $@"
	$(_Q)$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LOCAL_LDLIBS)		

#=========================================================================	
clean_tests: wrap_lib_clean	

#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	

#========================================================================
wrap_lib:
	@make -f wrap_lib_mk

wrap_lib_clean:	
	@make -f wrap_lib_mk clean_tests	
