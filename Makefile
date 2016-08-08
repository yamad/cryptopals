TARGET_NAMES := cc-01 cc-02 cc-03 cc-04 cc-05 cc-06

SRC_DIR     := src
INC_DIR     := include

BUILD_DIR   := build
BIN_DIR		:= $(BUILD_DIR)/bin
DEP_DIR     := $(BUILD_DIR)/depends
OBJ_DIR     := $(BUILD_DIR)/objs
LIB_DIR		:= $(BUILD_DIR)/libs
RESULT_DIR  := $(BUILD_DIR)/results
BUILD_DIRS  := $(BUILD_DIR) $(BIN_DIR) $(DEP_DIR) $(OBJ_DIR) $(LIB_DIR) $(RESULT_DIR)

DIRS := $(SRC_DIR) $(INC_DIR) $(BUILD_DIRS)

# make directories
makedir = $(shell mkdir -p $1)
$(foreach dir,$(DIRS),$(call makedir,$(dir)))

SRCS := $(wildcard $(SRC_DIR)/*.c)			              # all source files (*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS)) # all object files (*.o)
DEPS := $(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SRCS)) # all depend files (*.d)

TARGETS = $(foreach t,$(TARGET_NAMES),$(BIN_DIR)/$(t))


CFLAGS  := -I$(INC_DIR)
LDFLAGS := -L$(LIB_DIR) -lcryptopals -lm

.PHONY: all objects depend test
all: $(TARGETS) objects test
objects: $(OBJS)
depend: $(DEPS)
test:
	rake test:all
	./tests-run-01.sh
clean:
	$(RM) -r $(BUILD_DIR)

debug: CFLAGS += -DDEBUG -g
debug: CXXFLAGS += -DDEBUG -g
debug: all

# static library to use in all binaries
LIBCP_FILES = util.o hex.o xor.o b64.o
LIBCP_PATHS = $(foreach f,$(LIBCP_FILES),$(OBJ_DIR)/$(f))
$(LIB_DIR)/libcryptopals.a : $(LIBCP_PATHS)
	ar rcs $@ $^

# build targets
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB_DIR)/libcryptopals.a
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

# build objects and dependencies
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*.d -o $@ -c $<

# print variable for debugging
print-%: ; @echo $* = '$($*)' from $(origin $*)

-include $(DEPS)
