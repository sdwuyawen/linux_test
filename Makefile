#=========================================================================
CC := gcc
CXX := g++ 

ifeq ($(V), 1)
_Q :=
else
_Q := @
endif


TARGETS_ROOT := $(shell pwd)
TARGETS_DIR := $(TARGETS_ROOT)/bin

export CC
export CXX
export _Q
export TARGETS_ROOT
export TARGETS_DIR

#=========================================================================
FILTER_SRCS := $(shell cat filter.lst)
FILTER_SRCS_C := $(filter *.c,$(FILTER_SRCS))
FILTER_SRCS_CXX := $(filter *.cpp,$(FILTER_SRCS))

SRCS_C := $(wildcard *.c)
SRCS_C := $(filter-out $(FILTER_SRCS_C),$(SRCS_C))
TARGETS_C := $(subst .c, ,$(SRCS_C))
TARGETS_C :=  $(foreach i,$(TARGETS_C),$(TARGETS_DIR)/$(i))

SRCS_CXX := $(wildcard *.cpp)
SRCS_CXX := $(filter-out $(FILTER_SRCS_CXX),$(SRCS_CXX))
TARGETS_CXX := $(subst .cpp, ,$(SRCS_CXX))
TARGETS_CXX :=  $(foreach i,$(TARGETS_CXX),$(TARGETS_DIR)/$(i))

#=========================================================================
LOCAL_CFLAGS := -I/lib/modules/`uname -r`/build/include
LOCAL_LDLIBS := -lpthread -m32

CFLAGS += $(LOCAL_CFLAGS)
#=========================================================================
MK_FILE_LIST := $(shell find -name *.mk)

#=========================================================================
all: debug $(TARGETS_C) $(TARGETS_CXX)
	@for file in $(MK_FILE_LIST) ; do \
		cd `dirname $$file`; \
		echo -e "\033[32;1m=== make -f $$file === \033[0m" ; \
		$(MAKE) -f `basename $$file` $@; \
		cd -; \
	done
	@echo "=========== build tests OK ==========="

debug:

clean:
	@rm -rf bin 

#=========================================================================
include ./mkconfig
