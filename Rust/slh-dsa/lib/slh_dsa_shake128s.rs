use fips205::slh_dsa_shake_128s;
use fips205::traits::{SerDes, Signer, Verifier};
use std::env;

fn main() {
    let mut msg = String::from("Hello world!");
    let method = String::from("SLH-DSA-SHAKE-128s");

    let args: Vec<String> = env::args().collect();
    if args.len() > 1 { msg = args[1].clone(); }

    let msg_bytes = msg.as_bytes();

    let (pk, sk) = match slh_dsa_shake_128s::try_keygen() {
        Ok(keys) => keys,
        Err(e) => {
            println!("Failed to generate keys: {}", e);
            return;
        }
    };

    let signature = match sk.try_sign(msg_bytes, b"", true) {
        Ok(sig) => sig,
        Err(e) => {
            println!("Failed to sign: {}", e);
            return;
        }
    };

    let pk_bytes = pk.into_bytes();
    let sk_bytes = sk.into_bytes();
    let sig_bytes = signature;

    println!("\nMessage: {}\nMethod: {}\n", msg, method);
    println!("Private (Secret) key: Length: {} bytes", sk_bytes.len());
    println!("Public key: Length: {} bytes", pk_bytes.len());
    println!("Signature: Length: {} bytes", sig_bytes.len());
    println!("Private key (hex, truncated): {}...", &hex::encode(&sk_bytes)[0..100.min(hex::encode(&sk_bytes).len())]);
    println!("Public key (hex, truncated): {}...", &hex::encode(&pk_bytes)[0..100.min(hex::encode(&pk_bytes).len())]);
    println!("Signature (hex, truncated): {}...", &hex::encode(&sig_bytes)[0..100.min(hex::encode(&sig_bytes).len())]);

    let pk_verifier = match slh_dsa_shake_128s::PublicKey::try_from_bytes(&pk_bytes) {
        Ok(pk) => pk,
        Err(e) => {
            println!("Failed to deserialize public key: {}", e);
            return;
        }
    };

    let verified = pk_verifier.verify(msg_bytes, &sig_bytes, b"");

    if verified {
        println!("\nSignature verified");
    } else {
        println!("\nSignature NOT verified");
    }
}