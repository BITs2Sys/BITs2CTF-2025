#!/bin/python

with open("./exp.c", "rb") as f:
    code = f.read().decode().replace("\n", "")
    print(code)
