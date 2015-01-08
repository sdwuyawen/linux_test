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
#	6). 在include <file>时, 如果makefile中有目标<file>， 就会执行目标<file>		
#	7). shexport命令输出全局变量		
#	8). 导入shell命令
#   9). $(call Package/test/description), 会在当前展开Package/test/description, 所以要注意多行语句(echo).
#		通过 call 函数可以将一个变量定义为一个复杂的表达式, 格式如下：
#		$(call VAR, 参数1, 参数2,...)
#		call 函数可以将 "参数1“ ，”参数2“ 传递到VAR中, VAR根据传递进来的参数进行展开成一个完整的表达式.
#	10). 命令行中调用makefile的函数dirname basename, 需要$$, 如果是函数内部，需要$$$$
##
k = $(2)$(1) 
map = $(foreach a,$(2),$(call $(1),$(a),good-)) 
o = $(call map,k,are you ok) 
$(info o:$(o))
pathsearch = $(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))     
LS := $(call pathsearch,ls)

export SHELL=/usr/bin/env bash -c '. shell.sh; eval "$$2"' --

NO_TRACE_MAKE := make

.PHONY: all test stamp clean
all: test foo stamp .obj/1.txt
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
	@echo $(sort $o)

define func
.PHONY: $1
$1:
	@echo "at $$@"
endef

$(eval $(call func,foo))

stamp::
	@+( \
		echo "stamp 1"; \
		echo "make test"; \
		$(NO_TRACE_MAKE) test; \
		MD5SUM=$$(cat Makefile | (md5sum || md5) 2>/dev/null | awk '{print $$1}'); \
		echo "md5 of Makefile : $$MD5SUM"; \
	)

define shvar
V_$(subst .,_,$(subst -,_,$(subst /,_,$(1))))
endef

define shexport
$(call shvar,$(1))=$$(call $(1))
export $(call shvar,$(1))
endef
	
define Package/test/description
	1.Voice Access mini-PBX Demo Application
#	2.Voice Access mini-PBX Demo Application
endef

define Package/test/config
	menu "Configuration"
		depends EXTERNAL_TOOLCHAIN && PACKAGE_libgcc

	config LIBGCC_ROOT_DIR
		string
		prompt "libgcc shared library base directory"
		depends EXTERNAL_TOOLCHAIN && PACKAGE_libgcc
		default TOOLCHAIN_ROOT  if !NATIVE_TOOLCHAIN
		default "/"  if NATIVE_TOOLCHAIN

	config LIBGCC_FILE_SPEC
		string
		prompt "libgcc shared library files (use wildcards)"
		depends EXTERNAL_TOOLCHAIN && PACKAGE_libgcc
		default "./lib/libgcc_s.so.*"

	endmenu
endef

define Dumpinfo/Package
$(info Package: $(1)
$(if $(Package/$(1)/description),Description:
$(Package/$(1)/description)
)@@
$(if $(Package/$(1)/config),Config:
$(Package/$(1)/config)
)@@)
endef

define BuildPackage
  $(call shexport,Package/$(1)/description)
  $(call shexport,Package/$(1)/config)

  $(if $(DUMP),$(Dumpinfo/Package))
endef

stamp::
	@$(eval $(call BuildPackage,test))
	@echo $(call Package/test/description)
	@echo "Description:"; \
	getvar $(call shvar,Package/test/description);
	@echo "Config :"; \
	getvar $(call shvar,Package/test/config);
	@echo "stamp 2"	

.obj/file.mk:
	@echo $@ - $$(basename $@)
	@mkdir -p $(@D)
	@(echo "hello" | awk '{ \
			print "$$(eval $$(call func,test1))"; \
		}'; \
	) > $@

all: test1			
-include .obj/file.mk	
#include也是作为依赖

define stampfile
.obj/stamp: 	
	@echo $$@ - $$$$(dirname $$@) - $$(@D)
	@mkdir -p $$$$(dirname $$@)
	@touch $$@
endef	

$(eval $(call stampfile))
all: .obj/stamp

clean :
	-rm -rf .obj
	
.obj/1.txt: FORCE	
	@echo -e "\033[34;1malways out of date" 
	@mkdir -p $(@D)
	@touch $@
	@ls -l $@
	@echo -e "\033[0m"

# A target that is always out of date.
FORCE:
.PHONY: FORCE
