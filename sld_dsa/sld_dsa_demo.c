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
    size_t display_len = (len > 32) ? 32 : len;
    for (size_t i = 0; i < display_len; i++) printf("%02X", buf[i]);
    if (len > 32) printf("...");
    printf("\n");
}

static void handle_openssl_error(const char *msg) {
    fprintf(stderr, "ERROR: %s\n", msg);
    ERR_print_errors_fp(stderr);
}

static EVP_PKEY *generate_keys(const char *type) {
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *kctx = NULL;

    printf("🔐 Generating keys for: %s\n", type);
    
    kctx = EVP_PKEY_CTX_new_from_name(NULL, type, NULL);
    if (!kctx) {
        handle_openssl_error("Failed to create context");
        return NULL;
    }

    if (EVP_PKEY_keygen_init(kctx) <= 0) {
        handle_openssl_error("Failed to initialize key generation");
        EVP_PKEY_CTX_free(kctx);
        return NULL;
    }

    if (EVP_PKEY_keygen(kctx, &pkey) <= 0) {
        handle_openssl_error("Failed to generate key pair");
        EVP_PKEY_CTX_free(kctx);
        return NULL;
    }

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

int do_sign(EVP_PKEY *pkey, const char *msg, size_t msg_len, const char *type) {
    size_t sig_len;
    unsigned char *sig = NULL;
    EVP_PKEY_CTX *sctx = NULL;
    EVP_SIGNATURE *sig_alg = NULL;
    int ret = 0;

    printf("\n📝 Signing message with %s\n", type);
    printf("Message: \"%s\" (%zu bytes)\n", msg, msg_len);

    const OSSL_PARAM params[] = {
        OSSL_PARAM_octet_string("context-string", (unsigned char *)"Context string", 14),
        OSSL_PARAM_END
    };

    sctx = EVP_PKEY_CTX_new_from_pkey(NULL, pkey, NULL);
    if (!sctx) {
        handle_openssl_error("Failed to create signing context");
        return 0;
    }

    sig_alg = EVP_SIGNATURE_fetch(NULL, type, NULL);
    if (!sig_alg) {
        handle_openssl_error("Failed to fetch signature algorithm");
        EVP_PKEY_CTX_free(sctx);
        return 0;
    }

    if (EVP_PKEY_sign_message_init(sctx, sig_alg, params) <= 0) {
        handle_openssl_error("Failed to initialize message signing");
        goto cleanup;
    }

    /* Get size of signature */
    if (EVP_PKEY_sign(sctx, NULL, &sig_len, (const unsigned char *)msg, msg_len) <= 0) {
        handle_openssl_error("Failed to get signature size");
        goto cleanup;
    }

    sig = OPENSSL_zalloc(sig_len);
    if (!sig) {
        handle_openssl_error("Failed to allocate memory for signature");
        goto cleanup;
    }

    if (EVP_PKEY_sign(sctx, sig, &sig_len, (const unsigned char *)msg, msg_len) <= 0) {
        handle_openssl_error("Failed to sign message");
        goto cleanup;
    }

    printf("✅ Signature length: %zu bytes\n", sig_len);
    hexdump("Signature", sig, sig_len);
    ret = 1;

cleanup:
    if (sig) OPENSSL_free(sig);
    if (sig_alg) EVP_SIGNATURE_free(sig_alg);
    if (sctx) EVP_PKEY_CTX_free(sctx);
    return ret;
}

int main(int argc, char** argv) {
    printf("🚀 SLH-DSA Signature Demo\n");
    printf("=========================\n");

    // Initialize OpenSSL
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    const char *type = "SLH-DSA-SHA2-128f";
    const char *msg = "Hello";
    size_t msg_len = 5;

    if (argc > 1) type = argv[1];
    if (argc > 2) {
        msg = argv[2];
        msg_len = strlen(msg);
    }

    printf("Using algorithm: %s\n", type);
    printf("Using message: \"%s\"\n", msg);

    EVP_PKEY *key = generate_keys(type);
    if (!key) {
        fprintf(stderr, "\n❌ Key generation failed!\n");
        fprintf(stderr, "💡 Make sure you're using OQS OpenSSL with SLH-DSA support\n");
        fprintf(stderr, "💡 Available algorithms might include:\n");
        fprintf(stderr, "   - SLH-DSA-SHA2-128s, SLH-DSA-SHA2-128f\n");
        fprintf(stderr, "   - SLH-DSA-SHA2-192s, SLH-DSA-SHA2-192f\n");
        fprintf(stderr, "   - SLH-DSA-SHA2-256s, SLH-DSA-SHA2-256f\n");
        return EXIT_FAILURE;
    }

    if (!do_sign(key, msg, msg_len, type)) {
        fprintf(stderr, "\n❌ Signing failed!\n");
        EVP_PKEY_free(key);
        return EXIT_FAILURE;
    }

    printf("\n🎉 SLH-DSA operations completed successfully!\n");

    EVP_PKEY_free(key);
    
    // Cleanup
    EVP_cleanup();
    ERR_free_strings();
    
    return EXIT_SUCCESS;
}

/*
*/