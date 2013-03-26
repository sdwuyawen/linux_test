
SRCS_C := $(wildcard *.c)
TARGETS := $(subst .c, ,$(SRCS_C))
TARGETS :=  $(foreach i,$(TARGETS),bin/$(i))

CC := gcc 

LOCAL_CFLAGS := -I/lib/modules/`uname -r`/build/include
LOCAL_LDLIBS := -lpthread -m32

CFLAGS += $(LOCAL_CFLAGS)

all: $(TARGETS)
	@echo "=========== build tests OK ==========="

clean:
	@rm -rf bin 

$(TARGETS): bin/% : %.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	@echo "\033[34;1m[CC] $< -> $@ \033[0m"
	@$(CC) $(CFLAGS) -o $@ $< $(LOCAL_LDLIBS)
