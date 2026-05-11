from Cryptodome.Cipher import AES
import os
import gmpy2
# from flag import FLAG
from Cryptodome.Util.number import *


FLAG='BITs2CTF{sjcihd_enf_huidjx_hsid}'
FLAG=FLAG.encode('utf-8')
assert len(FLAG) % 16 == 0, 'CBC加密明文字节数必须为16的整数倍'

def main():
    key = os.urandom(2) * 16  # 生成一个随机的密钥，长度为32字节（256位）
    iv = os.urandom(16)  # 生成一个随机的初始化向量，长度为16字节（128位）
    print(bytes_to_long(key) ^ bytes_to_long(iv))  # 将密钥和初始化向量转换为长整数，并执行异或操作，然后打印结果
    aes = AES.new(key, AES.MODE_CBC, iv)  # 使用密钥、模式（CBC）和初始化向量创建一个新的AES加密对象
    enc_flag = aes.encrypt(FLAG)  # 使用AES加密对象对FLAG进行加密，并将加密后的结果存储在enc_flag变量中
    print(enc_flag)  # 打印加密后的FLAG

if __name__ == "__main__":
    main()  # 如果当前模块是主模块，则调用main函数
