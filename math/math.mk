SRCS_C := $(wildcard *.c)
TARGETS_C := $(subst .c, ,$(SRCS_C))
TARGETS_C :=  $(foreach i,$(TARGETS_C),$(TARGETS_DIR)/$(i))

SRCS_CXX := $(wildcard *.cpp)
TARGETS_CXX := $(subst .cpp, ,$(SRCS_CXX))
TARGETS_CXX :=  $(foreach i,$(TARGETS_CXX),$(TARGETS_DIR)/$(i))

LOCAL_LDLIBS := -lm -lpthread -m32

#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
