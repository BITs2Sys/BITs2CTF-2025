import hashlib
import sympy
from Crypto.Util.number import *
import random
import math

flag = 'BITs2CTF{^_^cRVni_aXinSsd_fJnebvooo^_^}'

flag = flag.encode('utf-8')
m = bytes_to_long(flag)

e = 3


def is_prime(n, k=5):
    """
    使用Miller-Rabin素性测试判断一个数是否为素数
    n: 待测试的数
    k: 测试的次数，次数越多准确性越高
    """
    if n <= 1:
        return False
    elif n <= 3:
        return True
    elif n % 2 == 0:
        return False

    # 将n-1表示为d*2^s
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1

    # 进行k次测试
    for _ in range(k):
        a = random.randint(2, min(n - 2, 1 << 20))  # 随机选择一个数a
        x = pow(a, d, n)  # 计算a^d mod n

        if x == 1 or x == n - 1:
            continue

        for __ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            # 如果没有找到n-1，那么n是合数
            return False

    return True


def generate_125_digit_prime():
    """生成一个125位的十进制素数"""
    # 10^124 到 10^125 - 1 之间的随机数
    lower = 10 ** 124
    upper = (10 ** 125) - 1

    while True:
        # 生成范围内的随机数
        p = random.randint(lower, upper)
        # 确保是奇数
        if p % 2 == 0:
            p += 1
            if p > upper:
                continue

        # 检查是否为素数
        if is_prime(p, k=10):  # 增加测试次数以提高准确性
            return p


def generate_rsa_primes():
    """生成RSA算法中使用的两个不同的125位十进制素数p和q"""
    p = generate_125_digit_prime()
    while True:
        q = generate_125_digit_prime()
        if q != p:  # 确保p和q是不同的素数
            break
    return p, q


if __name__ == "__main__":
    for i in range(0, e):
        p, q = generate_rsa_primes()
        n = p * q
        c = pow(m, e, n)
        print(f"n = {n}")
        print(f"c = {c}")
        print()  # 空行分隔不同的密钥对
