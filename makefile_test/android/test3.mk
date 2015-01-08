LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := test3

$(call ndk_log,"current = $(LOCAL_MODULE)")

include build.mk
