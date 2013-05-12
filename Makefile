
SRCS_C := $(wildcard *.c)
TARGETS_C := $(subst .c, ,$(SRCS_C))
TARGETS_C :=  $(foreach i,$(TARGETS_C),bin/$(i))

SRCS_CXX := $(wildcard *.cpp)
TARGETS_CXX := $(subst .cpp, ,$(SRCS_CXX))
TARGETS_CXX :=  $(foreach i,$(TARGETS_CXX),bin/$(i))

CC := gcc
CXX := g++ 

LOCAL_CFLAGS := -I/lib/modules/`uname -r`/build/include
LOCAL_LDLIBS := -lpthread -m32

CFLAGS += $(LOCAL_CFLAGS)

all: $(TARGETS_C) $(TARGETS_CXX)
	@echo "=========== build tests OK ==========="

clean:
	@rm -rf bin 

$(TARGETS_C): bin/% : %.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	@echo "\033[34;1m[CC] $< -> $@ \033[0m"
	@$(CC) $(CFLAGS) -o $@ $< $(LOCAL_LDLIBS)

$(TARGETS_CXX): bin/% : %.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	@echo "\033[34;1m[CXX] $< -> $@ \033[0m"
	@$(CXX) $(CFLAGS) -o $@ $< $(LOCAL_LDLIBS)
