TARGET_NAMES = cc-01 cc-02 cc-03 cc-04 cc-05

BUILD_DIR   := build
BIN_DIR		:= $(BUILD_DIR)/bin
DEP_DIR     := $(BUILD_DIR)/depends
OBJ_DIR     := $(BUILD_DIR)/objs
LIB_DIR		:= $(BUILD_DIR)/libs
RESULT_DIR  := $(BUILD_DIR)/results
BUILD_PATHS := $(BUILD_DIR) $(BIN_DIR) $(DEP_DIR) $(OBJ_DIR) $(LIB_DIR) $(RESULT_DIR)

# make directories
makedir = $(shell mkdir -p $1)
$(foreach dir,$(BUILD_PATHS),$(call makedir,$(dir)))

SRCS := $(wildcard *.c)			               # all source files (*.c)
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS)) # all object files (*.o)
DEPS := $(patsubst %.c,$(DEP_DIR)/%.d,$(SRCS)) # all depend files (*.d)


TARGETS = $(foreach t,$(TARGET_NAMES),$(BIN_DIR)/$(t))


.PHONY: all objects depend test
all: $(TARGETS) objects test
objects: $(OBJS)
depend: $(DEPS)
test:
	./tests-run-01.sh
clean:
	$(RM) -r $(BUILD_DIR)


# static library to use in all binaries
LIBCP_FILES = util.o hex.o xor.o b64.o
LIBCP_PATHS = $(foreach f,$(LIBCP_FILES),$(OBJ_DIR)/$(f))
$(LIB_DIR)/libcryptopals.a : $(LIBCP_PATHS)
	ar rcs $@ $^

CFLAGS = -I.
LDFLAGS := -L$(LIB_DIR) -lcryptopals

# build targets
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB_DIR)/libcryptopals.a
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

# build objects and dependencies
$(OBJ_DIR)/%.o : %.c
	@$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*.d -o $@ -c $<

# print variable for debugging
print-%: ; @echo $* = '$($*)' from $(origin $*)

-include $(DEPS)
