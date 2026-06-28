# Handoff: ISSUE-0042 700F-A Minimal QPSK Baseband Prototype

- Branch: `feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype`
- Worktree: `C:/Users/user/Documents/700F-ISSUE-0042`
- Scope: Move only `freedv700f_a_balanced` from toy waveform prototype to
  minimal QPSK-like baseband prototype.

## Implementation

- 700F-A now uses `implementation_status=real_modem_prototype`.
- The prototype helper path creates ISSUE-0043 `ModemInputFrame`,
  `SymbolBlock`, `BasebandFrame`, and `ModemOutputFrame` values.
- Sweep/report metadata keeps `prototype=true`, `not_final_modem=true`,
  `waveform_capable=true`, `performance_validity=limited`,
  `downselect_valid=false`, `downselect_validity=invalid`,
  `codec_family=synthetic`, `fec_family=none`, `sync_family=none`, and
  `modem_family=minimal_qpsk`.
- 700F-B/C remain ISSUE-0032 surrogate rows.

## Guardrails

No final Codec2 payload, FEC, OFDM, interleaving, synchronization, or real
performance-valid downselect evidence is implemented. BER/FER remain non-real
performance evidence for this prototype. Real 700F downselect remains
prohibited.

## Verification

Focused TDD tests for candidate profiles, sweep artifacts, metrics artifacts,
scoring, baseline report rendering, and report artifact loading pass locally.
M2-H smoke/report passed with 21 completed rows, 6 Codec2-OFF official skips,
and 0 failures; the generated report warned that 700F-A is a real modem
prototype with `performance_valid=limited`, `sync_family=none`,
`modem_family=minimal_qpsk`, and real downselect prohibited.

`bash ./tools/run_ci_local.sh` passed with 21/21 CTest, version check, and
governance check. `bash ./tools/run_codec2_on_ci_local.sh` passed with 21/21
Codec2 ON CTest, 27/27 smoke rows, and 6 validated official FreeDV rows. The
Codec2 ON invocation used explicit `GIT_DIR`/`GIT_WORK_TREE` in WSL because this
sibling worktree stores its gitdir pointer in Windows path form.
