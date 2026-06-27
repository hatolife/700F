# Versioning policy

## Project and module versions

- `VERSION` exists at repository root and in each module.
- Project target for M1 is `v0.1.0`.
- Modules join the same compatibility line when they gain interface-bearing M1 work.

## Version transition rules

- Documentation-only updates keep versions unchanged unless scope requires a governance release.
- Implementation or API/contract changes move module `VERSION` to the next milestone line in coordination with milestone targets.
- Public contract changes require documented updates in module and project docs before bumping module `VERSION`.

## Compatibility line

For this repo, M1 work uses compatibility line `1`.
Modules with M1-touched interfaces should align with `v0.1.0` when those interfaces are merged.

## Tracking

- Issue scope and milestone should record the target version in the issue header.
- Any module-local version bump should also be noted in `modules/<NN>_<name>/CHANGELOG.md`.
