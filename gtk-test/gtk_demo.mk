#=========================================================================
SRCS_C := $(wildcard *.c)
TARGETS_C := $(subst .c, ,$(SRCS_C))
TARGETS_C :=  $(foreach i,$(TARGETS_C),$(TARGETS_DIR)/$(i))

SRCS_CXX := $(wildcard *.cpp)
TARGETS_CXX := $(subst .cpp, ,$(SRCS_CXX))
TARGETS_CXX :=  $(foreach i,$(TARGETS_CXX),$(TARGETS_DIR)/$(i))

#=========================================================================	
CFLAGS := `pkg-config --cflags gtk+-2.0`
LOCAL_LDLIBS := `pkg-config --libs gtk+-2.0`

#=========================================================================		
#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
