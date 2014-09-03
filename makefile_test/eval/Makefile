# sample Makefile
PROGRAMS = server client
server_OBJS = server.o server_priv.o server_access.o
server_LIBS = abc d
client_OBJS = client.o client_api.o client_mem.o
client_LIBS = abc d
# Everything after this is generic
.PHONY: all test
all: $(PROGRAMS)
define PROGRAM_template
$(1): $$($(1)_OBJS) $$($(1)_LIBS:%=-l%)
$(info $(1))
ALL_OBJS += $$($(1)_OBJS)
endef

$(foreach prog,$(PROGRAMS),$(eval $(call PROGRAM_template,$(prog))))

#$(PROGRAMS): test
#test:
#	@echo "hell world"
	
$(PROGRAMS):
	@echo $@ 
	@echo $($@_LIBS:%=-l%)
	@echo $($@_OBJS)
	$(LINK.o) $^ $(LDLIBS) -o $@


clean:
	rm -f $(ALL_OBJS) $(PROGRAMS)
