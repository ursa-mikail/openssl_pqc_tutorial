# Makefile for SLH-DSA Rust Implementations

# Configuration
CARGO := cargo
LIB_DIR := lib
SRC_DIR := src
BUILD_DIR := target

# Find all SLH-DSA source files in lib directory
SLH_DSA_SOURCES := $(wildcard $(LIB_DIR)/slh_dsa*.rs)
SLH_DSA_TARGETS := $(patsubst $(LIB_DIR)/slh_dsa%.rs,%,$(SLH_DSA_SOURCES))

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
	@echo "Building SLH-DSA project with current main.rs..."
	$(CARGO) build $(CARGO_FLAGS)

# Build specific SLH-DSA version (temporary replacement)
.PHONY: $(SLH_DSA_TARGETS)
$(SLH_DSA_TARGETS): %: $(LIB_DIR)/slh_dsa%.rs backup-main
	@echo "Building SLH-DSA version: $@"
	@cp $(LIB_DIR)/slh_dsa$@.rs $(MAIN_RS)
	$(CARGO) build $(CARGO_FLAGS)
	@echo "Built $@ successfully"

# Run specific SLH-DSA version with timer
.PHONY: run-%
run-%: %
	@echo "========================================"; \
	echo "Running SLH-DSA version: $*"; \
	echo "========================================"; \
	cp $(LIB_DIR)/slh_dsa$*.rs $(MAIN_RS); \
	start_time=$$(date +%s); \
	$(CARGO) run $(CARGO_FLAGS) --; \
	end_time=$$(date +%s); \
	elapsed=$$((end_time - start_time)); \
	echo ""; \
	echo "⏱️  Execution time: $${elapsed} seconds"; \
	echo ""

# Run with custom message and timer
.PHONY: run-%-message
run-%-message: %
	@echo "Running SLH-DSA version $* with custom message..."; \
	cp $(LIB_DIR)/slh_dsa$*.rs $(MAIN_RS); \
	start_time=$$(date +%s); \
	$(CARGO) run $(CARGO_FLAGS) -- "Custom message for SLH-DSA $*"; \
	end_time=$$(date +%s); \
	elapsed=$$((end_time - start_time)); \
	echo "⏱️  Execution time: $${elapsed} seconds"

# Build and run all versions with individual timers
.PHONY: run-all
run-all: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🚀 Starting SLH-DSA test suite..."; \
	echo "⚠️  WARNING: 's' variants may take 10-60+ seconds each!"; \
	echo ""; \
	for version in $(SLH_DSA_TARGETS); do \
		echo "========================================"; \
		echo "Running SLH-DSA version: $$version"; \
		echo "========================================"; \
		cp $(LIB_DIR)/slh_dsa$$version.rs $(MAIN_RS); \
		start_time=$$(date +%s); \
		$(CARGO) run $(CARGO_FLAGS) -- || true; \
		end_time=$$(date +%s); \
		elapsed=$$((end_time - start_time)); \
		echo "⏱️  Time for $$version: $${elapsed} seconds"; \
		echo ""; \
	done; \
	total_end=$$(date +%s); \
	total_elapsed=$$((total_end - total_start)); \
	minutes=$$((total_elapsed / 60)); \
	seconds=$$((total_elapsed % 60)); \
	echo "========================================"; \
	echo "✅ All tests completed!"; \
	echo "⏱️  Total execution time: $${minutes}m $${seconds}s ($${total_elapsed} seconds)"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Run only fast variants (SHA2 and SHAKE with 'f' suffix)
.PHONY: run-fast
run-fast: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🚀 Running FAST variants only (*f)..."; \
	echo ""; \
	for version in $(SLH_DSA_TARGETS); do \
		case $$version in \
			*f) \
				echo "========================================"; \
				echo "Running SLH-DSA version: $$version"; \
				echo "========================================"; \
				cp $(LIB_DIR)/slh_dsa$$version.rs $(MAIN_RS); \
				start_time=$$(date +%s); \
				$(CARGO) run $(CARGO_FLAGS) -- || true; \
				end_time=$$(date +%s); \
				elapsed=$$((end_time - start_time)); \
				echo "⏱️  Time for $$version: $${elapsed} seconds"; \
				echo ""; \
				;; \
		esac; \
	done; \
	total_end=$$(date +%s); \
	total_elapsed=$$((total_end - total_start)); \
	echo "========================================"; \
	echo "✅ Fast variants completed!"; \
	echo "⏱️  Total execution time: $${total_elapsed} seconds"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Run only small variants (SHA2 and SHAKE with 's' suffix) - WARNING: SLOW!
.PHONY: run-small
run-small: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🐌 Running SMALL variants only (*s)..."; \
	echo "⚠️  WARNING: These will take 10-60+ seconds EACH!"; \
	echo "⚠️  Consider using: make BUILD_MODE=release run-small"; \
	echo ""; \
	for version in $(SLH_DSA_TARGETS); do \
		case $$version in \
			*s) \
				echo "========================================"; \
				echo "Running SLH-DSA version: $$version"; \
				echo "========================================"; \
				cp $(LIB_DIR)/slh_dsa$$version.rs $(MAIN_RS); \
				start_time=$$(date +%s); \
				$(CARGO) run $(CARGO_FLAGS) -- || true; \
				end_time=$$(date +%s); \
				elapsed=$$((end_time - start_time)); \
				echo "⏱️  Time for $$version: $${elapsed} seconds"; \
				echo ""; \
				;; \
		esac; \
	done; \
	total_end=$$(date +%s); \
	total_elapsed=$$((total_end - total_start)); \
	minutes=$$((total_elapsed / 60)); \
	seconds=$$((total_elapsed % 60)); \
	echo "========================================"; \
	echo "✅ Small variants completed!"; \
	echo "⏱️  Total execution time: $${minutes}m $${seconds}s ($${total_elapsed} seconds)"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Run only SHA2 variants
.PHONY: run-sha2
run-sha2: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🚀 Running SHA2 variants only..."; \
	echo ""; \
	for version in $(SLH_DSA_TARGETS); do \
		case $$version in \
			_sha2*) \
				echo "========================================"; \
				echo "Running SLH-DSA version: $$version"; \
				echo "========================================"; \
				cp $(LIB_DIR)/slh_dsa$$version.rs $(MAIN_RS); \
				start_time=$$(date +%s); \
				$(CARGO) run $(CARGO_FLAGS) -- || true; \
				end_time=$$(date +%s); \
				elapsed=$$((end_time - start_time)); \
				echo "⏱️  Time for $$version: $${elapsed} seconds"; \
				echo ""; \
				;; \
		esac; \
	done; \
	total_end=$$(date +%s); \
	total_elapsed=$$((total_end - total_start)); \
	minutes=$$((total_elapsed / 60)); \
	seconds=$$((total_elapsed % 60)); \
	echo "========================================"; \
	echo "✅ SHA2 variants completed!"; \
	echo "⏱️  Total execution time: $${minutes}m $${seconds}s ($${total_elapsed} seconds)"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Run only SHAKE variants
.PHONY: run-shake
run-shake: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🚀 Running SHAKE variants only..."; \
	echo ""; \
	for version in $(SLH_DSA_TARGETS); do \
		case $$version in \
			_shake*) \
				echo "========================================"; \
				echo "Running SLH-DSA version: $$version"; \
				echo "========================================"; \
				cp $(LIB_DIR)/slh_dsa$$version.rs $(MAIN_RS); \
				start_time=$$(date +%s); \
				$(CARGO) run $(CARGO_FLAGS) -- || true; \
				end_time=$$(date +%s); \
				elapsed=$$((end_time - start_time)); \
				echo "⏱️  Time for $$version: $${elapsed} seconds"; \
				echo ""; \
				;; \
		esac; \
	done; \
	total_end=$$(date +%s); \
	total_elapsed=$$((total_end - total_start)); \
	minutes=$$((total_elapsed / 60)); \
	seconds=$$((total_elapsed % 60)); \
	echo "========================================"; \
	echo "✅ SHAKE variants completed!"; \
	echo "⏱️  Total execution time: $${minutes}m $${seconds}s ($${total_elapsed} seconds)"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Show available SLH-DSA versions
.PHONY: list
list:
	@echo "Available SLH-DSA versions:"
	@echo ""
	@echo "SHA2 variants:"
	@for version in $(SLH_DSA_TARGETS); do \
		case $$version in \
			_sha2*f) echo "  - $$version (SHA2, FAST)";; \
			_sha2*s) echo "  - $$version (SHA2, SMALL - SLOW!)";; \
		esac; \
	done
	@echo ""
	@echo "SHAKE variants:"
	@for version in $(SLH_DSA_TARGETS); do \
		case $$version in \
			_shake*f) echo "  - $$version (SHAKE, FAST)";; \
			_shake*s) echo "  - $$version (SHAKE, SMALL - SLOW!)";; \
		esac; \
	done
	@echo ""
	@echo "Files found in $(LIB_DIR)/:"
	@ls -la $(LIB_DIR)/slh_dsa*.rs 2>/dev/null || echo "No slh_dsa*.rs files found in $(LIB_DIR)/"

# Clean build artifacts and restore main.rs
.PHONY: clean
clean: restore-main
	@echo "Cleaning build artifacts..."
	$(CARGO) clean

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

# Initialize project
.PHONY: init
init:
	@echo "Initializing project structure..."
	@mkdir -p $(LIB_DIR) $(SRC_DIR) $(BUILD_DIR)
	@if [ ! -f $(CARGO_TOML) ]; then \
		echo '[package]\nname = "slh_dsa"\nversion = "0.1.0"\nedition = "2021"\n\n[dependencies]\nhex = "0.4.3"\nslh-dsa = "0.1.0"\nrand = "0.8.5"\nsignature = "2.2"' > $(CARGO_TOML); \
	fi
	@if [ ! -f $(SRC_DIR)/main.rs ]; then \
		echo '// SLH-DSA Main Entry Point\nfn main() {\n    println!("SLH-DSA Signature Implementations");\n    println!("Use: make run-<version> to test different implementations");\n}' > $(SRC_DIR)/main.rs; \
	fi

# Show current status
.PHONY: status
status:
	@echo "Current project status:"
	@echo "Available SLH-DSA versions: $(SLH_DSA_TARGETS)"
	@if [ -f $(MAIN_RS).orig ]; then \
		echo "Main.rs status: TEMPORARY (original backed up)"; \
	else \
		echo "Main.rs status: ORIGINAL"; \
	fi

# Reset to original state
.PHONY: reset
reset: restore-main
	@echo "Project reset to original state"

# Benchmark all variants
.PHONY: benchmark
benchmark: backup-main
	@echo "========================================"; \
	echo "📊 SLH-DSA Performance Benchmark"; \
	echo "========================================"; \
	echo ""; \
	echo "Variant        | Time (seconds) | Hash | Speed"; \
	echo "---------------|----------------|------|-------"; \
	total_start=$$(date +%s); \
	for version in $(SLH_DSA_TARGETS); do \
		cp $(LIB_DIR)/slh_dsa$$version.rs $(MAIN_RS); \
		start_time=$$(date +%s); \
		$(CARGO) run $(CARGO_FLAGS) -- > /dev/null 2>&1 || true; \
		end_time=$$(date +%s); \
		elapsed=$$((end_time - start_time)); \
		printf "%-14s | %14d | " "$$version" "$$elapsed"; \
		case $$version in \
			_sha2*) printf "SHA2 | ";; \
			_shake*) printf "SHAKE| ";; \
		esac; \
		if [ $$elapsed -lt 5 ]; then \
			echo "⚡ FAST"; \
		elif [ $$elapsed -lt 30 ]; then \
			echo "🐌 SLOW"; \
		else \
			echo "🐢 VERY SLOW"; \
		fi; \
	done; \
	total_end=$$(date +%s); \
	total_elapsed=$$((total_end - total_start)); \
	minutes=$$((total_elapsed / 60)); \
	seconds=$$((total_elapsed % 60)); \
	echo "---------------|----------------|------|-------"; \
	printf "TOTAL          | %14d | ---  | $${minutes}m $${seconds}s\n" "$$total_elapsed"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Help target
.PHONY: help
help:
	@echo "SLH-DSA Makefile Targets:"
	@echo ""
	@echo "Building:"
	@echo "  all              - Build with current main.rs"
	@echo "  build            - Build current version"
	@echo ""
	@echo "Running (with timers):"
	@echo "  run-<version>    - Run specific variant with timer"
	@echo "  run-<version>-message - Run with custom message and timer"
	@echo "  run-all          - Run ALL variants ⚠️  SLOW!"
	@echo "  run-fast         - Run ONLY fast (f) variants ⚡"
	@echo "  run-small        - Run ONLY small (s) variants 🐌 VERY SLOW!"
	@echo "  run-sha2         - Run ONLY SHA2 variants"
	@echo "  run-shake        - Run ONLY SHAKE variants"
	@echo "" 
	@echo "Analysis:"
	@echo "  benchmark        - Benchmark all variants (quiet mode)"
	@echo "  list             - List available SLH-DSA versions"
	@echo "  status           - Show current project status"
	@echo ""
	@echo "Maintenance:"
	@echo "  clean            - Clean build artifacts and restore main.rs"
	@echo "  check            - Check code"
	@echo "  test             - Run tests"
	@echo "  fmt              - Format code"
	@echo "  init             - Initialize project"
	@echo "  reset            - Reset to original state"
	@echo ""
	@echo "Build Modes:"
	@echo "  make BUILD_MODE=release run-all    - Run with optimizations"
	@echo "  make BUILD_MODE=release run-small  - Run slow variants (optimized)"
	@echo ""
	@echo "Examples:"
	@echo "  make run-_sha2_128f                 - Quick SHA2 test"
	@echo "  make run-_shake128f                 - Quick SHAKE test"
	@echo "  make run-fast                       - Test all fast variants"
	@echo "  make run-sha2                       - Test all SHA2 variants"
	@echo "  make BUILD_MODE=release run-small   - Test slow variants (optimized)"
	@echo "  make benchmark                      - Compare all variants"

.DEFAULT_GOAL := help