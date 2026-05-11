from cryptography.hazmat.primitives.ciphers import Cipher, algorithms
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.backends import default_backend
from Crypto.Util.number import long_to_bytes, bytes_to_long
import time
import random
import os


def chacha20_encrypt(plaintext: bytes, key: bytes, nonce: bytes) -> bytes:
    cipher = Cipher(algorithms.ChaCha20(key, nonce), mode=None, backend=default_backend())
    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(plaintext)
    return ciphertext


def chacha20_decrypt(ciphertext: bytes, key: bytes, nonce: bytes) -> bytes:
    cipher = Cipher(algorithms.ChaCha20(key, nonce), mode=None, backend=default_backend())
    decryptor = cipher.decryptor()
    plaintext = decryptor.update(ciphertext)
    return plaintext


if __name__ == "__main__":
    k1 = 89156737880809474145449532029493055444849328922741582677584755390029529653680
    n1 = 20979402206073728478533457085044507592

    # random.seed(int(time.time())%100)
    #遍历即可
    random.seed(37)
    k2=random.getrandbits(128)
    n2=random.getrandbits(64)

    key=long_to_bytes(k1^k2)
    nonce=long_to_bytes(n1^n2)

    encrypted_data=long_to_bytes(0xa8c123f27ed9d34a6040a98f0b9d5e22930ca34bd3195e27a1e73725aba2f3eff888)
    decrypted_data = chacha20_decrypt(encrypted_data, key, nonce)
    print(f"解密后: {decrypted_data.decode()}")
