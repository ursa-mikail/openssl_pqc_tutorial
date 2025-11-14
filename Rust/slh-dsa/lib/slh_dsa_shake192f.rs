use fips205::slh_dsa_shake_192f;
use fips205::traits::{SerDes, Signer, Verifier};
use std::env;

fn main() {
    let mut msg = String::from("Hello world!");
    let method = String::from("SLH-DSA-SHAKE-192f");

    let args: Vec<String> = env::args().collect();
    if args.len() > 1 { msg = args[1].clone(); }

    let msg_bytes = msg.as_bytes();

    let (pk1, sk) = match slh_dsa_shake_192f::try_keygen() {
        Ok(keys) => keys,
        Err(e) => {
            println!("Failed to generate keys: {}", e);
            return;
        }
    };

    let sig_bytes = match sk.try_sign(&msg_bytes, b"", true) {
        Ok(sig) => sig,
        Err(e) => {
            println!("Failed to sign: {}", e);
            return;
        }
    };

    let pk_send = pk1.into_bytes();
    let sk_send = sk.into_bytes();
    let msg_send = msg_bytes;
    let sig_send = sig_bytes;

    let pk2 = match slh_dsa_shake_192f::PublicKey::try_from_bytes(&pk_send) {
        Ok(pk) => pk,
        Err(e) => {
            println!("Failed to deserialize public key: {}", e);
            return;
        }
    };

    let verified = pk2.verify(&msg_send, &sig_send, b"");

    println!("\nMessage: {}\nMethod: {}\n", msg, method);
    println!("Private (Secret) key length: {} bytes", sk_send.len());
    println!("Public key length: {} bytes", pk_send.len());
    println!("Signature length: {} bytes", sig_send.len());
    println!("Private key (hex, truncated): {}...", &hex::encode(&sk_send)[0..100.min(hex::encode(&sk_send).len())]);
    println!("Public key (hex, truncated): {}...", &hex::encode(&pk_send)[0..100.min(hex::encode(&pk_send).len())]);
    println!("Signature (hex, truncated): {}...", &hex::encode(&sig_send)[0..100.min(hex::encode(&sig_send).len())]);

    if verified {
        println!("\nSignature verified");
    } else {
        println!("\nSignature NOT verified");
    }
}