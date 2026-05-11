#!/bin/python

import base64

with open("./exp.rs", "rb") as f:
    code = f.read()
    code = base64.b64encode(code).decode()
    print(code)
