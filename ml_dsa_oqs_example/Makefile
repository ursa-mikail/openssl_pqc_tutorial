# Makefile for ML-DSA with OpenSSL linking
CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

# liboqs paths
OQS_INCLUDE = -I../include
OQS_LIB = -L../lib

# Find OpenSSL on macOS
OPENSSL_PATHS = /usr/local/opt/openssl /opt/homebrew/opt/openssl
OPENSSL_INCLUDE = $(shell for path in $(OPENSSL_PATHS); do if [ -d "$$path/include" ]; then echo "-I$$path/include"; break; fi; done)
OPENSSL_LIB = $(shell for path in $(OPENSSL_PATHS); do if [ -d "$$path/lib" ]; then echo "-L$$path/lib"; break; fi; done)

# Libraries
LIBS = -loqs -lcrypto

# Combine all paths
INCLUDE = $(OQS_INCLUDE) $(OPENSSL_INCLUDE)
LIB = $(OQS_LIB) $(OPENSSL_LIB)

# Targets
TARGET = mldsa_example
SOURCES = mldsa_example.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: check-deps $(TARGET)

# Main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIB) $(LIBS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Test different ML-DSA variants
test: $(TARGET)
	@echo "🧪 Testing ML-DSA Algorithms"
	@echo "============================"
	@for algo in "Dilithium2" "Dilithium3" "Dilithium5"; do \
		echo "Testing $$algo..."; \
		./$(TARGET) "$$algo" 2>/dev/null && echo "✅ $$algo: SUCCESS" || echo "❌ $$algo: FAILED"; \
	done

# Clean build files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Check dependencies
check-deps:
	@echo "🔍 Checking dependencies..."
	@if [ -f "../lib/liboqs.a" ]; then \
		echo "✅ liboqs found: ../lib/liboqs.a"; \
	else \
		echo "❌ liboqs not found: ../lib/liboqs.a"; \
		exit 1; \
	fi
	@if [ -n "$(OPENSSL_LIB)" ]; then \
		echo "✅ OpenSSL found: $(OPENSSL_LIB)"; \
	else \
		echo "❌ OpenSSL not found in standard locations"; \
		echo "💡 Install with: brew install openssl"; \
		exit 1; \
	fi

# Install OpenSSL if missing
install-openssl:
	@echo "📦 Installing OpenSSL via Homebrew..."
	brew install openssl

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build
release: CFLAGS += -O2 -DNDEBUG
release: $(TARGET)

# Show build information
info:
	@echo "📋 Build Information"
	@echo "==================="
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "Include: $(INCLUDE)"
	@echo "Library: $(LIB)"
	@echo "Libraries: $(LIBS)"

# Run with specific algorithm
run-dilithium2: $(TARGET)
	./$(TARGET) Dilithium2

run-dilithium3: $(TARGET)
	./$(TARGET) Dilithium3

run-dilithium5: $(TARGET)
	./$(TARGET) Dilithium5

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Build the program (default)"
	@echo "  run              - Build and run with default parameters"
	@echo "  test             - Test all ML-DSA variants"
	@echo "  clean            - Remove build files"
	@echo "  check-deps       - Check if all dependencies are available"
	@echo "  install-openssl  - Install OpenSSL via Homebrew"
	@echo "  debug            - Build with debug flags"
	@echo "  release          - Build with optimization flags"
	@echo "  info             - Show build information"
	@echo "  run-dilithium2   - Build and run with Dilithium2"
	@echo "  run-dilithium3   - Build and run with Dilithium3"
	@echo "  run-dilithium5   - Build and run with Dilithium5"

.PHONY: all run test clean check-deps install-openssl debug release info help run-dilithium2 run-dilithium3 run-dilithium5