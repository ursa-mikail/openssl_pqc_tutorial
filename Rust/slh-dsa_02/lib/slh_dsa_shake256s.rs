use slh_dsa::*;
use signature::*;
use rand::thread_rng;
use std::env;

fn main() {
    let mut msg = String::from("Hello world!");
    let method = String::from("SLH-DSA-SHAKE-256s");

    let args: Vec<String> = env::args().collect();
    if args.len() > 1 { msg = args[1].clone(); }

    let mut rng = rand::thread_rng();

    // Generate keys
    let sign_key = SigningKey::<Shake256s>::new(&mut rng);
    let verify_key = sign_key.verifying_key();

    let vk_bytes = verify_key.to_bytes();
    let sk_bytes = sign_key.to_bytes();

    // Sign the message
    let message = msg.as_bytes();
    let signature = sign_key.sign_with_rng(&mut rng, message);

    let sig_bytes = signature.to_bytes();

    // Verify signature
    let ver = verify_key.verify(message, &signature);

    println!("\nMessage: {}\nMethod: {}\n", msg, method);
    println!("Private (Signing) key length: {} bytes", sk_bytes.len());
    println!("Public (Verify) key length: {} bytes", vk_bytes.len());
    println!("Signature length: {} bytes", sig_bytes.len());
    println!("Private key (hex, truncated): {}...", &hex::encode(&sk_bytes)[0..100.min(hex::encode(&sk_bytes).len())]);
    println!("Public key (hex, truncated): {}...", &hex::encode(&vk_bytes)[0..100.min(hex::encode(&vk_bytes).len())]);
    println!("Signature (hex, truncated): {}...", &hex::encode(&sig_bytes)[0..100.min(hex::encode(&sig_bytes).len())]);

    if ver.is_ok() {
        println!("\nSignature verified");
    } else {
        println!("\nSignature NOT verified");
    }
}