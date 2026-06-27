#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parents[1]
errors = []

if root.joinpath("VERSION").read_text().strip() != "v0.0.1":
    errors.append("project VERSION mismatch")

for path in root.glob("modules/*/VERSION"):
    if path.read_text().strip() != "v0.0.1":
        errors.append(f"{path.relative_to(root)} mismatch")

manifest = root.joinpath("versions.toml").read_text()
for module_dir in root.glob("modules/*"):
    key = module_dir.name[3:]
    if f'{key} = "v0.0.1"' not in manifest:
        errors.append(f"missing {key} in versions.toml")

if errors:
    print("\n".join(errors))
    sys.exit(1)

print("version check passed")
