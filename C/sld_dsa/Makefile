# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lssl -lcrypto

# OpenSSL detection
OPENSSL_INCLUDE = $(shell pkg-config --cflags openssl 2>/dev/null || echo "-I/usr/include/openssl")
OPENSSL_LIB = $(shell pkg-config --libs openssl 2>/dev/null || echo "-lssl -lcrypto")

# Targets
TARGET = slh_dsa_demo
SOURCES = slh_dsa_demo.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS) $(OPENSSL_LIB)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) $(OPENSSL_INCLUDE) -c $< -o $@

# Run the program with default parameters
run: $(TARGET)
	./$(TARGET)

# Test different SLH-DSA variants
test: $(TARGET)
	@echo "Testing SLH-DSA variants:"
	@echo "========================="
	@for algo in "SLH-DSA-SHA2-128s" "SLH-DSA-SHA2-128f" "SLH-DSA-SHA2-192s" "SLH-DSA-SHA2-192f" "SLH-DSA-SHA2-256s" "SLH-DSA-SHA2-256f"; do \
		echo "Testing $$algo:"; \
		./$(TARGET) "$$algo" "Test message for $$algo" 2>/dev/null && echo "✅ Success" || echo "❌ Failed"; \
		echo; \
	done

# Test with custom message
test-custom: $(TARGET)
	@echo "Testing with custom message:"
	./$(TARGET) "SLH-DSA-SHA2-128f" "This is a custom test message for SLH-DSA"

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

# Debug build with more flags
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

# Check if OpenSSL supports SLH-DSA
check-slh-dsa:
	@echo "Checking SLH-DSA support in OpenSSL:"
	@openssl list -public-key-algorithms 2>/dev/null | grep -i "slh-dsa" || echo "No SLH-DSA algorithms found"

# Check all available signature algorithms
check-sig-algorithms:
	@echo "Available signature algorithms:"
	@openssl list -signature-algorithms 2>/dev/null | head -20 || echo "Cannot list signature algorithms"

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Build the program (default)"
	@echo "  run              - Build and run with default parameters"
	@echo "  test             - Test all SLH-DSA variants"
	@echo "  test-custom      - Test with custom message"
	@echo "  clean            - Remove build files"
	@echo "  install-deps-macos  - Install dependencies on macOS"
	@echo "  install-deps-ubuntu - Install dependencies on Ubuntu/Debian"
	@echo "  debug            - Build with debug flags"
	@echo "  release          - Build with optimization flags"
	@echo "  show-config      - Show OpenSSL configuration"
	@echo "  check-slh-dsa    - Check if OpenSSL supports SLH-DSA"
	@echo "  check-sig-algorithms - Check available signature algorithms"

.PHONY: all run test test-custom clean install-deps-macos install-deps-ubuntu debug release show-config check-slh-dsa check-sig-algorithms help
	