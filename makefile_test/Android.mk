parent-dir = $(patsubst %/,%,$(dir $1))
my-dir = $(call parent-dir,$(lastword $(MAKEFILE_LIST)))
all-makefiles-under = $(wildcard $1/*/Android.mk)
all-subdir-makefiles = $(call all-makefiles-under, $(call my-dir))

define debug_out
@echo "======== `pwd`/$(lastword $(MAKEFILE_LIST)) ======="
@echo "======== $(shell pwd)/$(lastword $(MAKEFILE_LIST)) ======="
endef

export debug_out
export parent-dir
export my-dir
export all-makefiles-under
export all-subdir-makefiles

LOCAL_PATH := $(call my-dir)

.PHONY: all clean


all clean:
	@$(call debug_out)
	@echo "DIR = $(LOCAL_PATH)"
	@echo $(all-subdir-makefiles)
	@for file in $(all-subdir-makefiles); do \
		echo "`dirname $$file` - `basename $$file`"; \
		make -C `dirname $$file` -f `basename $$file` $(MAKECMDGOALS); \
	done
