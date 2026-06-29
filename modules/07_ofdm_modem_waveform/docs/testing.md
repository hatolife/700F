# Testing

Module 07 adds `ofdm_carrier_layout_contract` for ISSUE-0046. The test covers:

- prototype layout metadata for carrier count, 100 Hz spacing, 1.9 kHz occupied
  bandwidth target, data/pilot/guard counts, and placeholder families;
- invalid carrier spacing, invalid occupied-bandwidth target, zero-carrier
  layouts, and mismatched data/pilot/guard totals;
- empty `BasebandFrame` placeholder metadata propagation; and
- preservation of the ISSUE-0042 minimal QPSK runtime metadata.

Run the module contract through the repository build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target test_ofdm_carrier_layout
ctest --test-dir build -R ofdm_carrier_layout_contract --output-on-failure
```
