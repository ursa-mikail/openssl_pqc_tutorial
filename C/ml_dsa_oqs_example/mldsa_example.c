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

void demonstrate_ml_dsa(const char* sig_name) {
    printf("\n📝 Testing %s\n", sig_name);
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
    printf("   Public key size: %zu bytes\n", sig->length_public_key);
    printf("   Secret key size: %zu bytes\n", sig->length_secret_key);
    printf("   Signature size: %zu bytes\n", sig->length_signature);
    
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
    
    // Step 2: Create message to sign
    const char* message = "This is an important document that requires post-quantum signatures!";
    size_t message_len = strlen(message);
    printf("2. 📄 Message to sign: \"%s\" (%zu bytes)\n", message, message_len);
    
    // Step 3: Sign the message
    printf("3. ✍️  Signing message...\n");
    if (OQS_SIG_sign(sig, signature, &signature_len, 
                    (const uint8_t*)message, message_len, secret_key) != OQS_SUCCESS) {
        printf("❌ Signing failed\n");
        goto cleanup;
    }
    printf("✅ Message signed\n");
    print_hex("   Signature", signature, signature_len);
    
    // Step 4: Verify the signature
    printf("4. ✅ Verifying signature...\n");
    if (OQS_SIG_verify(sig, (const uint8_t*)message, message_len, 
                      signature, signature_len, public_key) != OQS_SUCCESS) {
        printf("❌ Signature verification failed\n");
        goto cleanup;
    }
    printf("✅ Signature verified successfully\n");
    
    // Step 5: Test tamper detection
    printf("5. 🔒 Testing tamper detection...\n");
    const char* tampered_message = "This is a tampered document that requires post-quantum signatures!";
    size_t tampered_len = strlen(tampered_message);
    
    if (OQS_SIG_verify(sig, (const uint8_t*)tampered_message, tampered_len, 
                      signature, signature_len, public_key) == OQS_SUCCESS) {
        printf("❌ FAILED: Tampered message was accepted!\n");
    } else {
        printf("✅ SUCCESS: Tampered message correctly rejected\n");
    }
    
    printf("\n💡 Practical usage:\n");
    printf("   • Digital document signing\n");
    printf("   • Code signing\n");
    printf("   • Certificate authorities\n");
    printf("   • Secure boot verification\n");
    
cleanup:
    OQS_SIG_free(sig);
    if (public_key) free(public_key);
    if (secret_key) free(secret_key);
    if (signature) free(signature);
}

void list_available_signatures() {
    printf("📋 Available Signature Algorithms:\n");
    printf("==================================\n");
    
    int count = 0;
    for (int i = 0; i < OQS_SIG_alg_count(); i++) {
        const char* alg_name = OQS_SIG_alg_identifier(i);
        if (OQS_SIG_alg_is_enabled(alg_name)) {
            OQS_SIG* sig = OQS_SIG_new(alg_name);
            if (sig) {
                printf("  %2d. %s\n", ++count, alg_name);
                printf("      Public: %4zu bytes, Secret: %4zu bytes, Signature: %4zu bytes\n", 
                       sig->length_public_key, sig->length_secret_key, sig->length_signature);
                OQS_SIG_free(sig);
            }
        }
    }
    
    if (count == 0) {
        printf("   No signature algorithms enabled\n");
    }
}

int main(int argc, char* argv[]) {
    printf("🎯 ML-DSA (Dilithium) Signature Demonstration\n");
    printf("============================================\n");
    
    // List available algorithms
    list_available_signatures();
    
    const char* algorithm = "Dilithium2";
    if (argc > 1) {
        algorithm = argv[1];
    }
    
    printf("\nUsing algorithm: %s\n", algorithm);
    
    // Test specific ML-DSA variants
    const char* ml_dsa_variants[] = {
        "Dilithium2", "Dilithium3", "Dilithium5",
        "ML-DSA-44", "ML-DSA-65", "ML-DSA-87",
        NULL
    };
    
    int tested = 0;
    for (int i = 0; ml_dsa_variants[i] != NULL; i++) {
        demonstrate_ml_dsa(ml_dsa_variants[i]);
        tested++;
        if (tested >= 1) break; // Test only the first available for demo
    }
    
    if (tested == 0) {
        printf("\n💡 No ML-DSA variants found. Available algorithms:\n");
        list_available_signatures();
    }
    
    printf("\n✨ ML-DSA demonstration completed!\n");
    return 0;
}

