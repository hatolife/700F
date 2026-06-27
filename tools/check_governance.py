#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parents[1]
required = [
    "AGENTS.md",
    "issues/README.md",
    "reports/README.md",
    "docs/protocol.md",
    "docs/module_contracts.md",
    ".github/workflows/ci.yml",
    ".github/workflows/governance.yml",
    ".github/workflows/security.yml",
]
errors = [path for path in required if not root.joinpath(path).exists()]

for module_dir in root.glob("modules/*"):
    for rel in [
        "AGENTS.md",
        "VERSION",
        "CHANGELOG.md",
        "README.md",
        "docs/README.md",
        "issues/README.md",
        "reports/README.md",
    ]:
        path = module_dir.joinpath(rel)
        if not path.exists():
            errors.append(str(path.relative_to(root)))

if errors:
    print("missing governance files:")
    print("\n".join(map(str, errors)))
    sys.exit(1)

print("governance check passed")
