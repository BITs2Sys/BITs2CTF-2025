
output=[139,161,68,233,197,35,129,79,171,76,167,6,37,135,207,236,143,169,41,205,8,197,109,109,205,228,140,128]
# print(len(output))
key=[ord(i) for i in 'reee']
count=50

for i in range(len(output)):
    output[i]=((output[i]<<3)%256)|(output[i]>>5)
    output[i]^=0b101**2

ls=[]
for i in range(4):
    ls.append(
        (output[7 * i] & 0x40) | (output[7 * i + 1] & 0x20) | (output[7 * i + 2] & 0x10) | (output[7 * i + 3] & 0x8) | (
                    output[7 * i + 4] & 0x4) | (output[7 * i + 5] & 0x2) | (output[7 * i + 6] & 0x1))
    ls.append(
        (output[7 * i] & 0x20) | (output[7 * i + 1] & 0x10) | (output[7 * i + 2] & 0x8) | (output[7 * i + 3] & 0x4) | (
                    output[7 * i + 4] & 0x2) | (output[7 * i + 5] & 0x1) | (output[7 * i + 6] & 0x40))
    ls.append(
        (output[7 * i] & 0x10) | (output[7 * i + 1] & 0x8) | (output[7 * i + 2] & 0x4) | (output[7 * i + 3] & 0x2) | (
                    output[7 * i + 4] & 0x1) | (output[7 * i + 5] & 0x40) | (output[7 * i + 6] & 0x20))
    ls.append(
        (output[7 * i] & 0x8) | (output[7 * i + 1] & 0x4) | (output[7 * i + 2] & 0x2) | (output[7 * i + 3] & 0x1) | (
                    output[7 * i + 4] & 0x40) | (output[7 * i + 5] & 0x20) | (output[7 * i + 6] & 0x10))
    ls.append(
        (output[7 * i] & 0x4) | (output[7 * i + 1] & 0x2) | (output[7 * i + 2] & 0x1) | (output[7 * i + 3] & 0x40) | (
                    output[7 * i + 4] & 0x20) | (output[7 * i + 5] & 0x10) | (output[7 * i + 6] & 0x8))
    ls.append(
        (output[7 * i] & 0x2) | (output[7 * i + 1] & 0x1) | (output[7 * i + 2] & 0x40) | (output[7 * i + 3] & 0x20) | (
                    output[7 * i + 4] & 0x10) | (output[7 * i + 5] & 0x8) | (output[7 * i + 6] & 0x4))
    ls.append(
        (output[7 * i] & 0x1) | (output[7 * i + 1] & 0x40) | (output[7 * i + 2] & 0x20) | (output[7 * i + 3] & 0x10) | (
                    output[7 * i + 4] & 0x8) | (output[7 * i + 5] & 0x4) | (output[7 * i + 6] & 0x2))

for i in range(1,4):
    if(key[i]==key[i-1]):
        key[i]=count
        count+=1
    else:
        count=50

for i in range(3):
    key[i]^=key[i+1]

i=0
while(i<len(ls)//8):
    ls[4 * i] ^= key[i % 4]
    ls[4 * i + 1] ^= key[(i + 1) % 4]
    ls[4 * i + 2] ^= key[(i + 2) % 4]
    ls[4 * i + 3] ^= key[(i + 3) % 4]
    i+=1

print(''.join(map(chr,ls)))
print('ISCC{Ru57R3_1s_r34Liy_345Y!}')
print('BITs2CTF{D1fficuLt_Ru57R3~~}')

