#! /usr/bin/env python3

from subprocess import check_call

if __name__ == "__main__":
    command = ["ninja", "-C", "build"]
    check_call(command)
