# Makefile for ML-DSA Rust Implementations

# Configuration
CARGO := cargo
LIB_DIR := lib
SRC_DIR := src
BUILD_DIR := target
TEMP_MAIN := $(SRC_DIR)/main_temp.rs

# Find all ML-DSA source files in lib directory
ML_DSA_SOURCES := $(wildcard $(LIB_DIR)/ml_dsa*.rs)
ML_DSA_TARGETS := $(patsubst $(LIB_DIR)/ml_dsa%.rs,%,$(ML_DSA_SOURCES))

# Default Rust files
MAIN_RS := $(SRC_DIR)/main.rs
CARGO_TOML := Cargo.toml

# Build mode
BUILD_MODE ?= debug
ifeq ($(BUILD_MODE),release)
	CARGO_FLAGS := --release
	BUILD_PATH := $(BUILD_DIR)/release
else
	CARGO_FLAGS :=
	BUILD_PATH := $(BUILD_DIR)/debug
endif

.PHONY: all
all: build

# Backup original main.rs
.PHONY: backup-main
backup-main:
	@if [ -f $(MAIN_RS) ] && [ ! -f $(MAIN_RS).orig ]; then \
		cp $(MAIN_RS) $(MAIN_RS).orig; \
		echo "Backed up original main.rs to main.rs.orig"; \
	fi

# Restore original main.rs
.PHONY: restore-main
restore-main:
	@if [ -f $(MAIN_RS).orig ]; then \
		cp $(MAIN_RS).orig $(MAIN_RS); \
		echo "Restored original main.rs"; \
		rm -f $(MAIN_RS).orig; \
	else \
		echo "No original main.rs backup found"; \
	fi

# Main build target using Cargo (uses current main.rs)
.PHONY: build
build: $(CARGO_TOML) $(MAIN_RS)
	@echo "Building ML-DSA project with current main.rs..."
	$(CARGO) build $(CARGO_FLAGS)

# Build specific ML-DSA version (temporary replacement)
.PHONY: $(ML_DSA_TARGETS)
$(ML_DSA_TARGETS): %: $(LIB_DIR)/ml_dsa%.rs backup-main
	@echo "Building ML-DSA version: $@"
	@cp $(LIB_DIR)/ml_dsa$@.rs $(MAIN_RS)
	$(CARGO) build $(CARGO_FLAGS)
	@echo "Built $@ successfully"

# Run specific ML-DSA version (temporary replacement)
.PHONY: run-%
run-%: %
	@echo "Running ML-DSA version: $*"
	@cp $(LIB_DIR)/ml_dsa$*.rs $(MAIN_RS)
	$(CARGO) run $(CARGO_FLAGS) --

# Run with custom message
.PHONY: run-%-message
run-%-message: %
	@echo "Running ML-DSA version $* with custom message..."
	@cp $(LIB_DIR)/ml_dsa$*.rs $(MAIN_RS)
	$(CARGO) run $(CARGO_FLAGS) -- "Custom message for ML-DSA $*"

# Build and run all versions (restores main.rs at the end)
.PHONY: run-all
run-all: backup-main
	@for version in $(ML_DSA_TARGETS); do \
		echo "========================================"; \
		echo "Running ML-DSA version: $$version"; \
		echo "========================================"; \
		cp $(LIB_DIR)/ml_dsa$$version.rs $(MAIN_RS); \
		$(CARGO) run $(CARGO_FLAGS) -- || true; \
		echo ""; \
	done
	@$(MAKE) restore-main

# Show available ML-DSA versions
.PHONY: list
list:
	@echo "Available ML-DSA versions:"
	@for version in $(ML_DSA_TARGETS); do \
		echo "  - $$version"; \
	done
	@echo ""
	@echo "Files found in $(LIB_DIR)/:"
	@ls -la $(LIB_DIR)/ml_dsa*.rs 2>/dev/null || echo "No ml_dsa*.rs files found in $(LIB_DIR)/"

# Clean build artifacts and restore main.rs
.PHONY: clean
clean: restore-main
	@echo "Cleaning build artifacts..."
	$(CARGO) clean
	@rm -f $(TEMP_MAIN)

# Check code
.PHONY: check
check:
	@echo "Checking code..."
	$(CARGO) check

# Run tests
.PHONY: test
test:
	@echo "Running tests..."
	$(CARGO) test

# Format code
.PHONY: fmt
fmt:
	@echo "Formatting code..."
	$(CARGO) fmt

# Show current status
.PHONY: status
status:
	@echo "Current project status:"
	@echo "Available ML-DSA versions: $(ML_DSA_TARGETS)"
	@if [ -f $(MAIN_RS).orig ]; then \
		echo "Main.rs status: TEMPORARY (original backed up)"; \
	else \
		echo "Main.rs status: ORIGINAL"; \
	fi
	@echo "Current main.rs content:"
	@head -5 $(MAIN_RS) 2>/dev/null || echo "main.rs not found"

# Reset to original state
.PHONY: reset
reset: restore-main
	@echo "Project reset to original state"

# Help target
.PHONY: help
help:
	@echo "ML-DSA Makefile Targets:"
	@echo "  all              - Build with current main.rs"
	@echo "  build            - Build current version"
	@echo "  ml_dsa<X>        - Build specific ML-DSA version (e.g., make ml_dsa44)"
	@echo "  run-<version>    - Run specific ML-DSA version (e.g., make run-44)"
	@echo "  run-<version>-message - Run with custom message"
	@echo "  run-all          - Build and run all versions (restores main.rs)"
	@echo "  list             - List available ML-DSA versions"
	@echo "  clean            - Clean build artifacts and restore main.rs"
	@echo "  check            - Check code"
	@echo "  test             - Run tests"
	@echo "  fmt              - Format code"
	@echo "  status           - Show current project status"
	@echo "  backup-main      - Backup original main.rs"
	@echo "  restore-main     - Restore original main.rs"
	@echo "  reset            - Reset to original state"
	@echo "  help             - Show this help"

.DEFAULT_GOAL := help