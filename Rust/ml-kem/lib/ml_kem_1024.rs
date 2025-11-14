use fips203::ml_kem_1024;
use fips203::traits::{Decaps, Encaps, KeyGen, SerDes};

fn main() {
    let method = String::from("ML-KEM-1024");

    println!("\nMethod: {}\n", method);

    // Alice runs `try_keygen()` and then serializes the encaps key `ek` for Bob (to bytes).
    let (alice_ek, alice_dk) = match ml_kem_1024::KG::try_keygen() {
        Ok(keys) => keys,
        Err(e) => {
            println!("Failed to generate keys: {}", e);
            return;
        }
    };
    let alice_ek_bytes = alice_ek.into_bytes();

    println!("Alice generated keypair:");
    println!("  Encapsulation key length: {} bytes", alice_ek_bytes.len());
    println!("  Encapsulation key (hex, truncated): {}...", &hex::encode(&alice_ek_bytes)[0..100]);

    // Alice sends the encaps key `ek_bytes` to Bob.
    let bob_ek_bytes = alice_ek_bytes;

    // Bob deserializes the encaps `ek_bytes` and then runs `encaps() to get the shared 
    // secret `ssk` and ciphertext `ct`. He serializes the ciphertext `ct` for Alice (to bytes).
    let bob_ek = match ml_kem_1024::EncapsKey::try_from_bytes(bob_ek_bytes) {
        Ok(ek) => ek,
        Err(e) => {
            println!("Bob failed to deserialize encapsulation key: {}", e);
            return;
        }
    };

    let (bob_ssk, bob_ct) = match bob_ek.try_encaps() {
        Ok(result) => result,
        Err(e) => {
            println!("Bob failed to encapsulate: {}", e);
            return;
        }
    };
    let bob_ct_bytes = bob_ct.into_bytes();

    println!("Bob encapsulated shared secret:");
    println!("  Ciphertext length: {} bytes", bob_ct_bytes.len());
    println!("  Ciphertext (hex, truncated): {}...", &hex::encode(&bob_ct_bytes)[0..100]);

    // Bob sends the ciphertext `ct_bytes` to Alice
    let alice_ct_bytes = bob_ct_bytes;

    // Alice deserializes the ciphertext `ct` and runs `decaps()` with her decaps key
    let alice_ct = match ml_kem_1024::CipherText::try_from_bytes(alice_ct_bytes) {
        Ok(ct) => ct,
        Err(e) => {
            println!("Alice failed to deserialize ciphertext: {}", e);
            return;
        }
    };

    let alice_ssk = match alice_dk.try_decaps(&alice_ct) {
        Ok(ssk) => ssk,
        Err(e) => {
            println!("Alice failed to decapsulate: {}", e);
            return;
        }
    };

    // Alice and Bob will now have the same secret key
    if bob_ssk == alice_ssk {
        println!("\n✅ Key exchange successful! Both parties have the same shared secret.");
    } else {
        println!("\n❌ Key exchange failed! Shared secrets do not match.");
    }
}