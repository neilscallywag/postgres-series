# Location of build directory
BUILD_DIR := build

# Default target: configure, build, and run tests
all: configure build test

# Step 1: Run CMake configuration
configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

# Step 2: Build using make inside build/
build:
	@cd $(BUILD_DIR) && $(MAKE)

# Step 3: Run tests using ctest
test:
	@cd $(BUILD_DIR) && ctest --output-on-failure

# Remove build directory
clean:
	@rm -rf $(BUILD_DIR)

# Clean + build fresh
rebuild: clean all

docs:
	@if ! command -v doxygen >/dev/null 2>&1; then \
		echo "Error: doxygen not found. Install with 'brew install doxygen'."; \
		exit 1; \
	fi
	doxygen Doxyfile

.PHONY: all configure build test clean rebuild
