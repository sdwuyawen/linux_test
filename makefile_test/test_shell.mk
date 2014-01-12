#shell:
# export ab=111
# export ac="\'1 2 3 4 5\'"
# export ad="\'11\' \'22 33\' \'44\' \'55\'"
# 	把ac当作一个字符串来传，makefile 就可以访问到
# 执行make, 之后输出:
##	  ==== test shell makefile ====
##	  111
##	  \'1 2 3 4 5\'
##	  \'11\' \'22 33\' \'44\' \'55\'
##	  === ae ('1 2 3 4 5') ===
##	  1 2 3 4 5
##	  === af ('11' '22 33' '44' '55') ===
##	  11
##	  22 33
##	  44
##	  55

.PHONY: all all_tests

ae := $(shell echo ${ac})
af := $(shell echo ${ad})

all:
all_tests:
	@echo "==== test shell makefile ===="
	@echo "${ab}"
	@echo "${ac}"
	@echo "${ad}"
	@echo "=== ae (${ae}) ==="
	@for dir in $(ae); do echo $$dir; done 
	@echo "=== af (${af}) ==="
	@for dir1 in $(af); do echo $$dir1; done 
