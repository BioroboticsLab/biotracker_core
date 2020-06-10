#! /usr/bin/env python3

from ast import literal_eval
from os import environ as env, pathsep
from subprocess import check_call
from pathlib import Path
from platform import system


def define(key: str, value: str):
    return ["-D", f"{key}={value}"]


def define_env(name: str):
    return define(name, env[name]) if name in env else []



if __name__ == "__main__":
    if system() == "Windows":
        env["PATH"] += f"{pathsep}{env['VCPKG_INSTALL_DIR']}/bin"
        for path in Path("vendor").glob("*/bin"):
            env["PATH"] += f"{pathsep}{path}"

        shared_libraries = [
            "zlib1",
            "tiff",
            "jpeg62",
            "libpng16",
            "webp",
            "lzma",
            "openblas",
            "avformat-58",
            "avcodec-58",
            "avutil-56",
            "swscale-5",
            "swresample-3",
        ]

        opencv_components = [
            "core",
            "calib3d",
            "features2d",
            "flann",
            "highgui",
            "imgcodecs",
            "imgproc",
            "ml",
            "objdetect",
            "photo",
            "shape",
            "stitching",
            "superres",
            "video",
            "videoio",
            "videostab",
        ]

    command = ["cmake"]
    command += ["-S", "."]
    command += ["-B", "build"]
    command += ["-G", "Ninja"]
    command += define("CMAKE_PREFIX_PATH", Path("vendor").resolve())
    command += define_env("CMAKE_BUILD_TYPE")
    command += define_env("CMAKE_TOOLCHAIN_FILE")
    command += define("CMAKE_SUPPRESS_REGENERATION", "ON")
    command += define("CMAKE_SKIP_PACKAGE_ALL_DEPENDENCY", "ON")
    command += define_env("VCPKG_TARGET_TRIPLET")

    if system() == "Windows":
        command += define("PACKAGE_MSI", "ON")
        command += define("INSTALL_SYSTEM_LIBRARIES", "ON")
        command += define("INSTALL_SHARED_LIBRARIES", ";".join(shared_libraries),)
        command += define("INSTALL_OPENCV_COMPONENTS", ";".join(opencv_components),)

    elif system() == "Linux":
        command += define("PACKAGE_APPIMAGE", "ON")
    else:
        assert False

    check_call(command)
