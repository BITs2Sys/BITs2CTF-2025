import yaml
import json
from yaml import Loader
import base64

from yaml.parser import ParserError
from color import *


def ok(x): return print(color(x, GREEN))
def info(x): return print(color(x, GRAY))
def err(x): return print(color(x, RED))


def yaml2json(yaml_text: str):
    info(f"Your input:\n{yaml_text}")
    try:
        obj = yaml.load(yaml_text, Loader=Loader)
    except ParserError as e:
        err("Fail to parse yaml text!")
        err(e.__format__(""))
        exit(1)
    info("Ok, here's JSON format:")
    ok(json.dumps(obj))


def banner():
    print(
        color("<*] BITs2CTF ~ Yaml笑传之Class Convert Blindly [*>".center(64, "="), CYAN))
    with open('./banner.logo', "rb") as f1:
        with open("./banner.title", "rb") as f2:
            banner = f1.read().decode().replace("ESC", "\033")
            title = f2.read().decode().replace("ESC", "\033")
            for (line_banner, line_title) in zip(banner.splitlines(), title.splitlines()):
                print(f"{line_banner}{PYTHON}{line_title}")
    print(color(f"====| 北京理工大学 |{"-" * 24}| 2025年7月29日作 |====", CYAN))


def interact():
    info("A simple YAML to JSON converter")
    code = input(color("Please give your yaml file (Base64 encode)\n> ", GRAY))
    try:
        res = base64.b64decode(code)
    except:
        err(f"Fail to decode text `{code}`!")
        err("Make sure your input is base64 encoded")
        exit(1)
    return res


def main():
    banner()
    yaml2json(interact().decode())
    pass


if __name__ == "__main__":
    main()
