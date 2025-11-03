#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "oqs/oqs.h"

void print_hex(const char* label, const uint8_t* data, size_t len) {
    printf("%s (%zu bytes): ", label, len);
    for (size_t i = 0; i < len && i < 32; i++) {
        printf("%02x", data[i]);
    }
    if (len > 32) printf("...");
    printf("\n");
}

void list_available_signatures() {
    printf("📋 Available Signature Algorithms:\n");
    printf("==================================\n");
    
    int count = 0;
    int enabled_count = 0;
    
    for (int i = 0; i < OQS_SIG_alg_count(); i++) {
        const char* alg_name = OQS_SIG_alg_identifier(i);
        int enabled = OQS_SIG_alg_is_enabled(alg_name);
        printf("  %2d. %s - %s\n", ++count, alg_name, enabled ? "✅ ENABLED" : "❌ DISABLED");
        
        if (enabled) {
            enabled_count++;
            OQS_SIG* sig = OQS_SIG_new(alg_name);
            if (sig) {
                printf("      Public: %4zu bytes, Secret: %4zu bytes, Signature: %4zu bytes\n", 
                       sig->length_public_key, sig->length_secret_key, sig->length_signature);
                OQS_SIG_free(sig);
            }
        }
    }
    
    printf("\n✅ %d of %d signature algorithms enabled\n", enabled_count, count);
}

void demonstrate_signature(const char* sig_name) {
    printf("\n🔐 Testing %s\n", sig_name);
    printf("================\n");
    
    if (!OQS_SIG_alg_is_enabled(sig_name)) {
        printf("❌ %s is not enabled in this build\n", sig_name);
        return;
    }
    
    OQS_SIG* sig = OQS_SIG_new(sig_name);
    if (!sig) {
        printf("❌ Failed to initialize signature scheme\n");
        return;
    }
    
    printf("✅ Algorithm: %s\n", sig_name);
    printf("   Public key: %zu bytes\n", sig->length_public_key);
    printf("   Secret key: %zu bytes\n", sig->length_secret_key);
    printf("   Signature: %zu bytes\n", sig->length_signature);
    
    // Allocate memory
    uint8_t* public_key = malloc(sig->length_public_key);
    uint8_t* secret_key = malloc(sig->length_secret_key);
    uint8_t* signature = malloc(sig->length_signature);
    size_t signature_len;
    
    if (!public_key || !secret_key || !signature) {
        printf("❌ Memory allocation failed\n");
        goto cleanup;
    }
    
    // Step 1: Key generation
    printf("1. 🔑 Generating key pair...\n");
    if (OQS_SIG_keypair(sig, public_key, secret_key) != OQS_SUCCESS) {
        printf("❌ Key generation failed\n");
        goto cleanup;
    }
    printf("✅ Key pair generated\n");
    
    // Step 2: Create messages to sign
    const char* message1 = "This document demonstrates post-quantum cryptography";
    const char* message2 = "Second message with different content for testing";
    size_t message1_len = strlen(message1);
    size_t message2_len = strlen(message2);
    
    printf("2. 📄 Message 1: \"%s\" (%zu bytes)\n", message1, message1_len);
    
    // Step 3: Sign the first message
    printf("3. ✍️  Signing message 1...\n");
    if (OQS_SIG_sign(sig, signature, &signature_len, 
                    (const uint8_t*)message1, message1_len, secret_key) != OQS_SUCCESS) {
        printf("❌ Signing failed\n");
        goto cleanup;
    }
    printf("✅ Message 1 signed\n");
    print_hex("   Signature", signature, signature_len);
    
    // Step 4: Verify the first signature
    printf("4. ✅ Verifying signature 1...\n");
    if (OQS_SIG_verify(sig, (const uint8_t*)message1, message1_len, 
                      signature, signature_len, public_key) != OQS_SUCCESS) {
        printf("❌ Signature verification failed\n");
        goto cleanup;
    }
    printf("✅ Signature 1 verified successfully!\n");
    
    // Step 5: Sign and verify a second message
    printf("5. 📄 Message 2: \"%s\" (%zu bytes)\n", message2, message2_len);
    printf("   ✍️  Signing message 2...\n");
    if (OQS_SIG_sign(sig, signature, &signature_len, 
                    (const uint8_t*)message2, message2_len, secret_key) != OQS_SUCCESS) {
        printf("❌ Signing message 2 failed\n");
        goto cleanup;
    }
    printf("✅ Message 2 signed\n");
    
    if (OQS_SIG_verify(sig, (const uint8_t*)message2, message2_len, 
                      signature, signature_len, public_key) != OQS_SUCCESS) {
        printf("❌ Signature 2 verification failed\n");
        goto cleanup;
    }
    printf("✅ Signature 2 verified successfully!\n");
    
    // Step 6: Test tamper detection
    printf("6. 🔒 Testing tamper detection...\n");
    const char* tampered_message = "This tampered document should be rejected";
    size_t tampered_len = strlen(tampered_message);
    
    if (OQS_SIG_verify(sig, (const uint8_t*)tampered_message, tampered_len, 
                      signature, signature_len, public_key) == OQS_SUCCESS) {
        printf("❌ FAILED: Tampered message was accepted!\n");
    } else {
        printf("✅ SUCCESS: Tampered message correctly rejected\n");
    }
    
    printf("\n💡 Algorithm Information:\n");
    if (strstr(sig_name, "ML-DSA") != NULL || strstr(sig_name, "Dilithium") != NULL) {
        printf("   • ML-DSA (Dilithium) - Lattice-based signatures\n");
        printf("   • Moderate signature sizes\n");
        printf("   • Good performance balance\n");
    } else if (strstr(sig_name, "Falcon") != NULL) {
        printf("   • Falcon - Lattice-based signatures\n");
        printf("   • Very small signature sizes\n");
        printf("   • Good for constrained environments\n");
    } else if (strstr(sig_name, "SPHINCS") != NULL || strstr(sig_name, "SLH-DSA") != NULL) {
        printf("   • SLH-DSA (SPHINCS+) - Hash-based signatures\n");
        printf("   • Very large signature sizes\n");
        printf("   • Conservative security based on hash functions\n");
    }
    
cleanup:
    OQS_SIG_free(sig);
    if (public_key) free(public_key);
    if (secret_key) free(secret_key);
    if (signature) free(signature);
}

int main(int argc, char* argv[]) {
    printf("🎯 Post-Quantum Signature Demonstration\n");
    printf("======================================\n");
    
    // List all available signature algorithms
    list_available_signatures();
    
    // Determine which algorithm to use
    const char* algorithm_to_use = NULL;
    
    // Try these algorithms in order (they should be available based on your output)
    const char* available_algorithms[] = {
        "ML-DSA-44",        // Dilithium2 - 128-bit security
        "ML-DSA-65",        // Dilithium3 - 192-bit security  
        "ML-DSA-87",        // Dilithium5 - 256-bit security
        "Falcon-512",       // Small signatures
        "Falcon-1024",      // Higher security
        "SPHINCS+-SHA2-128f-simple",  // Hash-based
        "SPHINCS+-SHA2-192f-simple",  // Hash-based
        "SPHINCS+-SHA2-256f-simple",  // Hash-based
        NULL
    };
    
    // Use command line argument if provided
    if (argc > 1) {
        algorithm_to_use = argv[1];
    } else {
        // Use ML-DSA-44 (Dilithium2) as default since it's available
        algorithm_to_use = "ML-DSA-44";
    }
    
    if (algorithm_to_use && OQS_SIG_alg_is_enabled(algorithm_to_use)) {
        printf("\n🎯 Using algorithm: %s\n", algorithm_to_use);
        demonstrate_signature(algorithm_to_use);
    } else {
        printf("\n❌ Algorithm '%s' not found or not enabled!\n", algorithm_to_use);
        printf("💡 Try one of these available algorithms:\n");
        for (int i = 0; available_algorithms[i] != NULL; i++) {
            if (OQS_SIG_alg_is_enabled(available_algorithms[i])) {
                printf("   ./slh_dsa_demo_fixed %s\n", available_algorithms[i]);
            }
        }
        return 1;
    }
    
    printf("\n✨ Demonstration completed!\n");
    return 0;
}