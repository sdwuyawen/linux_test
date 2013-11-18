parent-dir = $(patsubst %/,%,$(dir $1))
my-dir = $(call parent-dir,$(lastword $(MAKEFILE_LIST)))
all-makefiles-under = $(wildcard $1/*/Android.mk)
all-subdir-makefiles = $(call all-makefiles-under, $(call my-dir))

LOCAL_PATH := $(call my-dir)

all:
	@echo "DIR = $(LOCAL_PATH)"
	@echo $(all-subdir-makefiles)
