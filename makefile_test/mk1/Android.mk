parent-dir = $(patsubst %/,%,$(dir $1))
my-dir = $(call parent-dir,$(lastword $(MAKEFILE_LIST)))

LOCAL_PATH := $(call my-dir)

#all:
#	@echo $(MAKEFILE_LIST)
#	@echo $(LOCAL_PATH)
