# Makefile for SLH-DSA Demo
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
TARGET = slh_dsa_demo
SOURCES = slh_dsa_demo.c
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

# Test different SLH-DSA variants
test: $(TARGET)
	@echo "🧪 Testing SLH-DSA Algorithms"
	@echo "============================"
	@for algo in "SLH-DSA-SHA2-128f" "SLH-DSA-SHA2-128s" "SLH-DSA-SHA2-192f"; do \
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

# Rebuild liboqs with SLH-DSA support
rebuild-liboqs:
	@echo "🏗️  Rebuilding liboqs with SLH-DSA support..."
	cd .. && rm -rf build && mkdir build && cd build && \
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DOQS_ENABLE_SIG_SPHINCS=ON .. && \
	make -j$(sysctl -n hw.ncpu) && \
	echo "✅ liboqs rebuilt with SLH-DSA support!"

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
run-128f: $(TARGET)
	./$(TARGET) SLH-DSA-SHA2-128f

run-128s: $(TARGET)
	./$(TARGET) SLH-DSA-SHA2-128s

run-192f: $(TARGET)
	./$(TARGET) SLH-DSA-SHA2-192f

# Help target
help:
	@echo "Available targets:"
	@echo "  all            - Build the program (default)"
	@echo "  run            - Build and run with default parameters"
	@echo "  test           - Test SLH-DSA variants"
	@echo "  clean          - Remove build files"
	@echo "  check-deps     - Check dependencies"
	@echo "  install-openssl - Install OpenSSL"
	@echo "  rebuild-liboqs - Rebuild liboqs with SLH-DSA support"
	@echo "  debug          - Build with debug flags"
	@echo "  release        - Build with optimization flags"
	@echo "  info           - Show build information"
	@echo "  run-128f       - Run with SLH-DSA-SHA2-128f"
	@echo "  run-128s       - Run with SLH-DSA-SHA2-128s"
	@echo "  run-192f       - Run with SLH-DSA-SHA2-192f"

.PHONY: all run test clean check-deps install-openssl rebuild-liboqs debug release info help run-128f run-128s run-192f
	