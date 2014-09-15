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
##
k = $(2)$(1) 
map = $(foreach a,$(2),$(call $(1),$(a),good-)) 
o = $(call map,k,are you ok) 

pathsearch = $(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))     
LS := $(call pathsearch,ls)

NO_TRACE_MAKE := make

.PHONY: all test stamp

all: test foo stamp test1
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
	@echo "at $1"
endef

$(eval $(call func,foo))

stamp::
	@+( \
		echo "stamp 1"; \
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
	Voice Access mini-PBX Demo Application
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
	@$(info Config : $(V_Package_test_config))
	@$(info Description : $(V_Package_test_description))
	@echo "stamp 2"	
			
.obj/file.mk:
	@echo $@
	@mkdir -p $(@D)
	@(echo "hello" | awk '{ \
			print "$$(eval $$(call func,test1))"; \
		}'; \
	) > $@

-include .obj/file.mk	
#include也是作为依赖
