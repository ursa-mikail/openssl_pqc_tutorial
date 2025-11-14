# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lssl -lcrypto

# OpenSSL detection
OPENSSL_INCLUDE = $(shell pkg-config --cflags openssl 2>/dev/null || echo "-I/usr/include/openssl")
OPENSSL_LIB = $(shell pkg-config --libs openssl 2>/dev/null || echo "-lssl -lcrypto")

# Targets
TARGET = mlkem_demo
SOURCES = mlkem_demo.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS) $(OPENSSL_LIB)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) $(OPENSSL_INCLUDE) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Test different ML-KEM variants
test: $(TARGET)
	@echo "Testing ML-KEM variants:"
	@echo "========================"
	./$(TARGET) ML-KEM-512
	@echo
	./$(TARGET) ML-KEM-768
	@echo
	./$(TARGET) ML-KEM-1024

# Test with alternative algorithm names
test-all: $(TARGET)
	@echo "Testing all KEM algorithm names:"
	@echo "================================"
	@for algo in "ML-KEM-512" "ML-KEM-768" "ML-KEM-1024" "kyber512" "kyber768" "kyber1024"; do \
		echo "Testing $$algo:"; \
		./$(TARGET) "$$algo" 2>/dev/null && echo "✅ Success" || echo "❌ Failed"; \
		echo; \
	done

# Clean build files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Install dependencies (macOS)
install-deps-macos:
	brew install openssl pkg-config

# Install dependencies (Ubuntu/Debian)
install-deps-ubuntu:
	sudo apt-get update
	sudo apt-get install libssl-dev pkg-config

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build
release: CFLAGS += -O2 -DNDEBUG
release: $(TARGET)

# Show OpenSSL configuration
show-config:
	@echo "OpenSSL include: $(OPENSSL_INCLUDE)"
	@echo "OpenSSL lib: $(OPENSSL_LIB)"
	@echo "OpenSSL version: $(shell openssl version 2>/dev/null || echo "Not found")"
	@echo "OpenSSL path: $(shell which openssl)"

# Check if OpenSSL supports ML-KEM
check-mlkem:
	@echo "Checking ML-KEM support in OpenSSL:"
	@openssl list -public-key-algorithms 2>/dev/null | grep -i "ml-kem\|kyber" || echo "No ML-KEM algorithms found"

# Help target
help:
	@echo "Available targets:"
	@echo "  all           - Build the program (default)"
	@echo "  run           - Build and run with default parameters"
	@echo "  test          - Test ML-KEM-512/768/1024"
	@echo "  test-all      - Test all possible algorithm names"
	@echo "  clean         - Remove build files"
	@echo "  install-deps  - Install dependencies"
	@echo "  debug         - Build with debug flags"
	@echo "  release       - Build with optimization flags"
	@echo "  show-config   - Show OpenSSL configuration"
	@echo "  check-mlkem   - Check if OpenSSL supports ML-KEM"

.PHONY: all run test test-all clean install-deps-macos install-deps-ubuntu debug release show-config check-mlkem help