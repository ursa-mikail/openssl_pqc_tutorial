
## Install:
### Update packages:
```
brew install cmake
brew install libssl-dev 
brew install ninja
brew install libssolv
```

### Install oqs-provider:
```
git clone https://github.com/open-quantum-safe/oqs-provider.git
cd oqs-provider
./scripts/fullbuild.sh
sudo cmake --install build
```

### Run the tests provided:
```
./scripts/runtests.sh
```

### Edit the openssl configuration to add the provider:
```
sudo vi /etc/ssl/openssl.cnf
```

### Add the following lines at the end of the file
```
# PQC via OpenQuantumSafe
[provider_sect]
default = default_sect
oqsprovider = oqsprovider_sect
[default_sect]
activate = 1
[oqsprovider_sect]
activate = 1
```

### Verify that openssl now shows the OQS provider:
```
openssl list -providers
```

```
openssl list -kem-algorithms

or

% openssl list -public-key-algorithms | grep -i kem
Name: OpenSSL ML-KEM-512 implementation
IDs: { 2.16.840.1.101.3.4.4.1, id-alg-ml-kem-512, ML-KEM-512, MLKEM512 } @ default
Name: OpenSSL ML-KEM-768 implementation
IDs: { 2.16.840.1.101.3.4.4.2, id-alg-ml-kem-768, ML-KEM-768, MLKEM768 } @ default
Name: OpenSSL ML-KEM-1024 implementation
IDs: { 2.16.840.1.101.3.4.4.3, id-alg-ml-kem-1024, ML-KEM-1024, MLKEM1024 } @ default
Name: X25519+ML-KEM-768 TLS hybrid implementation
IDs: X25519MLKEM768 @ default
Name: X448+ML-KEM-1024 TLS hybrid implementation
IDs: X448MLKEM1024 @ default
Name: P-256+ML-KEM-768 TLS hybrid implementation
IDs: SecP256r1MLKEM768 @ default
Name: P-384+ML-KEM-1024 TLS hybrid implementation
IDs: SecP384r1MLKEM1024 @ default

```

