# ============================================
# Project Build Configuration
# ============================================

BUILD_DIR := build
SRC_DIR := src
FIND_SOURCES := $(shell find $(SRC_DIR) -name '*.cpp' -o -name '*.h')

# Default: format → lint → sast → configure → build → test
all: format lint sast configure build test

# ============================================
# Build Targets
# ============================================

configure:
	@echo "\033[1;34m[CONFIGURE]\033[0m Running CMake..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

build:
	@echo "\033[1;34m[BUILD]\033[0m Building project..."
	@cd $(BUILD_DIR) && $(MAKE)

test:
	@echo "\033[1;34m[TEST]\033[0m Running tests..."
	@cd $(BUILD_DIR) && ctest --output-on-failure

clean:
	@echo "\033[1;33m[CLEAN]\033[0m Removing build directory..."
	@rm -rf $(BUILD_DIR)

rebuild: clean all

# ============================================
# Documentation
# ============================================

docs:
	@if ! command -v doxygen >/dev/null 2>&1; then \
		echo "\033[1;31mError:\033[0m doxygen not found. Install with 'brew install doxygen'."; \
		exit 1; \
	fi
	@echo "\033[1;34m[DOCS]\033[0m Generating documentation..."
	@doxygen Doxyfile
	@echo "\033[1;32mDocumentation generated in docs/html/\033[0m"

# ============================================
# Formatting / Linting / SAST
# ============================================

format:
	@echo "\033[1;34m[FORMAT]\033[0m Running clang-format..."
	@clang-format -i $(FIND_SOURCES)

lint:
	@if [ ! -f build/compile_commands.json ]; then \
	    echo "\033[1;31mError:\033[0m compile_commands.json missing. Run 'make configure' first."; \
	    exit 1; \
	fi
	@if ! command -v clang-tidy >/dev/null 2>&1; then \
		echo "\033[1;31mError:\033[0m clang-tidy not found. Install llvm via brew."; \
		exit 1; \
	fi
	@echo "\033[1;34m[LINT]\033[0m Running clang-tidy with auto-fixes..."
	@clang-tidy -p build $(shell find src -name '*.cpp') --fix --fix-errors

# Optional manual cppcheck target
cppcheck:
	@echo "\033[1;34m[CPPCHECK]\033[0m Running cppcheck..."
	@cppcheck --enable=all --inconclusive --std=c++17 -I src $(SRC_DIR)

# Clean cppcheck (quiet, no missing includes)
sast:
	@echo "\033[1;34m[SAST]\033[0m Running cppcheck (quiet)..."
	@cppcheck \
	    --enable=all \
	    --inconclusive \
	    --std=c++17 \
	    -I src \
	    --suppress=missingIncludeSystem \
	    --quiet src

# ============================================
# clang Static Analyzer (deep SAST)
# ============================================

analyze:
	@if ! command -v scan-build >/dev/null 2>&1; then \
		echo "\033[1;31mError:\033[0m scan-build not found. Install llvm via brew."; \
		exit 1; \
	fi
	@echo "\033[1;34m[ANALYZE]\033[0m Running Clang Static Analyzer..."
	@scan-build -o analysis --status-bugs $(MAKE) build

# ============================================
# Sanitizers
# ============================================

sanitize:
	@echo "\033[1;34m[SANITIZE]\033[0m Rebuilding with sanitizers enabled..."
	@rm -rf build
	@mkdir build
	@cd build && cmake -DSANITIZE=ON .. && make

.PHONY: all configure build test clean rebuild \
        docs format lint cppcheck sast analyze sanitize
