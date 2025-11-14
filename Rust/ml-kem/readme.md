# ML-KEM (Module-Lattice Key Encapsulation Mechanism)

A Rust implementation and testing framework for ML-KEM (formerly known as Kyber), a post-quantum key encapsulation mechanism.

## Overview

This project provides a comprehensive testing framework for ML-KEM implementations using the `fips203` crate. It supports all three security levels of ML-KEM as standardized in FIPS 203.

## Security Levels

| Variant | Security Level | Description |
|---------|---------------|-------------|
| ML-KEM-512 | Level 1 | Fastest, suitable for most applications |
| ML-KEM-768 | Level 3 | Balanced performance and security |
| ML-KEM-1024 | Level 5 | Highest security, recommended for long-term protection |

## Project Structure


```
% make run-all
Backed up original main.rs to main.rs.orig

🚀 Starting ML-KEM test suite...

========================================
Running ML-KEM version: _1024
========================================
  :

Method: ML-KEM-1024

Alice generated keypair:
  Encapsulation key length: 1568 bytes
  Encapsulation key (hex, truncated): 2a082eb8b3ad2d62b940066c9cf89b4eeb358d2b50a27657b17cbf36b597955c8fd7426b85e68e1a19ad5a1a2680404f00a8...
Bob encapsulated shared secret:
  Ciphertext length: 1568 bytes
  Ciphertext (hex, truncated): db921421629f623a6d88460159132129bff74cc8e6c93be03c57607fd1b1a71a46cfb1f5bdd99d7bc6ac51d7c61426acffd6...

✅ Key exchange successful! Both parties have the same shared secret.
⏱️  Time for _1024: 5 seconds

========================================
Running ML-KEM version: _512
========================================
   Compiling ml_kem v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust/ml-kem)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.14s
     Running `target/debug/ml_kem`

Method: ML-KEM-512

Alice generated keypair:
  Encapsulation key length: 800 bytes
  Encapsulation key (hex, truncated): 2f9c69524aaf0c47196e76ce09011099ccce9bc83d66e0ac540218b7a06ea63147418371539b6aa307bab8ec0b00064ddfb8...
Bob encapsulated shared secret:
  Ciphertext length: 768 bytes
  Ciphertext (hex, truncated): 542ee0a9b0f771d547c9104fe7ca0176bbd65f49519f9f038c859286072e35b385154bfe220e5d94781d54bf55d56356d663...

✅ Key exchange successful! Both parties have the same shared secret.
⏱️  Time for _512: 0 seconds

========================================
Running ML-KEM version: _768
========================================
   Compiling ml_kem v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust/ml-kem)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.13s
     Running `target/debug/ml_kem`

Method: ML-KEM-768

Alice generated keypair:
  Encapsulation key length: 1184 bytes
  Encapsulation key (hex, truncated): 288a97de3a3c257691855bc7cea6557ff7ae34757b2ec1a491949bdd85185b119acee41f64f7c3ba0a813dc305329a0a4d95...
Bob encapsulated shared secret:
  Ciphertext length: 1088 bytes
  Ciphertext (hex, truncated): 2170889001f3f5a1453b23a8ce7b6d9bbfcad791229039a63b2775709d5013c3cae495c4ae3f6b07302e0edfd9c692907de2...

✅ Key exchange successful! Both parties have the same shared secret.
⏱️  Time for _768: 1 seconds

========================================
✅ All tests completed!
⏱️  Total execution time: 0m 6s (6 seconds)
========================================
Restored original main.rs
```
