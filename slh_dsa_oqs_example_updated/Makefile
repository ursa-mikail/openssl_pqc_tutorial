# Makefile for Post-Quantum Signatures Demo
CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g

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
TARGET = pq_sig_demo
SOURCES = slh_dsa_demo_fixed.c

# Default target
all:
	$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) -o $(TARGET) $(LIB) $(LIBS)

# Run with default algorithm (ML-DSA-44)
run: all
	./$(TARGET)

# Test different algorithms
test: all
	@echo "🧪 Testing Available Algorithms"
	@echo "==============================="
	@for algo in "ML-DSA-44" "ML-DSA-65" "Falcon-512" "SPHINCS+-SHA2-128f-simple"; do \
		echo "Testing $$algo..."; \
		./$(TARGET) "$$algo" 2>/dev/null && echo "✅ $$algo: SUCCESS" || echo "❌ $$algo: FAILED"; \
	done

# Run specific algorithms
run-mldsa44: all
	./$(TARGET) ML-DSA-44

run-mldsa65: all
	./$(TARGET) ML-DSA-65

run-falcon512: all
	./$(TARGET) Falcon-512

run-sphincs128: all
	./$(TARGET) SPHINCS+-SHA2-128f-simple

# Clean
clean:
	rm -f $(TARGET)

# Quick build
quick:
	$(CC) $(CFLAGS) -I../include $(SOURCES) -o $(TARGET) -L../lib -loqs -lcrypto

# Help
help:
	@echo "Available targets:"
	@echo "  all              - Build the program"
	@echo "  run              - Run with ML-DSA-44 (default)"
	@echo "  test             - Test multiple algorithms"
	@echo "  run-mldsa44      - Run with ML-DSA-44"
	@echo "  run-mldsa65      - Run with ML-DSA-65"
	@echo "  run-falcon512    - Run with Falcon-512"
	@echo "  run-sphincs128   - Run with SPHINCS+-SHA2-128f-simple"
	@echo "  clean            - Remove build files"
	@echo "  quick            - Quick build"

.PHONY: all run test run-mldsa44 run-mldsa65 run-falcon512 run-sphincs128 clean quick help