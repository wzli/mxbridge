BUILD_DIR ?= ./build
SRC_DIR ?= ./src
INC_DIR ?= ./include

SRCS := $(shell find -L $(SRC_DIR) -name "*.cpp" -or -name "*.c")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

$(BUILD_DIR)/test: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

INC_DIRS := $(shell find -L $(INC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

COMMONFLAGS =  -Wall -Wextra -Wpedantic -Ofast -DNDEBUG #-Og -g -DDEBUG

CFLAGS ?= -std=c99 $(COMMONFLAGS)
CC ?= gcc

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP 
LDLIBS += -lm

-include $(DEPS)
MKDIR_P ?= mkdir -p

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

