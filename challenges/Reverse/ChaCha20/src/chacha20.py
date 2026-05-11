from cryptography.hazmat.primitives.ciphers import Cipher, algorithms
from cryptography.hazmat.backends import default_backend
from Crypto.Util.number import long_to_bytes, bytes_to_long
from secret import flag
import time
import random
import os

def chacha20_encrypt(plaintext: bytes, key: bytes, nonce: bytes) -> bytes:
    cipher = Cipher(algorithms.ChaCha20(key, nonce), mode=None, backend=default_backend())
    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(plaintext)
    return ciphertext

if __name__ == "__main__":
    k1 = bytes_to_long(os.urandom(32))
    n1 = bytes_to_long(os.urandom(16))
    print(f'k1={k1}')
    print(f'n1={n1}')

    random.seed(int(time.time())%100)
    k2=random.getrandbits(128)
    n2=random.getrandbits(64)

    key=long_to_bytes(k1^k2)
    nonce=long_to_bytes(n1^n2)

    encrypted_data = chacha20_encrypt(flag, key, nonce)
    print(f"加密后: {encrypted_data.hex()}")

