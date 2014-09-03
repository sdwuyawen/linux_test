##
#
# note :
#	1). echo $(LS)输出：
#		good-are good-you good-ok	
#	2). 
#		$(wildcard, <list>)是返回list中存在的路径(文件)	
#	3). 
#		eval函数 : 在当前位置插入参数内容
#					
##
k = $(2)$(1) 
map = $(foreach a,$(2),$(call $(1),$(a),good-)) 
o = $(call map,k,are you ok) 

pathsearch = $(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))     
LS := $(call pathsearch,ls)

.PHONY: all test

all: test foo
	@echo "all"
	@echo "$(o)"
	@echo $(PATH)
	@echo $(addsuffix /ls,$(subst :, ,$(PATH)))
	@echo $(wildcard $(addsuffix /ls,$(subst :, ,$(PATH))))
	@echo $(LS)

test:
	@echo "test"

define func
.PHONY: $1
$1:
	@echo "at foo"
endef

$(eval $(call func,foo))
