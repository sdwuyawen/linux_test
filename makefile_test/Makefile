##
#
# note :
#	1). echo $(LS)输出：
#		good-are good-you good-ok	
#	2). 
#		$(wildcard, <list>)是返回list中存在的路径(文件)	
#	3). 
#		eval函数 : 在当前位置插入参数内容
#	4). @$(eval R := 5)
#		@echo $R
#		输出：5
#	5). stamp::中, 双冒号表示无条件执行命令行, 不同规则做独立处理, 不会合并, 这就意味着对这些规则的处理就像多个不同的普通规则一样
#		(<命令行>)或+(<命令行>), make会做两次解析, 所以有些变量需要$$才行。
#					
##
k = $(2)$(1) 
map = $(foreach a,$(2),$(call $(1),$(a),good-)) 
o = $(call map,k,are you ok) 

pathsearch = $(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))     
LS := $(call pathsearch,ls)

NO_TRACE_MAKE := make

.PHONY: all test stamp

all: test foo stamp
	@echo "all"
	@echo "$(o)"
	@echo $(PATH)
	@echo $(addsuffix /ls,$(subst :, ,$(PATH)))
	@echo $(wildcard $(addsuffix /ls,$(subst :, ,$(PATH))))
	@echo $(LS)

test:
	@echo "test"
	@$(eval R := 5)
	@echo $R

define func
.PHONY: $1
$1:
	@echo "at foo"
endef

$(eval $(call func,foo))

stamp::
	@+( \
		echo "stamp 1"; \
		$(NO_TRACE_MAKE) test; \
		MD5SUM=$$(cat Makefile | (md5sum || md5) 2>/dev/null | awk '{print $$1}'); \
		echo "md5 of Makefile : $$MD5SUM"; \
	)

stamp::
	@echo "stamp 2"
