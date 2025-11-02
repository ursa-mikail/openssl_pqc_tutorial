#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>  // Added for clock() and clock_t
#include "oqs/oqs.h"

void print_hex(const char* label, const uint8_t* data, size_t len) {
    printf("%s (%zu bytes): ", label, len);
    for (size_t i = 0; i < len && i < 32; i++) {
        printf("%02x", data[i]);
    }
    if (len > 32) printf("...");
    printf("\n");
}

void list_available_slh_dsa() {
    printf("📋 Available SLH-DSA (SPHINCS+) Algorithms:\n");
    printf("===========================================\n");
    
    int count = 0;
    int enabled_count = 0;
    
    // SLH-DSA algorithm names
    const char* slh_dsa_algorithms[] = {
        "SLH-DSA-SHA2-128f", "SLH-DSA-SHA2-128s",
        "SLH-DSA-SHA2-192f", "SLH-DSA-SHA2-192s", 
        "SLH-DSA-SHA2-256f", "SLH-DSA-SHA2-256s",
        "SLH-DSA-SHAKE-128f", "SLH-DSA-SHAKE-128s",
        "SLH-DSA-SHAKE-192f", "SLH-DSA-SHAKE-192s",
        "SLH-DSA-SHAKE-256f", "SLH-DSA-SHAKE-256s",
        NULL
    };
    
    for (int i = 0; slh_dsa_algorithms[i] != NULL; i++) {
        const char* alg_name = slh_dsa_algorithms[i];
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
    
    if (enabled_count == 0) {
        printf("\n❌ No SLH-DSA algorithms are enabled!\n");
        printf("💡 Rebuild liboqs with: -DOQS_ENABLE_SIG_SPHINCS=ON\n");
    } else {
        printf("\n✅ %d of %d SLH-DSA algorithms enabled\n", enabled_count, count);
    }
}

void demonstrate_slh_dsa(const char* sig_name) {
    printf("\n🌳 Testing %s\n", sig_name);
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
    const char* message1 = "This is the first document to sign with SLH-DSA";
    const char* message2 = "This is the second document with different content";
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
    
    // Step 5: Sign and verify a second message (demonstrating stateless nature)
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
    const char* tampered_message = "This is a tampered document to sign with SLH-DSA";
    size_t tampered_len = strlen(tampered_message);
    
    if (OQS_SIG_verify(sig, (const uint8_t*)tampered_message, tampered_len, 
                      signature, signature_len, public_key) == OQS_SUCCESS) {
        printf("❌ FAILED: Tampered message was accepted!\n");
    } else {
        printf("✅ SUCCESS: Tampered message correctly rejected\n");
    }
    
    printf("\n💡 SLH-DSA Characteristics:\n");
    printf("   • Stateless hash-based signatures\n");
    printf("   • Based on hash functions only (no number theory)\n");
    printf("   • Large signature sizes but very secure\n");
    printf("   • Suitable for long-term security requirements\n");
    
cleanup:
    OQS_SIG_free(sig);
    if (public_key) free(public_key);
    if (secret_key) free(secret_key);
    if (signature) free(signature);
}

void benchmark_slh_dsa(const char* sig_name) {
    if (!OQS_SIG_alg_is_enabled(sig_name)) {
        return;
    }
    
    OQS_SIG* sig = OQS_SIG_new(sig_name);
    if (!sig) return;
    
    uint8_t* public_key = malloc(sig->length_public_key);
    uint8_t* secret_key = malloc(sig->length_secret_key);
    uint8_t* signature = malloc(sig->length_signature);
    size_t signature_len;
    
    const char* message = "Benchmark message";
    size_t message_len = strlen(message);
    
    printf("\n⏱️  Benchmarking %s:\n", sig_name);
    
    // Time key generation
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    if (OQS_SIG_keypair(sig, public_key, secret_key) == OQS_SUCCESS) {
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
        time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-6; // Convert to milliseconds
        printf("   Key generation: %.3f ms\n", time_taken);
    }
    
    // Time signing
    clock_gettime(CLOCK_MONOTONIC, &start);
    if (OQS_SIG_sign(sig, signature, &signature_len, 
                    (const uint8_t*)message, message_len, secret_key) == OQS_SUCCESS) {
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
        time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-6;
        printf("   Signing:        %.3f ms\n", time_taken);
    }
    
    // Time verification
    clock_gettime(CLOCK_MONOTONIC, &start);
    if (OQS_SIG_verify(sig, (const uint8_t*)message, message_len, 
                      signature, signature_len, public_key) == OQS_SUCCESS) {
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
        time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-6;
        printf("   Verification:   %.3f ms\n", time_taken);
    }
    
    OQS_SIG_free(sig);
    free(public_key);
    free(secret_key);
    free(signature);
}

// Simple timing without benchmarking if clock_gettime is not available
void simple_demo_slh_dsa(const char* sig_name) {
    if (!OQS_SIG_alg_is_enabled(sig_name)) {
        printf("❌ %s is not enabled\n", sig_name);
        return;
    }
    
    OQS_SIG* sig = OQS_SIG_new(sig_name);
    if (!sig) {
        printf("❌ Failed to initialize %s\n", sig_name);
        return;
    }
    
    printf("\n🌳 Simple Demo: %s\n", sig_name);
    printf("==================\n");
    
    uint8_t* public_key = malloc(sig->length_public_key);
    uint8_t* secret_key = malloc(sig->length_secret_key);
    uint8_t* signature = malloc(sig->length_signature);
    size_t signature_len;
    
    const char* message = "Test message for SLH-DSA";
    size_t message_len = strlen(message);
    
    // Key generation
    printf("1. Generating keys... ");
    if (OQS_SIG_keypair(sig, public_key, secret_key) == OQS_SUCCESS) {
        printf("✅\n");
    } else {
        printf("❌\n");
        goto cleanup;
    }
    
    // Signing
    printf("2. Signing message... ");
    if (OQS_SIG_sign(sig, signature, &signature_len, 
                    (const uint8_t*)message, message_len, secret_key) == OQS_SUCCESS) {
        printf("✅ (%zu bytes)\n", signature_len);
    } else {
        printf("❌\n");
        goto cleanup;
    }
    
    // Verification
    printf("3. Verifying signature... ");
    if (OQS_SIG_verify(sig, (const uint8_t*)message, message_len, 
                      signature, signature_len, public_key) == OQS_SUCCESS) {
        printf("✅\n");
    } else {
        printf("❌\n");
    }
    
cleanup:
    OQS_SIG_free(sig);
    free(public_key);
    free(secret_key);
    free(signature);
}

int main(int argc, char* argv[]) {
    printf("🎯 SLH-DSA (SPHINCS+) Signature Demonstration\n");
    printf("============================================\n");
    
    // List all available SLH-DSA algorithms
    list_available_slh_dsa();
    
    // Determine which algorithm to use
    const char* algorithm_to_use = NULL;
    
    // Preferred SLH-DSA algorithms in order
    const char* preferred_algorithms[] = {
        "SLH-DSA-SHA2-128f", "SLH-DSA-SHA2-128s",
        "SLH-DSA-SHAKE-128f", "SLH-DSA-SHAKE-128s",
        "SLH-DSA-SHA2-192f", "SLH-DSA-SHA2-192s",
        "SLH-DSA-SHAKE-192f", "SLH-DSA-SHAKE-192s",
        "SLH-DSA-SHA2-256f", "SLH-DSA-SHA2-256s",
        "SLH-DSA-SHAKE-256f", "SLH-DSA-SHAKE-256s",
        NULL
    };
    
    // Use command line argument if provided
    if (argc > 1) {
        algorithm_to_use = argv[1];
    } else {
        // Find first available algorithm
        for (int i = 0; preferred_algorithms[i] != NULL; i++) {
            if (OQS_SIG_alg_is_enabled(preferred_algorithms[i])) {
                algorithm_to_use = preferred_algorithms[i];
                break;
            }
        }
    }
    
    if (algorithm_to_use && OQS_SIG_alg_is_enabled(algorithm_to_use)) {
        printf("\n🎯 Using algorithm: %s\n", algorithm_to_use);
        
        // Choose which demo to run based on available timing functions
        #ifdef CLOCK_MONOTONIC
        demonstrate_slh_dsa(algorithm_to_use);
        // Benchmark a few algorithms if available
        printf("\n📊 Performance Benchmarks:\n");
        printf("=========================\n");
        benchmark_slh_dsa("SLH-DSA-SHA2-128f");
        benchmark_slh_dsa("SLH-DSA-SHA2-128s");
        benchmark_slh_dsa("SLH-DSA-SHA2-192f");
        #else
        simple_demo_slh_dsa(algorithm_to_use);
        #endif
    } else {
        printf("\n❌ No suitable SLH-DSA algorithms found!\n");
        printf("💡 Rebuild liboqs with SPHINCS+ support:\n");
        printf("   cmake -DOQS_ENABLE_SIG_SPHINCS=ON ..\n");
        printf("   make -j$(nproc)\n");
        
        // Show what IS available
        printf("\n🔍 Available signature algorithms:\n");
        int count = 0;
        for (int i = 0; i < OQS_SIG_alg_count(); i++) {
            const char* alg_name = OQS_SIG_alg_identifier(i);
            if (OQS_SIG_alg_is_enabled(alg_name)) {
                printf("  - %s\n", alg_name);
                count++;
            }
        }
        if (count == 0) {
            printf("  No signature algorithms enabled!\n");
        }
        return 1;
    }
    
    printf("\n✨ SLH-DSA demonstration completed!\n");
    return 0;
}