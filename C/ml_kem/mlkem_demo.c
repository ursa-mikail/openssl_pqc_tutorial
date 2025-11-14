#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/core_names.h>
#include <openssl/params.h>
#include <openssl/ec.h>

static void hexdump(const char *label, const unsigned char *buf, size_t len) {
    printf("%s (%zu bytes): ", label, len);
    for (size_t i = 0; i < len && i < 32; i++) printf("%02X", buf[i]);
    if (len > 32) printf("...");
    printf("\n");
}

static void handle_openssl_error(const char *msg) {
    fprintf(stderr, "ERROR: %s\n", msg);
    ERR_print_errors_fp(stderr);
}

static EVP_PKEY *generate_mlkem(const char *type) {
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *kctx = NULL;

    printf("🔐 Generating ML-KEM keys for: %s\n", type);
    
    // Create context for the algorithm
    kctx = EVP_PKEY_CTX_new_from_name(NULL, type, NULL);
    if (!kctx) {
        handle_openssl_error("Failed to create context");
        return NULL;
    }

    // Initialize key generation
    if (EVP_PKEY_keygen_init(kctx) <= 0) {
        handle_openssl_error("Failed to initialize key generation");
        EVP_PKEY_CTX_free(kctx);
        return NULL;
    }

    // Generate key pair
    if (EVP_PKEY_keygen(kctx, &pkey) <= 0) {
        handle_openssl_error("Failed to generate key pair");
        EVP_PKEY_CTX_free(kctx);
        return NULL;
    }

    // Extract key parameters
    uint8_t pub[2592], priv[4896];
    size_t priv_len = 0, pub_len = 0;

    if (EVP_PKEY_get_octet_string_param(pkey, OSSL_PKEY_PARAM_PRIV_KEY,
                                      priv, sizeof(priv), &priv_len) <= 0) {
        handle_openssl_error("Failed to get private key");
    } else {
        printf("✅ Private key length: %zu bytes\n", priv_len);
        hexdump("Private key", priv, priv_len);
    }

    if (EVP_PKEY_get_octet_string_param(pkey, OSSL_PKEY_PARAM_PUB_KEY,
                                      pub, sizeof(pub), &pub_len) <= 0) {
        handle_openssl_error("Failed to get public key");
    } else {
        printf("✅ Public key length: %zu bytes\n", pub_len);
        hexdump("Public key", pub, pub_len);
    }

    EVP_PKEY_CTX_free(kctx);
    return pkey;
}

void demonstrate_kem_operations(EVP_PKEY *pkey, const char *type) {
    printf("\n🎯 Demonstrating KEM operations for %s\n", type);
    
    // For a complete KEM demo, we would:
    // 1. Get the public key from the key pair
    // 2. Create an encapsulation context
    // 3. Generate ciphertext and shared secret
    // 4. Decapsulate using the private key
    // 5. Verify shared secrets match
    
    printf("   (KEM encapsulation/decapsulation would go here)\n");
    printf("   Shared secret could be used for AES encryption\n");
}

int main(int argc, char** argv) {
    // Initialize OpenSSL
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    const char *type = "ML-KEM-768";
    
    if (argc > 1) {
        type = argv[1];
    }

    printf("🚀 ML-KEM Key Generation Demo\n");
    printf("=============================\n");

    EVP_PKEY *mlkem = generate_mlkem(type);
    
    if (mlkem) {
        printf("\n✅ ML-KEM key generation successful!\n");
        demonstrate_kem_operations(mlkem, type);
        EVP_PKEY_free(mlkem);
    } else {
        printf("\n❌ ML-KEM key generation failed!\n");
        printf("💡 Try these algorithm names:\n");
        printf("   - ML-KEM-512, ML-KEM-768, ML-KEM-1024\n");
        printf("   - kyber512, kyber768, kyber1024\n");
        printf("💡 Make sure you're using OQS OpenSSL\n");
    }

    // Cleanup
    EVP_cleanup();
    ERR_free_strings();
    
    return mlkem ? EXIT_SUCCESS : EXIT_FAILURE;
}


/*
🚀 ML-KEM Key Generation Demo
=============================
🔐 Generating ML-KEM keys for: ML-KEM-768
✅ Private key length: 2400 bytes
Private key (2400 bytes): CC4B066D059BAAD12823276D042A31DD9CC1858A8D37B86F521A61BE33073010...
✅ Public key length: 1184 bytes
Public key (1184 bytes): 280A47014A6A8C27C7F0B083103B9568C868A38556EA8C8785743745D5467354...

✅ ML-KEM key generation successful!

🎯 Demonstrating KEM operations for ML-KEM-768
   (KEM encapsulation/decapsulation would go here)
   Shared secret could be used for AES encryption
*/