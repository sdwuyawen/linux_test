#=========================================================================
TARGETS_C := $(TARGETS_DIR)/pulse-test

LOCAL_LDLIBS := -lpulse -lpulsecommon-0.9.21 -lpulse-simple

#=========================================================================
all: $(TARGETS_C)
	@echo "=========== OK =========="
		
clean:
	@rm -f $(TARGET)

#=========================================================================	
include $(TARGETS_ROOT)/mkconfig	
