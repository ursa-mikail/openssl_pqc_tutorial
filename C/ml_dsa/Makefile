# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lssl -lcrypto

# OpenSSL detection (modify paths if needed)
OPENSSL_INCLUDE = $(shell pkg-config --cflags openssl 2>/dev/null || echo "-I/usr/include/openssl")
OPENSSL_LIB = $(shell pkg-config --libs openssl 2>/dev/null || echo "-lssl -lcrypto")

# Targets
TARGET = mldsa_demo
SOURCES = mldsa_demo.c
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

# Test different ML-DSA variants
test: $(TARGET)
	@echo "Testing ML-DSA variants:"
	@echo "========================"
	./$(TARGET) mldsa44 "Test message for ML-DSA-44"
	@echo
	./$(TARGET) mldsa65 "Test message for ML-DSA-65"  
	@echo
	./$(TARGET) mldsa87 "Test message for ML-DSA-87"

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

# Help target
help:
	@echo "Available targets:"
	@echo "  all           - Build the program (default)"
	@echo "  run           - Build and run with default parameters"
	@echo "  test          - Test all ML-DSA variants"
	@echo "  clean         - Remove build files"
	@echo "  install-deps-macos  - Install dependencies on macOS"
	@echo "  install-deps-ubuntu - Install dependencies on Ubuntu/Debian"
	@echo "  debug         - Build with debug flags"
	@echo "  release       - Build with optimization flags"
	@echo "  show-config   - Show OpenSSL configuration"

.PHONY: all run test clean install-deps-macos install-deps-ubuntu debug release show-config help