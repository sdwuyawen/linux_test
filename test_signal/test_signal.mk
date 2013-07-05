
#=========================================================================
FILTER_SRCS := $(shell cat filter.lst)
FILTER_SRCS_C := $(filter *.c,$(FILTER_SRCS))
FILTER_SRCS_CXX := $(filter *.cpp,$(FILTER_SRCS))

#=========================================================================

SRCS_C := $(wildcard *.c)
SRCS_C := $(filter-out $(FILTER_SRCS_C),$(SRCS_C))
TARGETS_C := $(subst .c, ,$(SRCS_C))
TARGETS_C :=  $(foreach i,$(TARGETS_C),$(TARGETS_DIR)/$(i))

SRCS_CXX := $(wildcard *.cpp)
SRCS_CXX := $(filter-out $(FILTER_SRCS_CXX),$(SRCS_CXX))
TARGETS_CXX := $(subst .cpp, ,$(SRCS_CXX))
TARGETS_CXX :=  $(foreach i,$(TARGETS_CXX),$(TARGETS_DIR)/$(i))

#=========================================================================
include $(TARGETS_ROOT)/mkconfig	
