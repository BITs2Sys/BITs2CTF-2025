def KSA(key):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) % 256
        S[i], S[j] = S[j], S[i]
    return S


def PRGA(S,text,r):
    i, j = 0, 0
    for l in range(len(text)):
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        K = ((S[i] + S[j]) % 256) ^ r[(i+j)%len(r)]
        text[l]^=S[K]
    return text


def RC4(key, text, r):
    S = KSA(key)
    keystream = PRGA(S,text,r)
    return keystream

key = b'BITs2CTF{}'
plaintext=[190,103,243,118,225,92,160,121,31,66,167,2,230,153,141,227,206,16,48,54,246,28,27,161,77,37,69,67,238,53,252,177,135,179,187,93,38,30,231,8,80,252,78,164,126,69,230,252,201,24,56,16,166,75,127,213]

print(len(plaintext))
ran=[23,104,132,23,8,143,166,242,234,47,83,177,211,182,226,91,92,156,88,100,248,168,34,226,148,5,87,105,72,254,32,50,120,193,34,83,201,48,71,172,27,143,61,159,114,223,205,52,174,254,50,249,252,90,78,54]

ciphertext = RC4(key, plaintext,ran)
for i in range(len(ciphertext)-1,-1,-1):
    ciphertext[i]^=ciphertext[(i+1)%len(ciphertext)]
ls=[]
for i in range(len(ciphertext)//2):
    ls.append(chr(int(chr(ciphertext[2*i+1])+chr(ciphertext[2*i]),16)))
print('BITs2CTF{',end='')
print(''.join(ls),end='')
print('}')

print(len('ISCC{G0r3_15_h@Aa4%&rD_7O_5oLv3_Ov0?!}'))
print(len('BITs2CTF{G0r3_15_h@Aa4%&rD_7O_5oLv3?!}'))