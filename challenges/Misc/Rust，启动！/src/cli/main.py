import base64
import atexit
import subprocess as sp
import os
import time

from color import *
from pathlib import Path


def ok(x): return print(color(x, GREEN))
def info(x): return print(color(x, GRAY))
def err(x): return print(color(x, RED))


DIR: str = "../out"
RUST_NAME: str = f"{DIR}/code.rs"
EXE_NAME: str = f"{DIR}/code"


def banner():
    fake_input("cargo new genshin")
    time.sleep(0.2)
    print(f"\t{color("Creating", GREEN)} binary (application) `genshin` package")
    time.sleep(0.2)
    print(f"\t{color("note", CYAN)}: see more `Cargo.toml` keys and their definitions at https://doc.rust-lang.org/cargo/reference/manifest.html")
    time.sleep(0.8)
    fake_input("cd genshin && cargo run")
    time.sleep(0.2)
    print(f"\t{color("Running", GREEN)} `target/debug/genshin`")
    time.sleep(0.8)

    with open('./banner.logo', "rb") as f:
        banner = f.read().decode().replace("ESC", "\033")
        print(banner, end=END)
    print(color(f"|#|{"=" * 56}|#|", CYAN))
    print(
        color("> |#|  BITs2CTF ~ Rust，启动！ |#| <".center(56, " "), CYAN))
    print(color(f"|#|{"=" * 56}|#|", CYAN))


def process():
    ok("OK, I'll compile it")
    info("$ rustc ./code.rs")

    cmd = ["rustc", RUST_NAME]
    res = sp.run(cmd, cwd=DIR, stderr=sp.PIPE, stdout=sp.PIPE)

    if res.returncode != 0:
        err("Code cannot be compiled")
        err(f"stderr:\n{res.stderr.decode()}")
        exit(1)
    ok("Compile successfully")


def whats_next():
    info("What? No next step")
    info("Only compile, No run :)")
    info("Gooodbye~")


def interact():
    try:
        code = input(CYAN + "> " + END)
    except (KeyboardInterrupt, EOFError):
        info("\rGoodbye ~")
        info("$ exit")
        exit(0)
    res = ""
    try:
        res = base64.b64decode(code).decode()
    except:
        err(f"Fail to decode text `{code}`!")
        err("Make sure your input is base64 encoded")
        exit(1)

    with open(RUST_NAME, "w") as f:
        f.write(res)

    ok("Received. Here's Your Code:")
    info(res)


def clear():
    if os.path.exists(RUST_NAME):
        os.remove(RUST_NAME)
    if os.path.exists(EXE_NAME):
        os.remove(EXE_NAME)


def main():
    atexit.register(clear)
    banner()

    info("You can upload ANY Rust code, and I will compile it")
    info("Input your Rust code here (base64 encode):")

    interact()
    process()
    whats_next()


if __name__ == "__main__":
    main()
