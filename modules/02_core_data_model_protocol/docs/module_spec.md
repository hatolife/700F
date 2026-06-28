# Module Spec

Module 02 owns source-level C++20 data model and protocol value types used by
the local simulation code.

Current public surfaces:

- v0.1.0 Mode data blocks and descriptors in `include/f700f/types.hpp`.
- ISSUE-0043 modem frame, symbol, baseband, prototype status, rate, and
  placeholder family metadata containers in `include/f700f/types.hpp`.

The module does not implement concrete modes, modem waveforms, channel models,
metrics scoring, report rendering, plugin loading, Codec2 adapters, FEC, or
synchronization algorithms.
