
LOCAL_PATH := $(call my-dir)

all:
	@$(call debug_out)
	@echo $(MAKEFILE_LIST)
	@echo $(LOCAL_PATH)
	@echo "level = $(MAKELEVEL)"
