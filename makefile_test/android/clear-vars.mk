$(call clear-src-tags)

# Note: As a special exception, we don't want to clear LOCAL_PATH
$(call clear-vars, $(filter-out LOCAL_PATH,$(modules-LOCALS:%=LOCAL_%)))

# strip LOCAL_PATH
LOCAL_PATH := $(strip $(LOCAL_PATH))
