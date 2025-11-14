use ml_dsa::{MlDsa65, KeyGen, signature::{Signer, Verifier}};
use rand::thread_rng;
use std::env;

fn main() {
    let mut rng = thread_rng();
    let mut msg = String::from("Hello world!");
    let method = String::from("ML-DSA-65");

    let args: Vec<String> = env::args().collect();
    if args.len() > 1 { msg = args[1].clone(); }

    let kp = MlDsa65::key_gen(&mut rng);
    let sig = kp.signing_key().sign(msg.as_bytes());

    let sign_key = kp.signing_key().encode();
    let verify_key = kp.verifying_key().encode();
    let signature = sig.encode();

    println!("\nMessage: {}\nMethod: {}\n", msg, method);
    println!("Private (Signing) key: Length: {} bytes, Key (truncated): {}", sign_key.len(), &hex::encode(&sign_key)[0..200]);
    println!("Public (Verify) key: Length: {} bytes, Key (truncated): {}", verify_key.len(), &hex::encode(&verify_key)[0..200]);
    println!("Signature: Length: {} bytes, Signature (truncated): {}", signature.len(), &hex::encode(&signature)[0..200]);

    let ver = kp.verifying_key().verify(msg.as_bytes(), &sig);
    if ver.is_ok() { println!("\nSignature verified"); }
    else { println!("\nSignature NOT verified"); }
}