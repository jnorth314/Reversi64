ROMNAME  = Reversi64
ROMTITLE = "Reversi 64"

BUILD_DIR = build

SRC = $(shell find src/ -type f -name "*.c")

include $(N64_INST)/include/n64.mk

all: $(ROMNAME).z64

$(BUILD_DIR)/$(ROMNAME).elf: $(SRC:%.c=$(BUILD_DIR)/%.o)

$(ROMNAME).z64: N64_ROM_TITLE=$(ROMTITLE)

clean:
	rm -rf $(BUILD_DIR) $(ROMNAME).z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
