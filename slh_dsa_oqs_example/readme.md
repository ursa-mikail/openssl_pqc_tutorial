```
# Build the fixed demo
make

# Run with default algorithm (ML-DSA-44)
make run

# Or run specific algorithms
./pq_sig_demo ML-DSA-65
./pq_sig_demo Falcon-512
./pq_sig_demo SPHINCS+-SHA2-128f-simple

# Test all available algorithms
make test
```