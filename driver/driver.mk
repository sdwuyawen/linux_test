#===============================================================================
MK_DIRS :=
MK_DIRS += globalmem myfifo
MK_DIRS += lddbus
MK_DIRS += mymap
MK_DIRS += myv4l

#===============================================================================
all_tests:
	@for dir in $(MK_DIRS) ; do \
		make -C $$dir; \
	done

clean_tests:
	@for dir in $(MK_DIRS) ; do \
		make -C $$dir clean; \
	done
