#! /usr/bin/env python3

from os import environ as env, pathsep
from subprocess import check_call
from pathlib import Path
from platform import system

if __name__ == "__main__":
    if system() == "Linux":
        for path in Path("vendor").glob("*/bin"):
            env["PATH"] += f"{pathsep}{path}"
        env['PATH'] += f"{pathsep}{env['Qt5_DIR']}/../../../bin/"

    command = ["ninja", "-C", "build", "package"]
    check_call(command)
