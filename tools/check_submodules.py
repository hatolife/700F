#!/usr/bin/env python3
from pathlib import Path
import configparser
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
CODEC2_PATH = "external/codec2"
CODEC2_URL = "https://github.com/drowe67/codec2.git"


def run_git(args):
    return subprocess.run(
        ["git", *args],
        cwd=ROOT,
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )


def fail(message):
    print(f"submodule check failed: {message}")
    return 1


def main():
    gitmodules = ROOT / ".gitmodules"
    if not gitmodules.exists():
        return fail(".gitmodules is missing")

    parser = configparser.ConfigParser()
    parser.read(gitmodules)
    section = f'submodule "{CODEC2_PATH}"'
    if section not in parser:
        return fail(f"{CODEC2_PATH} entry is missing from .gitmodules")
    if parser[section].get("path") != CODEC2_PATH:
        return fail(f"{CODEC2_PATH} path is incorrect in .gitmodules")
    if parser[section].get("url") != CODEC2_URL:
        return fail(f"{CODEC2_PATH} url is incorrect in .gitmodules")

    gitlink = run_git(["ls-files", "--stage", CODEC2_PATH])
    if gitlink.returncode != 0 or not gitlink.stdout.startswith("160000 "):
        return fail(f"{CODEC2_PATH} is not recorded as a gitlink in the index")
    pinned = gitlink.stdout.split()[1]

    worktree = ROOT / CODEC2_PATH
    if (worktree / ".git").exists():
        head = run_git(["-C", CODEC2_PATH, "rev-parse", "HEAD"])
        if head.returncode != 0:
            return fail(head.stderr.strip())
        if head.stdout.strip() != pinned:
            return fail(f"{CODEC2_PATH} worktree HEAD {head.stdout.strip()} != pinned {pinned}")
        state = "initialized"
    else:
        state = "uninitialized"

    print(f"submodule check passed: {CODEC2_PATH} {pinned} ({state})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
