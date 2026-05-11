import base64
import hashlib
from Crypto.Util.number import *
from random import choice
from Crypto.Util.number import isPrime, sieve_base as primes

def parinad(n):
	return bin(n)[2:].count('1') % 2

def vinad(x, R):
	return int(''.join(str(parinad(x ^ r)) for r in R), 2)

def genkey(nbit):
	while True:
		R = [getRandomNBitInteger(nbit) for _ in range(nbit)]
		r = getRandomNBitInteger(nbit)
		p, q = vinad(r, R), getPrime(nbit)
		if isPrime(p):
			e = vinad(r + 0x10001, R)
			if GCD(e, (p - 1) * (q - 1)) == 1:
				return (e, R, p * q), (p, q)

def encrypt(message, pubkey):
	e, R, n = pubkey
	return pow(message + sum(R), e, n)


def getPrime(bits):
    while True:
        n = 2
        while n.bit_length() < bits:
            n *= choice(primes)
        if isPrime(n + 1):
            return n + 1

m1=getPrime(512)
m2=getPrime(512)
m=m1*m2
m1_bytes = long_to_bytes(m1)
m2_bytes = long_to_bytes(m2)

md5_m1 = hashlib.md5()
md5_m1.update(m1_bytes)
m1_md5 = md5_m1.hexdigest()

md5_m2 = hashlib.md5()
md5_m2.update(m2_bytes)
m2_md5 = md5_m2.hexdigest()

flag='BITs2CTF{'+m1_md5+'<*_*>'+m2_md5+'}'

nbit = 1024
pubkey, _ = genkey(nbit)
assert m < pubkey[2]
c = encrypt(m, pubkey)

print(f'R = {pubkey[1]}')
print(f'n = {pubkey[2]}')
print(f'c = {c}')
