# SLH-DSA (SPHINCS+) Rust Implementation

A comprehensive Rust implementation demonstrating all **12 parameter sets** of SLH-DSA (Stateless Hash-based Digital Signature Algorithm), also known as SPHINCS+, using the `slh-dsa` crate.

## Overview

SLH-DSA is a post-quantum digital signature scheme standardized by NIST as FIPS 205. This implementation demonstrates:

- Key generation
- Message signing with randomization
- Signature verification
- All 6 SHA2-based parameter sets
- All 6 SHAKE-based parameter sets

## Requirements

- Rust 1.70 or later
- Cargo

## Dependencies

```toml
[dependencies]
slh-dsa = "0.1.0"
hex = "0.4.3"
rand = "0.8.5"
signature = "2.2"
```

## Quick Start

```bash
# Build the project
make build

# Run a specific variant (fast)
make run-_sha2_128f

# Run all SHA2 variants
make run-sha2

# Run all SHAKE variants
make run-shake

# Run all fast variants (recommended)
make run-fast

# Run all variants (WARNING: slow variants will take minutes!)
make run-all

# Run with release optimizations (recommended for 's' variants)
make BUILD_MODE=release run-small

# Clean build artifacts
make clean
```

## Available Variants

The project includes **12 SLH-DSA parameter sets** across two hash function families:

### SHA2-based Variants (6 total)

| Variant | Security Level | Speed | Signature Size | Key Sizes (SK/PK) |
|---------|---------------|-------|----------------|-------------------|
| **sha2_128f** | 128-bit | Fast (~ms) | ~17 KB | 64 / 32 bytes |
| **sha2_128s** | 128-bit | Slow (10-30s) | ~7 KB | 64 / 32 bytes |
| **sha2_192f** | 192-bit | Fast (~ms) | ~35 KB | 96 / 48 bytes |
| **sha2_192s** | 192-bit | Slow (30-60s) | ~17 KB | 96 / 48 bytes |
| **sha2_256f** | 256-bit | Fast (~ms) | ~49 KB | 128 / 64 bytes |
| **sha2_256s** | 256-bit | Very Slow (60s+) | ~29 KB | 128 / 64 bytes |

### SHAKE-based Variants (6 total)

| Variant | Security Level | Speed | Signature Size | Key Sizes (SK/PK) |
|---------|---------------|-------|----------------|-------------------|
| **shake128f** | 128-bit | Fast (~ms) | ~17 KB | 64 / 32 bytes |
| **shake128s** | 128-bit | Slow (10-30s) | ~7 KB | 64 / 32 bytes |
| **shake192f** | 192-bit | Fast (~ms) | ~35 KB | 96 / 48 bytes |
| **shake192s** | 192-bit | Slow (30-60s) | ~16 KB | 96 / 48 bytes |
| **shake256f** | 256-bit | Fast (~ms) | ~49 KB | 128 / 64 bytes |
| **shake256s** | 256-bit | Very Slow (60s+) | ~29 KB | 128 / 64 bytes |

## ⚠️ IMPORTANT PERFORMANCE CAVEATS

### Speed vs Size Trade-off

SLH-DSA offers two variants for each security level and hash function:

- **"f" (fast) variants**: Optimized for speed, larger signatures
- **"s" (small) variants**: Optimized for size, MUCH slower

### Performance Differences

```
Variant      | Signing Time  | Signature Size | Recommendation
-------------|---------------|----------------|------------------
sha2_128f    | ~1-5 ms       | 17,088 bytes   | ✅ Use for testing/production
sha2_128s    | ~10-30 sec    | 7,856 bytes    | ⚠️  Very slow, size-critical only
shake128f    | ~1-5 ms       | 17,088 bytes   | ✅ Use for testing/production
shake128s    | ~10-30 sec    | 7,856 bytes    | ⚠️  Very slow, size-critical only
sha2_192f    | ~2-10 ms      | 35,664 bytes   | ✅ Use for testing/production
sha2_192s    | ~30-60 sec    | 16,976 bytes   | ⚠️  Extremely slow
shake192f    | ~2-10 ms      | 35,664 bytes   | ✅ Use for testing/production
shake192s    | ~30-60 sec    | 16,224 bytes   | ⚠️  Extremely slow
sha2_256f    | ~5-15 ms      | 49,856 bytes   | ✅ Use for testing/production
sha2_256s    | ~60+ sec      | 29,792 bytes   | ❌ Painfully slow
shake256f    | ~5-15 ms      | 49,856 bytes   | ✅ Use for testing/production
shake256s    | ~60+ sec      | 29,792 bytes   | ❌ Painfully slow
```

### SHA2 vs SHAKE

Both hash function families provide equivalent security:

- **SHA2**: Uses SHA-256/SHA-512 (traditional, widely deployed)
- **SHAKE**: Uses SHAKE256 (flexible output length, newer)

Performance is similar between SHA2 and SHAKE variants at the same security level.

### Why Are "s" Variants So Slow?

The "s" (small) variants achieve smaller signatures through:

1. **More hash tree levels** - Deeper FORS trees require more hashing
2. **Longer WOTS+ chains** - More iterations per signature component
3. **More cryptographic operations** - Exponentially more hash computations
4. **Trade-off design** - Intentional: smaller size = more computation

### Recommendations

**For Development/Testing:**
```bash
# Only test fast variants
make run-fast

# Or test specific hash function family
make run-sha2      # Test all SHA2 variants
make run-shake     # Test all SHAKE variants
```

**For Production Use:**
- Use **"f" variants** unless signature size is critical
- Choose **SHA2** for traditional deployments, **SHAKE** for flexibility
- If you must use "s" variants, compile with optimizations:
  ```bash
  make BUILD_MODE=release run-small
  ```

**Timeout Expectations:**
- **Debug mode**: "s" variants can take 30-120 seconds per signature
- **Release mode**: "s" variants take 5-30 seconds per signature
- **"f" variants**: Milliseconds in both debug and release modes

## Usage Examples

### Run Specific Variants

```bash
# Fast SHA2 variant (completes in milliseconds)
make run-_sha2_128f

# Fast SHAKE variant (completes in milliseconds)
make run-_shake128f

# Small SHA2 variant (takes 10-30 seconds in debug mode)
make run-_sha2_128s

# With custom message
make run-_sha2_128f-message
```

### Run By Category

```bash
# Run all fast variants (both SHA2 and SHAKE)
make run-fast

# Run all SHA2 variants (both fast and slow)
make run-sha2

# Run all SHAKE variants (both fast and slow)
make run-shake

# Run all small variants (very slow!)
make BUILD_MODE=release run-small
```

### Example Output

```
Message: Hello world!
Method: SLH-DSA-SHA2-128f

Private (Signing) key length: 64 bytes
Public (Verify) key length: 32 bytes
Signature length: 17088 bytes
Private key (hex, truncated): a7b3c2d1e4f5...
Public key (hex, truncated): 3c4d5e6f7a8b...
Signature (hex, truncated): 9e8d7c6b5a4f...

Signature verified
```

## Project Structure

```
.
├── Cargo.toml              # Project dependencies
├── Makefile                # Build and run automation
├── README.md               # This file
├── src/
│   └── main.rs             # Main entry point (swapped during build)
└── lib/
    ├── slh_dsa_sha2_128f.rs    # SHA2, 128-bit, fast
    ├── slh_dsa_sha2_128s.rs    # SHA2, 128-bit, small
    ├── slh_dsa_sha2_192f.rs    # SHA2, 192-bit, fast
    ├── slh_dsa_sha2_192s.rs    # SHA2, 192-bit, small
    ├── slh_dsa_sha2_256f.rs    # SHA2, 256-bit, fast
    ├── slh_dsa_sha2_256s.rs    # SHA2, 256-bit, small
    ├── slh_dsa_shake128f.rs    # SHAKE, 128-bit, fast
    ├── slh_dsa_shake128s.rs    # SHAKE, 128-bit, small
    ├── slh_dsa_shake192f.rs    # SHAKE, 192-bit, fast
    ├── slh_dsa_shake192s.rs    # SHAKE, 192-bit, small
    ├── slh_dsa_shake256f.rs    # SHAKE, 256-bit, fast
    └── slh_dsa_shake256s.rs    # SHAKE, 256-bit, small
```

## Makefile Targets

```bash
make help           # Show all available targets
make build          # Build current version
make run-<variant>  # Run specific variant
make run-all        # Run all variants (WARNING: slow!)
make run-fast       # Run all fast variants ⚡
make run-small      # Run all small variants 🐌 SLOW!
make run-sha2       # Run all SHA2 variants
make run-shake      # Run all SHAKE variants
make list           # List available variants
make clean          # Clean and restore original state
make check          # Check code without building
make fmt            # Format code
make status         # Show current project status
make benchmark      # Benchmark all variants
```

## Code Example

```rust
use slh_dsa::*;
use signature::*;
use rand::thread_rng;

fn main() {
    let msg = b"Hello, post-quantum world!";
    let mut rng = rand::thread_rng();
    
    // Generate keys (SHA2-128f variant)
    let sign_key = SigningKey::<Sha2_128f>::new(&mut rng);
    let verify_key = sign_key.verifying_key();
    
    // Sign the message
    let signature = sign_key.sign_with_rng(&mut rng, msg);
    
    // Verify
    let verified = verify_key.verify(msg, &signature);
    assert!(verified.is_ok());
    
    println!("Signature verified: {}", verified.is_ok());
}
```

## API Reference (slh-dsa v0.1.0)

### Key Generation
```rust
use slh_dsa::*;
use rand::thread_rng;

let mut rng = rand::thread_rng();

// SHA2 variants
let signing_key_sha2 = SigningKey::<Sha2_128f>::new(&mut rng);

// SHAKE variants
let signing_key_shake = SigningKey::<Shake128f>::new(&mut rng);

let verifying_key = signing_key.verifying_key();
```

### Signing
```rust
use signature::Signer;

// Randomized signing (recommended)
let signature = signing_key.sign_with_rng(&mut rng, message);

// Deterministic signing (for reproducibility)
let signature = signing_key.sign(message);
```

### Verification
```rust
use signature::Verifier;

// Returns Result<(), Error>
let result = verifying_key.verify(message, &signature);
if result.is_ok() {
    println!("Signature valid!");
}
```

### Serialization
```rust
let sk_bytes = signing_key.to_bytes();
let vk_bytes = verifying_key.to_bytes();
let sig_bytes = signature.to_bytes();
```

### Available Parameter Sets

```rust
// SHA2 variants
use slh_dsa::{Sha2_128f, Sha2_128s, Sha2_192f, Sha2_192s, Sha2_256f, Sha2_256s};

// SHAKE variants
use slh_dsa::{Shake128f, Shake128s, Shake192f, Shake192s, Shake256f, Shake256s};
```

## Security Levels

- **128-bit**: NIST Security Level 1 (equivalent to AES-128)
- **192-bit**: NIST Security Level 3 (equivalent to AES-192)
- **256-bit**: NIST Security Level 5 (equivalent to AES-256)

## When to Use SLH-DSA

**Advantages:**
- ✅ Post-quantum secure
- ✅ No key size constraints
- ✅ Stateless (no key state to manage)
- ✅ Conservative security assumptions (hash-based)
- ✅ Long-term security
- ✅ Choice of SHA2 or SHAKE hash functions

**Disadvantages:**
- ❌ Large signature sizes (7-50 KB)
- ❌ Slow signing (especially "s" variants)
- ❌ Large public operations

**Best Use Cases:**
- Firmware signing
- Software updates
- Certificate authorities
- Long-term archival signatures
- Applications where verification is more frequent than signing
- Ultra-conservative security requirements

## Choosing Between SHA2 and SHAKE

Both provide equivalent security. Choose based on:

**Use SHA2 if:**
- ✅ You need traditional, widely-deployed hash functions
- ✅ Compatibility with existing systems is important
- ✅ You want maximum familiarity and acceptance

**Use SHAKE if:**
- ✅ You want flexibility in output length
- ✅ You're building new systems from scratch
- ✅ You prefer modern XOF (extendable output function) design

In practice, **performance is similar** - choose based on your deployment context.

## Troubleshooting

### Program appears to hang
- **Likely cause**: Running an "s" variant in debug mode
- **Solution**: Use release mode or switch to "f" variants
  ```bash
  cargo build --release
  make BUILD_MODE=release run-small
  ```

### Compilation errors about missing methods
- **Cause**: Wrong slh-dsa version
- **Solution**: Ensure `Cargo.toml` specifies `slh-dsa = "0.1.0"`
  ```bash
  cargo clean
  cargo update
  cargo build
  ```

### Type errors with SigningKey
- **Cause**: Need to specify parameter set
- **Solution**: Use `SigningKey::<Sha2_128f>` or `SigningKey::<Shake128f>`

### Out of memory errors
- **Cause**: Large signature sizes
- **Solution**: This is normal; signatures can be 17-50 KB

## Performance Tips

1. **Always use release mode for "s" variants**:
   ```bash
   cargo build --release
   ```

2. **Start with "f" variants** for testing and development

3. **Choose appropriate security level** - don't use 256-bit if 128-bit suffices

4. **Batch operations** when possible to amortize overhead

5. **Pre-generate keys** offline for better runtime performance

6. **Consider "f" variants** unless signature size is absolutely critical

7. **Use SHA2 or SHAKE** based on your deployment - performance is similar

## References

- [NIST FIPS 205](https://csrc.nist.gov/pubs/fips/205/final)
- [SPHINCS+ Website](https://sphincs.org/)
- [slh-dsa Crate Documentation](https://docs.rs/slh-dsa/)
- [Post-Quantum Cryptography](https://csrc.nist.gov/projects/post-quantum-cryptography)

## License

This example code is provided for educational purposes. Check the `slh-dsa` crate license for production use.

## Contributing

Contributions are welcome! Please ensure:
- Code compiles on stable Rust
- All variants are tested
- Documentation is updated
- Performance characteristics are documented

## Changelog

### Version 0.1.0
- Initial implementation
- Support for all 12 parameter sets (6 SHA2 + 6 SHAKE)
- Makefile automation with category-based running
- Comprehensive documentation
- Performance warnings and optimization guidance

```
warning: unused import: `rand::rngs::ThreadRng`
 --> src/main.rs:3:5
  |
3 | use rand::rngs::ThreadRng;
  |     ^^^^^^^^^^^^^^^^^^^^^
  |
  = note: `#[warn(unused_imports)]` on by default

warning: `slh_dsa` (bin "slh_dsa") generated 1 warning (run `cargo fix --bin "slh_dsa"` to apply 1 suggestion)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 4.37s
     Running `target/debug/slh_dsa`

Message: Hello world!
Method: SLH-DSA-SHAKE-128f

Private (Signing) key length: 64 bytes
Public (Verify) key length: 32 bytes
Signature length: 17088 bytes
Private key (hex, truncated): 42ed0dd92cb5c7ce6cc4a228e77744bf2588578db170136d0fb12ea321f8a8412c698e5761af677ed67c395c4ec227324d21...
Public key (hex, truncated): 2c698e5761af677ed67c395c4ec227324d21574aff804ad0849e6612c5857a35...
Signature (hex, truncated): 66bd2fb8f61a20dfc1cbb443de2d6d3eaa1827b43e465573942c00b8958b08626c1d5dded8d54488940c6bb96d95c83bccc4...

Signature verified
⏱️  Time for _shake128f: 9 seconds

========================================
Running SLH-DSA version: _shake128s
========================================
   Compiling slh_dsa v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust/slh-dsa_02)
warning: unused import: `rand::rngs::ThreadRng`
 --> src/main.rs:3:5
  |
3 | use rand::rngs::ThreadRng;
  |     ^^^^^^^^^^^^^^^^^^^^^
  |
  = note: `#[warn(unused_imports)]` on by default

warning: `slh_dsa` (bin "slh_dsa") generated 1 warning (run `cargo fix --bin "slh_dsa"` to apply 1 suggestion)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.29s
     Running `target/debug/slh_dsa`

Message: Hello world!
Method: SLH-DSA-SHAKE-128s

Private (Signing) key length: 64 bytes
Public (Verify) key length: 32 bytes
Signature length: 7856 bytes
Private key (hex, truncated): ab72cd75fccc54c832ec2bea850cd06ec5d9ee53748a3f518672434204e8e7972c52d1f350cf6660fd36307613bfeea2acde...
Public key (hex, truncated): 2c52d1f350cf6660fd36307613bfeea2acdec80facff976e2d62694a2414d823...
Signature (hex, truncated): 290828b6c925976d86c0740361b0027ddfdf21e9349b6085ac07a19fc23fdc7a9cfc536e53b1905bd0cd78b20d43ffc8e03d...

Signature verified
⏱️  Time for _shake128s: 98 seconds

========================================
Running SLH-DSA version: _shake192f
========================================
   Compiling slh_dsa v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust/slh-dsa_02)
warning: unused import: `rand::thread_rng`
 --> src/main.rs:3:5
  |
3 | use rand::thread_rng;
  |     ^^^^^^^^^^^^^^^^
  |
  = note: `#[warn(unused_imports)]` on by default

warning: `slh_dsa` (bin "slh_dsa") generated 1 warning (run `cargo fix --bin "slh_dsa"` to apply 1 suggestion)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 1.18s
     Running `target/debug/slh_dsa`

Message: Hello world!
Method: SLH-DSA-SHAKE-192f

Private (Signing) key length: 96 bytes
Public (Verify) key length: 48 bytes
Signature length: 35664 bytes
Private key (hex, truncated): d3985ea207c6025c7f91d292cc34ffaff907aa4a93e18a37eb05a2238227ad55d2b80fa7f827b3a3f3b96e400d69ca249473...
Public key (hex, truncated): 9473bc2d6e874c2f860993ef1ffbecce8dc294d612711143cb65d077ddcdd7382f0212e2c2d69585e473189858ecf1f1...
Signature (hex, truncated): 1e6f0d299b5ae84da02cb5e35a0e17c915efbcf277ea9275ac5688d63536e4be0ef403dad57b0d3e955fba2c98f678fd719e...

Signature verified
⏱️  Time for _shake192f: 9 seconds

========================================
Running SLH-DSA version: _shake192s
========================================
   Compiling slh_dsa v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust/slh-dsa_02)
warning: unused import: `rand::thread_rng`
 --> src/main.rs:3:5
  |
3 | use rand::thread_rng;
  |     ^^^^^^^^^^^^^^^^
  |
  = note: `#[warn(unused_imports)]` on by default

warning: `slh_dsa` (bin "slh_dsa") generated 1 warning (run `cargo fix --bin "slh_dsa"` to apply 1 suggestion)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.31s
     Running `target/debug/slh_dsa`

Message: Hello world!
Method: SLH-DSA-SHAKE-192s

Private (Signing) key length: 96 bytes
Public (Verify) key length: 48 bytes
Signature length: 16224 bytes
Private key (hex, truncated): ffc8cb51e78568564772d9bade15886725bb3d35d00d923ad9de983e25f21c567af1b97210a3b9067ed9f2d90d6617ff5a00...
Public key (hex, truncated): 5a004b273589dc0be67c011b52893f9296eabc883474ae79ebd1f9a5eec1584b348141d5220a548383719a957bf0e0c4...
Signature (hex, truncated): 574350d433134929c5ecd19c907e683f158b542ba17c59d68aaa3320f7b0998f7ad73489ad434a09bc57273a631198314ca0...

Signature verified
⏱️  Time for _shake192s: 165 seconds

========================================
Running SLH-DSA version: _shake256f
========================================
   Compiling slh_dsa v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust/slh-dsa_02)
warning: unused import: `rand::thread_rng`
 --> src/main.rs:3:5
  |
3 | use rand::thread_rng;
  |     ^^^^^^^^^^^^^^^^
  |
  = note: `#[warn(unused_imports)]` on by default

warning: `slh_dsa` (bin "slh_dsa") generated 1 warning (run `cargo fix --bin "slh_dsa"` to apply 1 suggestion)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 1.20s
     Running `target/debug/slh_dsa`

Message: Hello world!
Method: SLH-DSA-SHAKE-256f

Private (Signing) key length: 128 bytes
Public (Verify) key length: 64 bytes
Signature length: 49856 bytes
Private key (hex, truncated): 3470244ba88b93f76bcb366fa6c73ba9684e2494baab1e2a31d6e3dad0d6cb8ba2f04b7702a663b635481446650e778ad747...
Public key (hex, truncated): 7bf51b12f01cff843bfbd9e3caad74cb8d9ff1e739eca4011f7886b49b941ce17d182bdec5573c17598dd6858bdf2df3d6ac...
Signature (hex, truncated): 6d0a998979f950ca621b3e1978ecf85faebee9eb01258bb6cc3f1527e23eb35acfafc22ee1e2503cdfc44a5ae624b6e953b7...

Signature verified
⏱️  Time for _shake256f: 16 seconds

========================================
Running SLH-DSA version: _shake256s
========================================
   Compiling slh_dsa v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust/slh-dsa_02)
warning: unused import: `rand::thread_rng`
 --> src/main.rs:3:5
  |
3 | use rand::thread_rng;
  |     ^^^^^^^^^^^^^^^^
  |
  = note: `#[warn(unused_imports)]` on by default

warning: `slh_dsa` (bin "slh_dsa") generated 1 warning (run `cargo fix --bin "slh_dsa"` to apply 1 suggestion)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 1.00s
     Running `target/debug/slh_dsa`

Message: Hello world!
Method: SLH-DSA-SHAKE-256s

Private (Signing) key length: 128 bytes
Public (Verify) key length: 64 bytes
Signature length: 29792 bytes
Private key (hex, truncated): ff25c2a4842fcf3946ff460f62f351ce2e3ca33288489a590abab8c9f2db69a3be78860357bc3a6c0aebeb5e4435ef024689...
Public key (hex, truncated): 6517bfed84726d9a03a88e3e86f02f324b4543f566009455f77c88f0ad0d7ec2543c97bc6ca8b420966cb357c69503a2748d...
Signature (hex, truncated): 35aac918c081a158a144d2dad2bb08c1e3a96bec7a351464b7d8685a3198268e30fa1f163abc07910cd41e59552f33437096...

Signature verified
⏱️  Time for _shake256s: 143 seconds

========================================
✅ All tests completed!
⏱️  Total execution time: 7m 21s (441 seconds)
========================================
Restored original main.rs
```