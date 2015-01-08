# -----------------------------------------------------------------------------
NDK_LOG := $(strip $(NDK_LOG))
ifeq ($(NDK_LOG),true)
    override NDK_LOG := 1
endif

override NDK_LOG := 1

__ndk_name    := test
__ndk_info     = $(info $(__ndk_name): $1 $2 $3 $4 $5)
__ndk_warning  = $(warning $(__ndk_name): $1 $2 $3 $4 $5)
__ndk_error    = $(error $(__ndk_name): $1 $2 $3 $4 $5)

ifdef NDK_NO_WARNINGS
__ndk_warning :=
endif
ifdef NDK_NO_ERRORS
__ndk_error :=
endif

# -----------------------------------------------------------------------------
# Function : ndk_log
# Arguments: 1: text to print when NDK_LOG is defined to 1
# Returns  : None
# Usage    : $(call ndk_log,<some text>)
# -----------------------------------------------------------------------------
ifeq ($(NDK_LOG),1)
ndk_log = $(info $(__ndk_name): $1)
else
ndk_log :=
endif
