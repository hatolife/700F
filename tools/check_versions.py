#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parents[1]
errors = []
manifest = root.joinpath("versions.toml").read_text()

def parse_versions_toml(text):
    current_section = None
    root_values = {}
    modules = {}
    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        if line.startswith("[") and line.endswith("]"):
            current_section = line.strip("[]")
            continue
        if "=" not in line:
            continue
        key, value = [part.strip() for part in line.split("=", 1)]
        value = value.split("#", 1)[0].strip().strip('"')
        if current_section == "modules":
            modules[key] = value
        elif current_section is None:
            root_values[key] = value
    return root_values, modules

root_values, module_versions = parse_versions_toml(manifest)

project_version = root.joinpath("VERSION").read_text().strip()
if root_values.get("project") != project_version:
    errors.append("project VERSION mismatch")

for path in root.glob("modules/*/VERSION"):
    module_name = path.parent.name
    expected = module_versions.get(module_name)
    if expected is None:
        errors.append(f"missing {module_name} in versions.toml")
    elif path.read_text().strip() != expected:
        errors.append(f"{path.relative_to(root)} mismatch")

if errors:
    print("\n".join(errors))
    sys.exit(1)

print("version check passed")
