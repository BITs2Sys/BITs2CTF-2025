from Cryptodome.Cipher import AES
import os
import gmpy2
from flag import FLAG
from Cryptodome.Util.number import *

def main():
    key = os.urandom(2) * 16
    iv = os.urandom(16)
    print(bytes_to_long(key) ^ bytes_to_long(iv))
    aes = AES.new(key, AES.MODE_CBC, iv)
    enc_flag = aes.encrypt(FLAG)
    print(enc_flag)

if __name__ == "__main__":
    main()



#output：
#54755426038978086460310747316994556563126341157253266647920514387148480709140
#b'K\x04?\'\xc1<\xe0\x15\xd2i\x8b\xb1"9?)\x1a\xb3)\xd6a\xfa\x9c\x84*\x0fi\xf7\xa4\x85\xd2\x1d'