#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "oqs/oqs.h"

void print_hex(const char* label, const uint8_t* data, size_t len) {
    printf("%s (%zu bytes): ", label, len);
    for (size_t i = 0; i < len && i < 16; i++) {
        printf("%02x", data[i]);
    }
    if (len > 16) printf("...");
    printf("\n");
}

int main(int argc, char* argv[]) {
    printf("🎯 ML-KEM Demonstration\n");
    printf("======================\n");
    
    const char* algorithm = "Kyber512";
    if (argc > 1) {
        algorithm = argv[1];
    }
    
    printf("Using algorithm: %s\n\n", algorithm);
    
    // Check if algorithm is available
    if (!OQS_KEM_alg_is_enabled(algorithm)) {
        printf("❌ Algorithm '%s' is not enabled\n", algorithm);
        printf("\nAvailable algorithms:\n");
        for (int i = 0; i < OQS_KEM_alg_count(); i++) {
            const char* alg_name = OQS_KEM_alg_identifier(i);
            if (OQS_KEM_alg_is_enabled(alg_name)) {
                printf("  - %s\n", alg_name);
            }
        }
        return 1;
    }
    
    OQS_KEM* kem = OQS_KEM_new(algorithm);
    if (!kem) {
        printf("❌ Failed to initialize KEM\n");
        return 1;
    }
    
    printf("✅ Algorithm initialized\n");
    printf("   Public key size: %zu bytes\n", kem->length_public_key);
    printf("   Secret key size: %zu bytes\n", kem->length_secret_key);
    printf("   Ciphertext size: %zu bytes\n", kem->length_ciphertext);
    printf("   Shared secret size: %zu bytes\n", kem->length_shared_secret);
    
    // Allocate memory
    uint8_t* public_key = malloc(kem->length_public_key);
    uint8_t* secret_key = malloc(kem->length_secret_key);
    uint8_t* ciphertext = malloc(kem->length_ciphertext);
    uint8_t* shared_secret_e = malloc(kem->length_shared_secret);
    uint8_t* shared_secret_d = malloc(kem->length_shared_secret);
    
    if (!public_key || !secret_key || !ciphertext || !shared_secret_e || !shared_secret_d) {
        printf("❌ Memory allocation failed\n");
        OQS_KEM_free(kem);
        return 1;
    }
    
    // Step 1: Key generation
    printf("\n1. 🔑 Generating key pair...\n");
    if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
        printf("❌ Key generation failed\n");
        goto cleanup;
    }
    printf("✅ Key pair generated\n");
    
    // Step 2: Encapsulation
    printf("2. 🔒 Encapsulating shared secret...\n");
    if (OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key) != OQS_SUCCESS) {
        printf("❌ Encapsulation failed\n");
        goto cleanup;
    }
    printf("✅ Key encapsulated\n");
    
    // Step 3: Decapsulation
    printf("3. 🔓 Decapsulating shared secret...\n");
    if (OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key) != OQS_SUCCESS) {
        printf("❌ Decapsulation failed\n");
        goto cleanup;
    }
    printf("✅ Key decapsulated\n");
    
    // Step 4: Verification
    printf("4. ✅ Verifying shared secrets...\n");
    if (memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret) == 0) {
        printf("✅ SUCCESS: Shared secrets match!\n");
        print_hex("   Shared secret", shared_secret_e, kem->length_shared_secret);
        
        printf("\n💡 This %zu-byte shared secret can be used for:\n", kem->length_shared_secret);
        printf("   • AES-256 encryption\n");
        printf("   • Secure channel establishment\n");
        printf("   • File encryption\n");
    } else {
        printf("❌ FAILED: Shared secrets don't match\n");
    }
    
cleanup:
    OQS_KEM_free(kem);
    free(public_key);
    free(secret_key);
    free(ciphertext);
    free(shared_secret_e);
    free(shared_secret_d);
    
    printf("\n✨ Demonstration completed!\n");
    return 0;
}


/*
Note: Install oqs

% pwd
/oqs-provider/liboqs/build/mlkem_example

% make run
clang mlkem_example.o -o mlkem_example -L../lib -L/opt/homebrew/opt/openssl/lib -loqs -lcrypto
./mlkem_example
✓ Key pair generated
✓ Key encapsulated
✓ Key decapsulated
✓ SUCCESS: Shared secrets match!
Shared secret: 077e98ae32d70571c68a714b7695dc22...
*/