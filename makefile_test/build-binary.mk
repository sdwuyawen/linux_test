#
# @echo "local = $(LOCAL_MODULE)" 
#   总是显示LOCAL_MODULE的最后赋值(test3)
#   在eval include过程中, 目标：依赖中的$(LOCAL_MODULE)被替换为test2 or test3
#	而目标下面的echo语句中的$(LOCAL_MODULE)没有被替换，$@和$(PRIVATE_MODULE)也不变,
#	等待最后make才去解析。
# @echo "local-@ = $@" 
#   显示LOCAL_MODULE的当前值(test2 or test3)
#	因为在展开之后，目标$(LOCAL_MODULE)已经替换为test2 or test3
# @如果依赖有PRIVATE_MODULE := $(LOCAL_MODULE)
#  应该新建一行目标：依赖
# @echo "private = $(PRIVATE_MODULE)" 
#  因为在eval include过程中, PRIVATE_MODULE := $(LOCAL_MODULE)中的$(LOCAL_MODULE)被替换为test2 or test3
#  所以make过程中，PRIVATE_MODULE被赋值为test2 or test3
#
.PHONY: $(LOCAL_MODULE)
$(LOCAL_MODULE): PRIVATE_MODULE := $(LOCAL_MODULE)	
$(LOCAL_MODULE): $(LOCAL_MODULE)-test
	@echo "local = $(LOCAL_MODULE)" 
	@echo "local-@ = $@" 
	@echo "private = $(PRIVATE_MODULE)" 

$(LOCAL_MODULE)-test:
	@echo $@
