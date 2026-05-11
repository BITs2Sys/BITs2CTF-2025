from sys import stdout
from time import sleep


BOLD = "\033[01m"

RED = "\033[01;31m"
GREEN = "\033[01;32m"
YELLOW = "\033[01;33m"
BLUE = "\033[01;34m"
PURPLE = "\033[01;35m"
CYAN = "\033[01;36m"
GRAY = "\033[01;37m"

END = "\033[0m"

# 0xFFBC03
PYTHON = "\033[01;38;2;255;188;3m"


def color(string: str, color: str):
    return color + string + END


def fake_input(string: str):
    print(GRAY + "$ ", end="")
    stdout.flush()
    for ch in string:
        print(ch, end="")
        stdout.flush()
        sleep(0.05)
    print(END)
