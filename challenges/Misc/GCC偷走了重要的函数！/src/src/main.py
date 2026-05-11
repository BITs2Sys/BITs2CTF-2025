from tree_sitter import Language, Parser
from pathlib import Path
import atexit
import tree_sitter_c
import subprocess as sp
import os
from color import *

TITLE = "GCCは大変なものを"
C_LANGUAGE = Language(tree_sitter_c.language())
DIR: Path = "../build"
FILE_NAME: Path = f"{DIR}/gunmu.c"
EXE_NAME: Path = f"{DIR}/gunmu"
FLAG = "BITS2FLAG"


def ok(x): return print(color(x, GREEN))
def info(x): return print(color(x, GRAY))
def err(x): return print(color(x, RED))


def fail_exit():
    err("[Check Failed]".center(50, '-'))
    err("Remember No Function :(")
    exit(1)


def check(code: str):
    parser = Parser(C_LANGUAGE)
    tree = parser.parse(code.encode())
    function_node = tree.root_node.children[0]

    if "/dev" in code:
        err("What do you want to do?")
        exit(1)

    if function_node.type == "declaration":
        if function_node.children[1].type == "function_declarator":
            fail_exit()
    elif function_node.type == "function_definition":
        fail_exit()

    ok("[Check Passed]".center(50, '-'))

    # print(function_node.type)
    # print(tree.root_node)


def clear():
    if os.path.exists(FILE_NAME):
        os.remove(FILE_NAME)
    if os.path.exists(EXE_NAME):
        os.remove(EXE_NAME)


def compile():
    cmd = ["gcc", "-o", EXE_NAME, FILE_NAME]
    res = sp.run(cmd, cwd=DIR, stderr=sp.PIPE, stdout=sp.PIPE)
    if res.returncode != 0:
        err("Code cannot be compiled")
        err(f"Error:\n{res.stderr.decode()}")
        exit(1)


def check_output():
    cmd = [EXE_NAME]
    res = sp.run(cmd, cwd=DIR, stderr=sp.PIPE, stdout=sp.PIPE)
    info(f"Ready to run '{EXE_NAME}'")

    if res.returncode != 0:
        err(f"Program exited with non-zero code: {res.returncode}")
        err(f"stdout:\n{res.stdout.decode()}")
        err(f"stderr:\n{res.stderr.decode()}")
        exit(1)
    else:
        output = res.stdout.decode()
        info(f"Your output: {output.encode()}")
        if output == "Hello World\n":
            ok("Congratulations on completing this challenge!")
            ok(f"Your Flag: {os.environ.get(FLAG)}")
        else:
            err("No 'Hello World\\n' Found in your program's output :(")
            exit(1)


def banner():
    print()
    with open('./banner', "rb") as f:
        banner = f.read().decode()
        print(banner.replace("ESC", "\033"))


def tips():
    info("Some useful information:")
    cmd = ["gcc", "-v"]
    res = sp.run(cmd, cwd=DIR, stderr=sp.PIPE, stdout=sp.PIPE)
    info(res.stderr.decode().split('\n')[-2])


def write_to_file(code: str):
    with open(FILE_NAME, "w") as f:
        f.write(code)


def main():
    atexit.register(clear)
    print(color(f"[ BITs2CTF | {TITLE} ]".center(50, '='), CYAN))

    banner()

    info("Your task is to provide a C source code that outputs 'Hello World\\n' exactly")
    info("Sounds simple? But we don't allow you to use functions")
    info(
        "For example , code like `int main() { puts(\"Hello World\"); return 0; }` is not allowed")

    tips()
    source_code = input(
        color("Give your C source code here(Only one line):\n> ", GRAY))

    if source_code.__len__() != 0:
        check(source_code)
        write_to_file(source_code)
        compile()
        check_output()
    else:
        err("Blank content cannot be compiled")
        exit(1)


if __name__ == "__main__":
    main()
