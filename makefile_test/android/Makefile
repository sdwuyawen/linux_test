##
#
# note :
#
##
include init.mk
include definitions.mk

$(call modules-clear)

include Android.mk

#导入两次build-binary.mk
#产生test2和test3目标依赖
all: $(__ndk_modules)
	@echo "======================="
	@echo "all : $(__ndk_modules)"

$(foreach __pass2_module,$(__ndk_modules),\
    $(eval LOCAL_MODULE := $(__pass2_module))\
    $(eval include build-binary.mk)\
)
