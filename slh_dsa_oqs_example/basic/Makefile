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
all: $(TARGET)

# Main executable
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) -o $(TARGET) $(LIB) $(LIBS)

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

# Check what signature algorithms are available
check-algorithms: $(TARGET)
	@echo "🔍 Checking available signature algorithms..."
	@./$(TARGET) 2>&1 | grep -A 20 "Available SLH-DSA"

# Rebuild liboqs with SLH-DSA support
rebuild-liboqs:
	@echo "🏗️  Rebuilding liboqs with SLH-DSA support..."
	cd .. && rm -rf build && mkdir build && cd build && \
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DOQS_ENABLE_SIG_SPHINCS=ON .. && \
	make -j$(sysctl -n hw.ncpu) && \
	echo "✅ liboqs rebuilt with SLH-DSA support!"

# Help target
help:
	@echo "Available targets:"
	@echo "  all               - Build the program (default)"
	@echo "  run               - Build and run with default parameters"
	@echo "  test              - Test SLH-DSA variants"
	@echo "  clean             - Remove build files"
	@echo "  check-algorithms  - Check available signature algorithms"
	@echo "  rebuild-liboqs    - Rebuild liboqs with SLH-DSA support"

.PHONY: all run test clean check-algorithms rebuild-liboqs help