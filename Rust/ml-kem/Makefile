# Makefile for ML-KEM Rust Implementations

# Configuration
CARGO := cargo
LIB_DIR := lib
SRC_DIR := src
BUILD_DIR := target

# Find all ML-KEM source files in lib directory
ML_KEM_SOURCES := $(wildcard $(LIB_DIR)/ml_kem*.rs)
ML_KEM_TARGETS := $(patsubst $(LIB_DIR)/ml_kem%.rs,%,$(ML_KEM_SOURCES))

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
	@echo "Building ML-KEM project with current main.rs..."
	$(CARGO) build $(CARGO_FLAGS)

# Build specific ML-KEM version (temporary replacement)
.PHONY: $(ML_KEM_TARGETS)
$(ML_KEM_TARGETS): %: $(LIB_DIR)/ml_kem%.rs backup-main
	@echo "Building ML-KEM version: $@"
	@cp $(LIB_DIR)/ml_kem$@.rs $(MAIN_RS)
	$(CARGO) build $(CARGO_FLAGS)
	@echo "Built $@ successfully"

# Run specific ML-KEM version with timer (temporary replacement)
.PHONY: run-%
run-%: %
	@echo "========================================"; \
	echo "Running ML-KEM version: $*"; \
	echo "========================================"; \
	cp $(LIB_DIR)/ml_kem$*.rs $(MAIN_RS); \
	start_time=$$(date +%s); \
	$(CARGO) run $(CARGO_FLAGS) --; \
	end_time=$$(date +%s); \
	elapsed=$$((end_time - start_time)); \
	echo ""; \
	echo "⏱️  Execution time: $${elapsed} seconds"; \
	echo ""

# Build and run all versions with individual timers (restores main.rs at the end)
.PHONY: run-all
run-all: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🚀 Starting ML-KEM test suite..."; \
	echo ""; \
	for version in $(ML_KEM_TARGETS); do \
		echo "========================================"; \
		echo "Running ML-KEM version: $$version"; \
		echo "========================================"; \
		cp $(LIB_DIR)/ml_kem$$version.rs $(MAIN_RS); \
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

# Run only 512-bit variant with timer
.PHONY: run-512
run-512: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🚀 Running ML-KEM-512 variant..."; \
	echo ""; \
	for version in $(ML_KEM_TARGETS); do \
		case $$version in \
			*512*) \
				echo "========================================"; \
				echo "Running ML-KEM version: $$version"; \
				echo "========================================"; \
				cp $(LIB_DIR)/ml_kem$$version.rs $(MAIN_RS); \
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
	echo "✅ ML-KEM-512 completed!"; \
	echo "⏱️  Total execution time: $${total_elapsed} seconds"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Run only 768-bit variant with timer
.PHONY: run-768
run-768: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🚀 Running ML-KEM-768 variant..."; \
	echo ""; \
	for version in $(ML_KEM_TARGETS); do \
		case $$version in \
			*768*) \
				echo "========================================"; \
				echo "Running ML-KEM version: $$version"; \
				echo "========================================"; \
				cp $(LIB_DIR)/ml_kem$$version.rs $(MAIN_RS); \
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
	echo "✅ ML-KEM-768 completed!"; \
	echo "⏱️  Total execution time: $${total_elapsed} seconds"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Run only 1024-bit variant with timer
.PHONY: run-1024
run-1024: backup-main
	@total_start=$$(date +%s); \
	echo ""; \
	echo "🚀 Running ML-KEM-1024 variant..."; \
	echo ""; \
	for version in $(ML_KEM_TARGETS); do \
		case $$version in \
			*1024*) \
				echo "========================================"; \
				echo "Running ML-KEM version: $$version"; \
				echo "========================================"; \
				cp $(LIB_DIR)/ml_kem$$version.rs $(MAIN_RS); \
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
	echo "✅ ML-KEM-1024 completed!"; \
	echo "⏱️  Total execution time: $${total_elapsed} seconds"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Show available ML-KEM versions
.PHONY: list
list:
	@echo "Available ML-KEM versions:"
	@for version in $(ML_KEM_TARGETS); do \
		case $$version in \
			*512*) echo "  - $$version (Level 1 - Fastest)";; \
			*768*) echo "  - $$version (Level 3 - Balanced)";; \
			*1024*) echo "  - $$version (Level 5 - Most Secure)";; \
			*) echo "  - $$version";; \
		esac; \
	done
	@echo ""
	@echo "Files found in $(LIB_DIR)/:"
	@ls -la $(LIB_DIR)/ml_kem*.rs 2>/dev/null || echo "No ml_kem*.rs files found in $(LIB_DIR)/"

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

# Initialize project with ML-KEM dependencies
.PHONY: init
init:
	@echo "Initializing project structure..."
	@mkdir -p $(LIB_DIR) $(SRC_DIR) $(BUILD_DIR)
	@if [ ! -f $(CARGO_TOML) ]; then \
		echo '[package]\nname = "ml_kem"\nversion = "0.1.0"\nedition = "2024"\n\n[dependencies]\nfips203 = "0.1.0"\nhex = "0.4.3"\nrand = "0.8.5"' > $(CARGO_TOML); \
	fi
	@if [ ! -f $(SRC_DIR)/main.rs ]; then \
		echo '// ML-KEM Main Entry Point\nfn main() {\n    println!("ML-KEM Key Encapsulation Implementations");\n    println!("Use: make run-<version> to test different implementations");\n}' > $(SRC_DIR)/main.rs; \
	fi

# Show current status
.PHONY: status
status:
	@echo "Current project status:"
	@echo "Available ML-KEM versions: $(ML_KEM_TARGETS)"
	@if [ -f $(MAIN_RS).orig ]; then \
		echo "Main.rs status: TEMPORARY (original backed up)"; \
	else \
		echo "Main.rs status: ORIGINAL"; \
	fi

# Reset to original state
.PHONY: reset
reset: restore-main
	@echo "Project reset to original state"

# Benchmark all variants (with detailed timing)
.PHONY: benchmark
benchmark: backup-main
	@echo "========================================"; \
	echo "🏁 ML-KEM Performance Benchmark"; \
	echo "========================================"; \
	echo ""; \
	echo "Variant        | Time (seconds) | Security Level"; \
	echo "---------------|----------------|---------------"; \
	total_start=$$(date +%s); \
	for version in $(ML_KEM_TARGETS); do \
		cp $(LIB_DIR)/ml_kem$$version.rs $(MAIN_RS); \
		start_time=$$(date +%s); \
		$(CARGO) run $(CARGO_FLAGS) -- > /dev/null 2>&1 || true; \
		end_time=$$(date +%s); \
		elapsed=$$((end_time - start_time)); \
		printf "%-14s | %14d | " "$$version" "$$elapsed"; \
		case $$version in \
			*512*) echo "Level 1 (Fastest)";; \
			*768*) echo "Level 3 (Balanced)";; \
			*1024*) echo "Level 5 (Secure)";; \
			*) echo "Unknown";; \
		esac; \
	done; \
	total_end=$$(date +%s); \
	total_elapsed=$$((total_end - total_start)); \
	minutes=$$((total_elapsed / 60)); \
	seconds=$$((total_elapsed % 60)); \
	echo "---------------|----------------|---------------"; \
	printf "TOTAL          | %14d | $${minutes}m $${seconds}s\n" "$$total_elapsed"; \
	echo "========================================"; \
	$(MAKE) restore-main

# Compare key sizes across all variants
.PHONY: compare-sizes
compare-sizes: backup-main
	@echo "========================================"; \
	echo "📊 ML-KEM Key and Ciphertext Size Comparison"; \
	echo "========================================"; \
	echo ""; \
	echo "Variant  | PK Size | CT Size | SS Size | Security"; \
	echo "---------|---------|---------|---------|----------"; \
	for version in $(ML_KEM_TARGETS); do \
		cp $(LIB_DIR)/ml_kem$$version.rs $(MAIN_RS); \
		$(CARGO) run $(CARGO_FLAGS) -- 2>/dev/null | \
		awk -v version="$$version" '\
			/Encapsulation key length:/ {pk=$$4} \
			/Ciphertext length:/ {ct=$$4} \
			/Shared secret length:/ {ss=$$4} \
			END { \
				security=""; \
				if (version ~ /512/) security="Level 1"; \
				else if (version ~ /768/) security="Level 3"; \
				else if (version ~ /1024/) security="Level 5"; \
				printf "%-9s | %7s | %7s | %7s | %s\n", version, pk, ct, ss, security \
			}' || true; \
	done; \
	echo "========================================"; \
	$(MAKE) restore-main

# Help target
.PHONY: help
help:
	@echo "ML-KEM Makefile Targets:"
	@echo ""
	@echo "Building:"
	@echo "  all              - Build with current main.rs"
	@echo "  build            - Build current version"
	@echo "  ml_kem<X>        - Build specific ML-KEM version"
	@echo ""
	@echo "Running (with timers):"
	@echo "  run-<version>    - Run specific variant with timer"
	@echo "  run-all          - Run ALL variants with timers"
	@echo "  run-512          - Run ONLY 512-bit variant"
	@echo "  run-768          - Run ONLY 768-bit variant"
	@echo "  run-1024         - Run ONLY 1024-bit variant"
	@echo ""
	@echo "Analysis:"
	@echo "  benchmark        - Benchmark all variants (quiet mode)"
	@echo "  compare-sizes    - Compare key sizes across variants"
	@echo "  list             - List available ML-KEM versions"
	@echo "  status           - Show current project status"
	@echo ""
	@echo "Maintenance:"
	@echo "  clean            - Clean build artifacts and restore main.rs"
	@echo "  check            - Check code"
	@echo "  test             - Run tests"
	@echo "  fmt              - Format code"
	@echo "  init             - Initialize project with ML-KEM dependencies"
	@echo "  reset            - Reset to original state"
	@echo ""
	@echo "Build Modes:"
	@echo "  make BUILD_MODE=release run-all    - Run with optimizations"
	@echo ""
	@echo "Examples:"
	@echo "  make run-_512                      - Test ML-KEM-512"
	@echo "  make run-_768                      - Test ML-KEM-768"
	@echo "  make run-_1024                     - Test ML-KEM-1024"
	@echo "  make run-all                       - Test all variants"
	@echo "  make compare-sizes                 - Compare key sizes"
	@echo "  make benchmark                     - Performance comparison"

.DEFAULT_GOAL := help