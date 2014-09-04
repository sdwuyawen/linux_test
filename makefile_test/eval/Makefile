# sample Makefile
OBJ_DIR := .obj

PROGRAMS = $(OBJ_DIR)/server $(OBJ_DIR)/client
server_OBJS = $(OBJ_DIR)/server.o $(OBJ_DIR)/server_priv.o $(OBJ_DIR)/server_access.o
server_LIBS = abc d
client_OBJS = $(OBJ_DIR)/client.o $(OBJ_DIR)/client_api.o $(OBJ_DIR)/client_mem.o
client_LIBS = abc d

# Everything after this is generic
.PHONY: all test
all: $(PROGRAMS)

define PROGRAM_template
$(1): $$($(notdir $(1))_OBJS)
$(info $(1))
$(eval ALL_OBJS += $$($(notdir $(1))_OBJS))
$(info $(ALL_OBJS))
endef

#ALL_OBJS += $$($(notdir $(1))_OBJS)

$(foreach prog,$(PROGRAMS),$(eval $(call PROGRAM_template,$(prog))))

#$(PROGRAMS): test
#test:
#	@echo "hell world"
	
$(PROGRAMS):
	@echo $@ 
	@echo $($(@F)_LIBS:%=-l%)
	@echo $(@F)_OBJS = $($(@F)_OBJS)
	@echo ALL_OBJS = $(ALL_OBJS)
	@[ -e $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	gcc $^ -L$(OBJ_DIR)/ $($(@F)_LIBS:%=-l%) $(LDLIBS) -o $@

clean:
	rm -f $(ALL_OBJS) $(PROGRAMS)

$(OBJ_DIR)/%.o:%.cpp
	@[ -e $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	g++ -c $< -o $@
	
$(OBJ_DIR)/%.o : %.c
	@[ -e $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	gcc -c -o $@ $<	
