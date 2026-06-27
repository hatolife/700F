# Codec2/FreeDV verification notes

Bootstrap M0 document for FreeDV 700F lab. See `codex_freedv700f_instruction.md` for the controlling v6 requirements.

Verified starting points on 2026-06-27: Codec2 hosts the FreeDV API, HF OFDM/FSK modem and FEC code, and SM1000 FreeDV 1600/700D/700E support in the upstream README; FreeDV documentation describes the API/library role. ONNX Runtime dependency docs recommend vcpkg as one supported C++ dependency path. Detailed constants require pinning `external/codec2` in a later issue before treating numeric 700E values as authoritative.
