Usage Examples
```
# Initialize project structure
make init

# List available ML-DSA versions
make list

# Build and run specific version
make run-44
make run-65
make run-87

# Run with custom message
make run-65-message

# Build all versions
make ml_dsa44 ml_dsa65 ml_dsa87

# Run all versions sequentially
make run-all

# Clean build artifacts
make clean
```

```        
.
├── Cargo.lock
├── Cargo.toml
├── lib
│   ├── ml_dsa44.rs
│   ├── ml_dsa65.rs
│   └── ml_dsa87.rs
├── Makefile
├── readme.md
└── src
    └── main.rs


```

```
% make run-all
Backed up original main.rs to main.rs.orig
========================================
Running ML-DSA version: 44
========================================
   Compiling libc v0.2.177
   Compiling typenum v1.19.0
   Compiling version_check v0.9.5
   Compiling cfg-if v1.0.4
   Compiling zerocopy v0.8.27
   Compiling autocfg v1.5.0
   Compiling zeroize v1.8.2
   Compiling const-oid v0.9.6
   Compiling hex v0.4.3
   Compiling rustc-serialize v0.3.25
   Compiling generic-array v0.14.7
   Compiling num-traits v0.2.19
   Compiling der v0.7.10
   Compiling hybrid-array v0.3.1
   Compiling getrandom v0.2.16
   Compiling cpufeatures v0.2.17
   Compiling keccak v0.1.5
   Compiling rand_core v0.6.4
   Compiling spki v0.7.3
   Compiling signature v2.2.0
   Compiling pkcs8 v0.10.2
   Compiling block-buffer v0.10.4
   Compiling crypto-common v0.1.7
   Compiling digest v0.10.7
   Compiling sha3 v0.10.8
   Compiling ml-dsa v0.0.4
   Compiling ppv-lite86 v0.2.21
   Compiling rand_chacha v0.3.1
   Compiling rand v0.8.5
   Compiling mldsa v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 4.21s
     Running `target/debug/mldsa`

Message: Hello world!
Method: ML-DSA-44

Private (Signing) key: Length: 2560 bytes, Key (truncated): bf9937e4bad7041c9d0bd96cb9dcbede77dad1b024110eb9dc9f9d448a8f4c9270d4c2dd19f09253775f2b5088ed8fe304da88d7c177aa94abb59029272c7ec3a71a0a9926a98c679345dcfffca2a9d4545ce0d4e9c3e89abb3afe083c8f57d788e2e638
Public (Verify) key: Length: 1312 bytes, Key (truncated): bf9937e4bad7041c9d0bd96cb9dcbede77dad1b024110eb9dc9f9d448a8f4c92e73a91fdcfc65a9d175e8ce617e6f39beaa3ae17a8145b56aec59c79a8206cb74cd0b70d61097f86eb6603d89456650644af4651669550aa229d81a602fe4d9209e66cac
Signature: Length: 2420 bytes, Signature (truncated): 087861dc1ddcceb4303471afca801d99daff01a3e27e3eda92e3375cd3b4e4a71c97353e57e99756f3e324d8147ce68eb7c5a2625bff8780cbc6e4b368c28871ede16bfd8888bcc20cba717ca4f06b5a9e834d5a358cb8ec2a70574b4ceb8fef21016653

Signature verified

========================================
Running ML-DSA version: 65
========================================
   Compiling mldsa v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.67s
     Running `target/debug/mldsa`

Message: Hello world!
Method: ML-DSA-65

Private (Signing) key: Length: 4032 bytes, Key (truncated): 351442d52eb38fe296e4ebb60eb645ec8d6dda85af1067eebf37bbdc8db616f1503c0251f21dae2d6b035bb4338f9f7701a1d392535f0af6d78508994ae6a63dc3452a21903412b4abf86e79b79b3215d4bf12ca16fc268dcf5487de8a6c803b1c7a7069
Public (Verify) key: Length: 1952 bytes, Key (truncated): 351442d52eb38fe296e4ebb60eb645ec8d6dda85af1067eebf37bbdc8db616f12c8da4db6fba06dab0a0a8d6a5a2b230efc73d4be123d58ac107baca8a51525e1b4e796f2e66afb30999eb9d42714faad61e22ac12aa205726c9b387e839af66929e2875
Signature: Length: 3309 bytes, Signature (truncated): dd8aef9f25c5ed80dd5bf4cf688395e0bf7ea8d61c845ff7dfea74485c3547f86cd1f5cd49731dcc3e9f57a0cd1cbf6803812e7af09671cf5002c1c8a8c66bfc6d267d75f820f9e37f781509a0be4647794cb70178b0e28c33dc274900567c71edbc93b6

Signature verified

========================================
Running ML-DSA version: 87
========================================
   Compiling mldsa v0.1.0 (/Users/chanfamily/ursa/git/openssl_pqc_tutorial/Rust)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.69s
     Running `target/debug/mldsa`

Message: Hello world!
Method: ML-DSA-87

Private (Signing) key: Length: 4896 bytes, Key (truncated): 33cec3050dd33334b74b30d332e6eac2db8a150e4710dc89bb742e0e27640b168418382f5da612573f5e402a56ff845e47e29fa4cfe423ed1eae7cefecd108c86e4ff51782f6a4f31b9b2f7ce19a9cde034a91d29c4dffcd6fcaa37e68f43da9d2b43ee0
Public (Verify) key: Length: 2592 bytes, Key (truncated): 33cec3050dd33334b74b30d332e6eac2db8a150e4710dc89bb742e0e27640b1618c9cec169a1a4907bb446296739a1305c0c99fba1aa87499b14c515b69f0cc4897f0bf32373b879a4ea34688c32edc7961e94b99eee72cd89446df5049cd11340ba9ca6
Signature: Length: 4627 bytes, Signature (truncated): 6743eb26bb10e02666b1fc7b0a0d63a61dd1ba6b6322dcd8cac89af8886da7d4b5467cada1548a7e96a2f871bf4ac9aec4ee578d6f9c9ba0b5c9c29ab9abf86217089309180e980572d5e22be3c36e00ca9172b6b9d1419718710d72246cff0e86e507af

Signature verified

Restored original main.rs
```