#!/bin/sh

(cd /home/ctf/src || exit 1 && su ctf -c '/home/ctf/.local/bin/uv run ./main.py')
