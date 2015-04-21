#=========================================================================
SRCS_C := $(wildcard *.c)
TARGETS_C := $(subst .c, ,$(SRCS_C))
TARGETS_C :=  $(foreach i,$(TARGETS_C),$(TARGETS_DIR)/$(i))

SRCS_CXX := $(wildcard *.cpp)
TARGETS_CXX := $(subst .cpp, ,$(SRCS_CXX))
TARGETS_CXX :=  $(foreach i,$(TARGETS_CXX),$(TARGETS_DIR)/$(i))

#=========================================================================	
LOCAL_CFLAGS += -D_GNU_SOURCE

LOCAL_LDLIBS := -lpthread -lcap

CFLAGS += $(LOCAL_CFLAGS)
#=========================================================================		
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
