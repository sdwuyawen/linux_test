#=========================================================================
TARGETS := $(TARGETS_DIR)/wrap_test

TARGETS_CLEAN := $(TARGETS)

#===============================================================================

SRCS_C :=
SRCS_C += wrap_test.c
SRCS_C += wrap_malloc.c
OBJS_C := $(subst .c,.o,$(SRCS_C))
OBJS_C := $(foreach i,$(OBJS_C),$(TARGETS_OBJ_DIR)/$(i))

SRCS_CXX :=
#SRCS_CXX += wrap_new.cpp
OBJS_CXX := $(subst .cpp,.o,$(SRCS_CXX))
OBJS_CXX := $(foreach i,$(OBJS_CXX),$(TARGETS_OBJ_DIR)/$(i))
OBJS := $(OBJS_C) $(OBJS_CXX)

#=========================================================================	
#CFLAGS += -m32
#CXXFLAGS += -m32
#LDFLAGS += -m32
LDFLAGS += -Wl,--wrap,malloc
LDFLAGS += -Wl,--wrap,free
LDFLAGS += -Wl,--wrap,calloc
LDFLAGS += -Wl,--wrap,realloc
LDFLAGS += -Wl,--wrap,_Znwj
LDFLAGS += -Wl,--wrap,_Znwm
LDFLAGS += -Wl,--wrap,_ZdlPv
LDFLAGS += -Wl,--wrap,_Znaj
LDFLAGS += -Wl,--wrap,_Znam
LDFLAGS += -Wl,--wrap,_ZdaPv
LOCAL_LDLIBS := -L./ -lwrap_lib -lstdc++

export CFLAGS
export CXXFLAGS
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
