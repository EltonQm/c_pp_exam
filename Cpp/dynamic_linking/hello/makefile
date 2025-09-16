# === Directories ===
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# === Common Flags ===
CC := clang
CFLAGS := -I$(INC_DIR)
DYLIB_FLAGS := -dynamiclib

# === Targets ===
TARGET_A := $(BUILD_DIR)/helloA
TARGET_B := $(BUILD_DIR)/helloB
LIBA := $(BUILD_DIR)/libA.dylib
LIBB := $(BUILD_DIR)/libB.dylib

# === Source Files ===
MAIN_SRC := $(SRC_DIR)/main.c
SRC_A := $(SRC_DIR)/a.c
SRC_B := $(SRC_DIR)/b.c

# === Default Rule ===
all: $(TARGET_A) $(TARGET_B)

# === Build Shared Libraries ===
$(LIBA): $(SRC_A) | $(BUILD_DIR)
	$(CC) $(DYLIB_FLAGS) $< -o $@ $(CFLAGS)

$(LIBB): $(SRC_B) | $(BUILD_DIR)
	$(CC) $(DYLIB_FLAGS) $< -o $@ $(CFLAGS)

# === Build Executables ===
$(TARGET_A): $(MAIN_SRC) $(LIBA) | $(BUILD_DIR)
	$(CC) $< -o $@ $(CFLAGS) -L$(BUILD_DIR) -lA

$(TARGET_B): $(MAIN_SRC) $(LIBB) | $(BUILD_DIR)
	$(CC) $< -o $@ $(CFLAGS) -L$(BUILD_DIR) -lB

# === Ensure build directory exists ===
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# === Run targets ===
runA: $(TARGET_A)
	DYLD_LIBRARY_PATH=$(BUILD_DIR) ./$(TARGET_A)

runB: $(TARGET_B)
	DYLD_LIBRARY_PATH=$(BUILD_DIR) ./$(TARGET_B)

# === Clean all build files ===
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean runA runB
