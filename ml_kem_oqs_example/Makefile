# macOS Makefile for ML-KEM - WITH OPENSSL LINKING
CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

# Correct paths based on your directory structure
OQS_INCLUDE = -I../include
OQS_LIB = -L../lib

# Find OpenSSL on macOS
OPENSSL_PATHS = /usr/local/opt/openssl /opt/homebrew/opt/openssl
OPENSSL_INCLUDE = $(shell for path in $(OPENSSL_PATHS); do if [ -d "$$path/include" ]; then echo "-I$$path/include"; break; fi; done)
OPENSSL_LIB = $(shell for path in $(OPENSSL_PATHS); do if [ -d "$$path/lib" ]; then echo "-L$$path/lib"; break; fi; done)

# Libraries - OpenSSL MUST come after liboqs
LIBS = -loqs -lcrypto

# Combine all paths
INCLUDE = $(OQS_INCLUDE) $(OPENSSL_INCLUDE)
LIB = $(OQS_LIB) $(OPENSSL_LIB)

# Targets
TARGET = mlkem_example
SOURCES = mlkem_example.c
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

# Test with different algorithms
test: $(TARGET)
	@echo "🧪 Testing ML-KEM Algorithms"
	@echo "============================"
	@for algo in "Kyber512" "Kyber768" "Kyber1024"; do \
		echo "Testing $$algo..."; \
		./$(TARGET) "$$algo" 2>/dev/null && echo "✅ $$algo: SUCCESS" || echo "❌ $$algo: FAILED"; \
	done

# Clean build files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Check dependencies
check-deps:
	@echo "🔍 Checking dependencies..."
	@# Check liboqs
	@if [ -f "../lib/liboqs.a" ]; then \
		echo "✅ liboqs found: ../lib/liboqs.a"; \
	else \
		echo "❌ liboqs not found: ../lib/liboqs.a"; \
		exit 1; \
	fi
	@# Check OpenSSL
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
	@echo "✅ OpenSSL installed"

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
	@echo "Target: $(TARGET)"
	@echo "OpenSSL include: $(OPENSSL_INCLUDE)"
	@echo "OpenSSL lib: $(OPENSSL_LIB)"

# Run with specific algorithm
run-kyber512: $(TARGET)
	./$(TARGET) Kyber512

run-kyber768: $(TARGET)
	./$(TARGET) Kyber768

run-kyber1024: $(TARGET)
	./$(TARGET) Kyber1024

# Help target
help:
	@echo "Available targets:"
	@echo "  all            - Build the program (default)"
	@echo "  run            - Build and run with default parameters"
	@echo "  test           - Test all ML-KEM variants"
	@echo "  clean          - Remove build files"
	@echo "  check-deps     - Check if all dependencies are available"
	@echo "  install-openssl - Install OpenSSL via Homebrew"
	@echo "  debug          - Build with debug flags"
	@echo "  release        - Build with optimization flags"
	@echo "  info           - Show build information"
	@echo "  run-kyber512   - Build and run with Kyber512"
	@echo "  run-kyber768   - Build and run with Kyber768"
	@echo "  run-kyber1024  - Build and run with Kyber1024"

.PHONY: all run test clean check-deps install-openssl debug release info help run-kyber512 run-kyber768 run-kyber1024
	