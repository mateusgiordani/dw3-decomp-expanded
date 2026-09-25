"""Download and check the public toolchain pinned in config/open_toolchain.json.

Fetches the GCC 2.8.1 PlayStation build from decompals/old-gcc and the pinned
maspsx revision, checks both archives and the files used against SHA-256,
and extracts them to ``toolchains/`` (git-ignored). GNU ``mipsel-linux-gnu-as``
is not downloaded; install it from your distribution, for example
``sudo apt install binutils-mipsel-linux-gnu`` (on Windows, inside WSL).

Usage:
    python tools/fetch_toolchain.py
"""

from __future__ import annotations

import hashlib
import io
import json
import shutil
import stat
import tarfile
import urllib.request
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEST = ROOT / "toolchains"


def fetch(url: str, digest: str, cache: Path) -> bytes:
    if cache.is_file() and hashlib.sha256(cache.read_bytes()).hexdigest() == digest:
        return cache.read_bytes()
    print(f"downloading {url}")
    with urllib.request.urlopen(url, timeout=120) as response:
        data = response.read()
    actual = hashlib.sha256(data).hexdigest()
    if actual != digest:
        raise SystemExit(f"{url}: SHA-256 {actual} does not match the pinned {digest}")
    cache.parent.mkdir(parents=True, exist_ok=True)
    cache.write_bytes(data)
    return data


def check(path: Path, digest: str) -> None:
    actual = hashlib.sha256(path.read_bytes()).hexdigest()
    if actual != digest:
        raise SystemExit(f"{path}: SHA-256 {actual} does not match the pinned {digest}")


def main() -> int:
    cfg = json.loads((ROOT / "config/open_toolchain.json").read_text(encoding="utf-8"))
    downloads = DEST / "downloads"

    gcc = cfg["gcc"]
    data = fetch(gcc["archive_url"], gcc["archive_sha256"], downloads / Path(gcc["archive_url"]).name)
    gcc_dir = DEST / gcc["id"]
    shutil.rmtree(gcc_dir, ignore_errors=True)
    gcc_dir.mkdir(parents=True)
    with tarfile.open(fileobj=io.BytesIO(data), mode="r:gz") as archive:
        for member in archive.getmembers():
            if not member.isfile():
                continue
            target = gcc_dir / Path(member.name).name
            target.write_bytes(archive.extractfile(member).read())
            target.chmod(target.stat().st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
    check(gcc_dir / "cc1", gcc["cc1_sha256"])

    masp = cfg["maspsx"]
    data = fetch(masp["archive_url"], masp["archive_sha256"], downloads / f"maspsx-{masp['revision']}.zip")
    masp_dir = DEST / "maspsx"
    shutil.rmtree(masp_dir, ignore_errors=True)
    with zipfile.ZipFile(io.BytesIO(data)) as archive:
        for name in archive.namelist():
            relative = Path(*Path(name).parts[1:])
            if name.endswith("/") or not relative.parts:
                continue
            target = masp_dir / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(archive.read(name))
    for relative, digest in masp["files_sha256"].items():
        check(masp_dir / relative, digest)

    print(f"ready: {gcc_dir.relative_to(ROOT)} and {masp_dir.relative_to(ROOT)}")
    print(f"also required: {cfg['gnu_as']['command']} ({cfg['gnu_as']['package']})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
