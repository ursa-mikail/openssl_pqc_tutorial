# SLH-DSA (SPHINCS+) Rust Implementation

A Rust implementation demonstrating all six parameter sets of SLH-DSA (Stateless Hash-based Digital Signature Algorithm), also known as SPHINCS+, using the `fips205` crate.

## Overview

SLH-DSA is a post-quantum digital signature scheme standardized by NIST as FIPS 205. This implementation demonstrates:

- Key generation
- Message signing with hedging
- Signature verification
- All six SHAKE-based parameter sets (128f, 128s, 192f, 192s, 256f, 256s)

## Requirements

- Rust 1.70 or later
- Cargo

## Dependencies

```toml
[dependencies]
fips205 = "0.4.1"
hex = "0.4.3"
```

## Quick Start

```bash
# Build the project
make build

# Run a specific variant (fast)
make run-_shake128f

# Run all variants (WARNING: slow variants will take minutes!)
make run-all

# Run with release optimizations (recommended for 's' variants)
make BUILD_MODE=release run-all

# Clean build artifacts
make clean
```

## Available Variants

The project includes six SLH-DSA parameter sets:

| Variant | Security Level | Speed | Signature Size | Key Sizes (SK/PK) |
|---------|---------------|-------|----------------|-------------------|
| **shake128f** | 128-bit | Fast (~ms) | ~17 KB | 64 / 32 bytes |
| **shake128s** | 128-bit | Slow (10-30s) | ~7 KB | 64 / 32 bytes |
| **shake192f** | 192-bit | Fast (~ms) | ~35 KB | 96 / 48 bytes |
| **shake192s** | 192-bit | Slow (30-60s) | ~17 KB | 96 / 48 bytes |
| **shake256f** | 256-bit | Fast (~ms) | ~49 KB | 128 / 64 bytes |
| **shake256s** | 256-bit | Very Slow (60s+) | ~29 KB | 128 / 64 bytes |

## ⚠️ IMPORTANT PERFORMANCE CAVEATS

### Speed vs Size Trade-off

SLH-DSA offers two variants for each security level:

- **"f" (fast) variants**: Optimized for speed, larger signatures
- **"s" (small) variants**: Optimized for size, MUCH slower

### Performance Differences

```
Variant    | Signing Time  | Signature Size | Recommendation
-----------|---------------|----------------|------------------
-128f      | ~1-5 ms       | 17,088 bytes   | ✅ Use for testing/production
-128s      | ~10-30 sec    | 7,856 bytes    | ⚠️  Very slow, size-critical only
-192f      | ~2-10 ms      | 35,664 bytes   | ✅ Use for testing/production
-192s      | ~30-60 sec    | 16,976 bytes   | ⚠️  Extremely slow
-256f      | ~5-15 ms      | 49,856 bytes   | ✅ Use for testing/production
-256s      | ~60+ sec      | 29,792 bytes   | ❌ Painfully slow
```

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
make run-_shake128f
make run-_shake192f
make run-_shake256f
```

**For Production Use:**
- Use **"f" variants** unless signature size is critical
- If you must use "s" variants, compile with optimizations:
  ```bash
  make BUILD_MODE=release run-_shake128s
  ```

**Timeout Expectations:**
- **Debug mode**: "s" variants can take 30-120 seconds per signature
- **Release mode**: "s" variants take 5-30 seconds per signature
- **"f" variants**: Milliseconds in both debug and release modes

## Usage Examples

### Run a Specific Variant

```bash
# Fast variant (completes in milliseconds)
make run-_shake128f

# Small variant (takes 10-30 seconds in debug mode)
make run-_shake128s

# With custom message
make run-_shake128f-message
# Then edit the Makefile to customize the message
```

### Example Output

```
Message: Hello world!
Method: SLH-DSA-SHAKE-128f

Private (Secret) key length: 64 bytes
Public key length: 32 bytes
Signature length: 17088 bytes
Private key (hex, truncated): cca27dc2eaef...
Public key (hex, truncated): d2ae2806abf2...
Signature (hex, truncated): bc8dd44f0d38...

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
    ├── slh_dsa_shake128f.rs
    ├── slh_dsa_shake128s.rs
    ├── slh_dsa_shake192f.rs
    ├── slh_dsa_shake192s.rs
    ├── slh_dsa_shake256f.rs
    └── slh_dsa_shake256s.rs
```

## Makefile Targets

```bash
make help           # Show all available targets
make build          # Build current version
make run-<variant>  # Run specific variant
make run-all        # Run all variants (WARNING: slow!)
make list           # List available variants
make clean          # Clean and restore original state
make check          # Check code without building
make fmt            # Format code
make status         # Show current project status
```

## Code Example

```rust
use fips205::slh_dsa_shake_128f;
use fips205::traits::{SerDes, Signer, Verifier};

fn main() {
    let msg = b"Hello, post-quantum world!";
    
    // Generate keys
    let (pk, sk) = slh_dsa_shake_128f::try_keygen()
        .expect("Key generation failed");
    
    // Sign with empty context and hedging enabled
    let signature = sk.try_sign(msg, b"", true)
        .expect("Signing failed");
    
    // Verify
    let verified = pk.verify(msg, &signature, b"");
    assert!(verified);
    
    println!("Signature verified: {}", verified);
}
```

## API Reference (fips205 v0.4.1)

### Key Generation
```rust
let (public_key, secret_key) = slh_dsa_shake_128f::try_keygen()?;
```

### Signing
```rust
// Parameters: message, context, hedge
let signature = secret_key.try_sign(message, b"", true)?;
```

### Verification
```rust
// Returns: bool
let verified = public_key.verify(message, &signature, b"");
```

### Serialization
```rust
let pk_bytes = public_key.into_bytes();
let sk_bytes = secret_key.into_bytes();
let pk_restored = PublicKey::try_from_bytes(&pk_bytes)?;
```

## Security Levels

- **SHAKE-128**: NIST Security Level 1 (equivalent to AES-128)
- **SHAKE-192**: NIST Security Level 3 (equivalent to AES-192)
- **SHAKE-256**: NIST Security Level 5 (equivalent to AES-256)

## When to Use SLH-DSA

**Advantages:**
- ✅ Post-quantum secure
- ✅ No key size constraints
- ✅ Stateless (no key state to manage)
- ✅ Conservative security assumptions
- ✅ Long-term security

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

## Troubleshooting

### Program appears to hang
- **Likely cause**: Running an "s" variant in debug mode
- **Solution**: Use release mode or switch to "f" variants
  ```bash
  cargo build --release
  make BUILD_MODE=release run-_shake128s
  ```

### Compilation errors about missing methods
- **Cause**: Wrong fips205 version
- **Solution**: Ensure `Cargo.toml` specifies `fips205 = "0.4.1"`
  ```bash
  cargo clean
  cargo update
  cargo build
  ```

### Out of memory errors
- **Cause**: Large signature sizes
- **Solution**: This is normal; signatures can be 17-50 KB

## Performance Tips

1. **Always use release mode for "s" variants**:
   ```bash
   cargo build --release
   ```

2. **Batch operations** when possible to amortize overhead

3. **Consider "f" variants** unless signature size is critical

4. **Pre-generate keys** offline for better runtime performance

5. **Use appropriate security level** - don't use 256-bit if 128-bit suffices

## References

- [NIST FIPS 205](https://csrc.nist.gov/pubs/fips/205/final)
- [SPHINCS+ Website](https://sphincs.org/)
- [fips205 Crate Documentation](https://docs.rs/fips205/)
- [Post-Quantum Cryptography](https://csrc.nist.gov/projects/post-quantum-cryptography)

## License

This example code is provided for educational purposes. Check the `fips205` crate license for production use.

## Contributing

Contributions are welcome! Please ensure:
- Code compiles on stable Rust
- All variants are tested
- Documentation is updated
- Performance characteristics are documented

## Changelog

### Version 0.1.0
- Initial implementation
- Support for all six SHAKE-based parameter sets
- Makefile automation
- Performance documentation and warnings


```
% make clean
Restored original main.rs
Cleaning build artifacts...
cargo clean
     Removed 403 files, 66.6MiB total
(base) chanfamily@Chans-MacBook-Air slh-dsa % make run-all
Backed up original main.rs to main.rs.orig

🚀 Starting SLH-DSA test suite...
⚠️  WARNING: 's' variants may take 10-60+ seconds each!

========================================
Running SLH-DSA version: _shake128f
========================================
   :

Message: Hello world!
Method: SLH-DSA-SHAKE-128f

Private (Secret) key length: 64 bytes
Public key length: 32 bytes
Signature length: 17088 bytes
Private key (hex, truncated): 20252ed4e3789cce5f17eea2db16f592dbe2527a1edaf2c1cf3ed42ad78b9ab66888255090fa79273e79596a17a81240259e...
Public key (hex, truncated): 6888255090fa79273e79596a17a81240259ef09425db150418bc9772e1abe825...
Signature (hex, truncated): 5195b03d6abc932fdf9994441f023b10d61e452f444bd8e183c37dfcd348a29d34902f177fa14d11f0b0581d008afcaf43eb...

Signature verified
⏱️  Time for _shake128f: 9 seconds

========================================
Running SLH-DSA version: _shake128s
========================================

Message: Hello world!
Method: SLH-DSA-SHAKE-128s

Private (Secret) key: Length: 64 bytes
Public key: Length: 32 bytes
Signature: Length: 7856 bytes
Private key (hex, truncated): b7f4d1e2823700beeb5b8317b0d5c1080a6a4e7096e911e825694f5aaefd5fffc875ee418e66d458cd44c6a0775193513d6b...
Public key (hex, truncated): c875ee418e66d458cd44c6a0775193513d6be5474a056f5112a32a51989abdd2...
Signature (hex, truncated): deebb7c67afa41d6aa3ce8b609727f7fd51e8ffa7a58e44358332dd970adf1082fccb36d7382b9068423426a320cddffe96a...

Signature verified
⏱️  Time for _shake128s: 98 seconds

========================================
Running SLH-DSA version: _shake192f
========================================

Message: Hello world!
Method: SLH-DSA-SHAKE-192f

Private (Secret) key length: 96 bytes
Public key length: 48 bytes
Signature length: 35664 bytes
Private key (hex, truncated): 7eb8e12a06a0ece78791d26a2310e11427910319f0400a6816821a2c9d959ae387939597d3aba3be3ba9bb86b449767b4a24...
Public key (hex, truncated): 4a24050b426a4ea272fc2c78c7e262bf06bed9ed6c6e79aea2ab6698baaaea8254f09f4bd9e12779e71108d9162ccfee...
Signature (hex, truncated): 2d98a1ca619df50685ac8d92867aad63268fdbbff616a05d8bfc9d5b95f3fba3f9314c2d65e7b92a66db540c5910473f32ff...

Signature verified
⏱️  Time for _shake192f: 8 seconds

========================================
Running SLH-DSA version: _shake192s
========================================
Message: Hello world!
Method: SLH-DSA-SHAKE-192s

Private (Secret) key length: 96 bytes
Public key length: 48 bytes
Signature length: 16224 bytes
Private key (hex, truncated): 0c10958f4c8a853e300ddbb47ecdc63ff04e093d241ccb5425b0ad9423abd4ad3c55d1c756d5e030670cfc50f5e5606d6cd1...
Public key (hex, truncated): 6cd1332d33be8cf5cceeea4d43a689f5d8d575bf7f3a01351ec20ed764d5a009222e0ed51678a41190995b0b77b36c7e...
Signature (hex, truncated): 9853f62228a3004a4912b9daa2930de13c53efa967a52b7e24fe0e209e09f1a7a137b8ffc2e4d5b5d0c154128a835b732885...

Signature verified
⏱️  Time for _shake192s: 169 seconds

========================================
Running SLH-DSA version: _shake256f
========================================
Message: Hello world!
Method: SLH-DSA-SHAKE-256f

Private (Secret) key length: 128 bytes
Public key length: 64 bytes
Signature length: 49856 bytes
Private key (hex, truncated): 53772ca6c011191ca250350ca0684698a3d86075d4156e4b4e1f872ddd3d135a8ae9c71d8e8d06cc521e3c5abd8df24b26e0...
Public key (hex, truncated): 4b3c296d7400af84e2aee47e53d046fff6f519c97314e92799ace4e0647312faf668845fddd9cf5bf67302a2290eed4b4a14...
Signature (hex, truncated): 172c85a71fef4c33c42ff296385f647056c518afbbc7dbbe4cd118db90d0a1d4f60aeaccc80f4be4d1f7951c40b1d98bcd20...

Signature verified
⏱️  Time for _shake256f: 16 seconds

========================================
Running SLH-DSA version: _shake256s
========================================

Message: Hello world!
Method: SLH-DSA-SHAKE-256s

Private (Secret) key length: 128 bytes
Public key length: 64 bytes
Signature length: 29792 bytes
Private key (hex, truncated): c27b10af54f3ec77f66acafeae3d7c4eb8d8862a40d6091f99a2be5304136f1f18021ec3962389d0a2b1433b939f51ce8c69...
Public key (hex, truncated): 538091714fce4395ee4941c2cb93fa26a0ad6623c62a56bc51e07e25471160a0ae7ff988e1046ba598b9aaf4e495aa082424...
Signature (hex, truncated): a85a435d1b782175b5cee2c56c99dc49959d5cee3fcbb03f9c986cace67c64c47a4103002ffe06c56cbaac0c31c5a20d9ff1...

Signature verified
⏱️  Time for _shake256s: 141 seconds

========================================
✅ All tests completed!
⏱️  Total execution time: 7m 21s (441 seconds)
========================================
Restored original main.rs


```