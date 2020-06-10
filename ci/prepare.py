#! /usr/bin/env python3

import tarfile
import shutil
import ssl

from os import environ as env, symlink
from platform import system
from io import BytesIO
from urllib.request import Request, urlopen
from urllib.parse import quote, urlencode
from zipfile import ZipFile
from pathlib import Path


def define(key: str, value: str):
    return ["-D", f"{key}={value}"]


def define_env(name: str):
    return define(name, env[name]) if name in env else []


def fetch_artifacts(project, reference, job):
    gitlab_host = "https://git.imp.fu-berlin.de"
    project = quote(project, safe="")
    reference = quote(reference, safe="")
    params = urlencode([("job", job)], doseq=True)
    url = f"{gitlab_host}/api/v4/projects/{project}/jobs/artifacts/{reference}/download?{params}"
    headers = {"JOB-TOKEN": env["CI_JOB_TOKEN"]}
    return ZipFile(
        BytesIO(
            urlopen(
                Request(url, headers=headers), context=ssl._create_unverified_context()
            ).read()
        )
    )


def extract_cmake_package(artifacts, name):
    for filename in artifacts.namelist():
        if Path(filename).match(f"{name}-*.tar.xz"):
            with tarfile.open(fileobj=BytesIO(artifacts.read(filename))) as f:
                f.extractall("vendor")
            shutil.move(next(Path("vendor").glob(f"{name}-*/")), f"vendor/{name}")


if __name__ == "__main__":
    if system() == "Windows":
        job_stem = "package windows"
    elif system() == "Linux":
        job_stem = "package centos"
    else:
        assert False

    for name, project, job in [
        ("biotracker-interfaces", "bioroboticslab/biotracker/interfaces", job_stem),
        ("biotracker-utility", "bioroboticslab/biotracker/utility", job_stem),
        ("robofish-behavior_loader", "bioroboticslab/biotracker/behavior_loader", job_stem),
    ]:
        with fetch_artifacts(project, "master", job) as artifacts:
            extract_cmake_package(artifacts, name)
