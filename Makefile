#=========================================================================
CC := gcc
CXX := g++ 
ECHO := echo -e

ifeq ($(V), 1)
_Q :=
else
_Q := @
endif

#=========================================================================
TARGETS_ROOT := $(shell pwd)
TARGETS_DIR := $(TARGETS_ROOT)/bin
TARGETS_OBJ_DIR := .obj

#=========================================================================
export CC
export CXX
export ECHO
export _Q
export TARGETS_ROOT
export TARGETS_DIR
export TARGETS_OBJ_DIR

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
LOCAL_KERNEL_NAME := $(shell uname -r)
#LOCAL_CFLAGS := -I/lib/modules/${LOCAL_KERNEL_NAME}/build/include -D_GNU_SOURCE
#LOCAL_CFLAGS += -I/lib/modules/${LOCAL_KERNEL_NAME}/build/arch/x86/include

LOCAL_CFLAGS += -D_GNU_SOURCE

LOCAL_LDLIBS := -lpthread
LOCAL_LDLIBS += -m32

LOCAL_ROOT_DIR := $(shell pwd)

CFLAGS += $(LOCAL_CFLAGS)
#=========================================================================
MK_FILE_LIST := $(shell find $(TARGETS_ROOT) -name *.mk)

#=========================================================================
all: debug $(TARGETS_C) $(TARGETS_CXX)
	@for file in $(MK_FILE_LIST) ; do \
		cd `dirname $$file`; \
		$(ECHO) "\033[35;1m[`dirname $$file`] \033[36;1m`basename $$file` \033[0m" ; \
		$(MAKE) -f `basename $$file` all_tests; \
		cd $(LOCAL_ROOT_DIR); \
	done
	@echo "=========== build tests OK ==========="
	
debug:

clean:
	@$(ECHO) "\033[35;1m=== clean targets === \033[0m" ;
	@for file in $(MK_FILE_LIST) ; do \
		cd `dirname $$file`; \
		$(ECHO) "\033[35;1m[`dirname $$file`] \033[36;1m`basename $$file` clean \033[0m" ; \
		$(MAKE) -f `basename $$file` clean_tests; \
		cd $(LOCAL_ROOT_DIR); \
	done	
	@rm -rf bin 

mk-%:
	@for file in $(shell find $(patsubst mk-%,%,$@) -name *.mk) ; do \
		cd `dirname $$file`; \
		$(ECHO) "\033[35;1m[`dirname $$file`] \033[36;1m`basename $$file` \033[0m" ; \
		$(MAKE) -f `basename $$file` all_tests; \
		cd $(LOCAL_ROOT_DIR); \
	done
		
clean-%:
	@for file in $(shell find $(patsubst clean-%,%,$@) -name *.mk) ; do \
		cd `dirname $$file`; \
		$(ECHO) "\033[35;1m[`dirname $$file`] \033[36;1m`basename $$file` clean \033[0m" ; \
		$(MAKE) -f `basename $$file` clean_tests; \
		cd $(LOCAL_ROOT_DIR); \
	done
	
#=========================================================================
include ./mkconfig
