#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t fun1(uint32_t a1, uint32_t a2)
{
    uint64_t sum = 0;
    uint32_t carry = 0;
    unsigned int pos = 0;

    while (a1 || a2) {
        uint32_t b1 = a1 & 1u;
        uint32_t b2 = a2 & 1u;
        a1 >>= 1;
        a2 >>= 1;

        uint32_t bit = (carry ^ b1 ^ b2) & 1u;
        sum |= ((uint64_t)bit << pos);

        carry = (carry & b1) | (b2 & b1) | (carry & b2);
        ++pos;
    }
    return sum | ((uint64_t)carry << pos);
}

static uint64_t fun2(uint32_t a1, int a2)
{
    return fun1(a1, (uint32_t)(-a2));
}

static uint32_t fun3(uint32_t a1, int a2)
{
    uint32_t acc = 0;
    unsigned int shift = 0;
    while (a1) {
        if (a1 & 1u) {
            acc += (uint32_t)((uint32_t)a2 << shift);
        }
        a1 >>= 1;
        ++shift;
    }
    return acc;
}

static uint32_t fun4(uint32_t a1, uint32_t a2)
{
    unsigned int v5;
    int v6;
    unsigned int v7;
    unsigned int v8;

    v5 = 0;
    v6 = 0;
    while (a1 || a2)
    {
        v7 = a1 & 1;
        v8 = a2 & 1;
        a1 >>= 1;
        a2 >>= 1;
        v5 += (v8 ^ v7) << v6++;
    }
    return v5;
}

static bool judge(uint32_t a1, uint32_t a2, uint32_t a3,
                       uint32_t a4, uint32_t a5, uint32_t a6)
{
    if (a1 <= 0x186a0 || a2 <= 0x186a0 || a3 <= 0x186a0 ||
        a4 <= 0x186a0 || a5 <= 0x186a0 || a6 <= 0x186a0)
        return false;

    if (a1 > 0xf423f || a2 > 0xf423f || a3 > 0xf423f ||
        a4 > 0xf423f || a5 > 0xf423f || a6 > 0xf423f)
        return false;

    uint32_t v1, v2, v3, v4, v5, v6;

    
    v1 = (uint32_t)fun1(a1, a2) % 0xe8329u;
	uint32_t t1 = (uint32_t)fun2(a1, (int)a2);
    uint32_t t2 = fun3(2u, (int)a3);
    v2 = (uint32_t)fun1(t1, t2);
    uint32_t t3 = fun3(4u, (int)a6);
    v3 = (uint32_t)fun4(t3, a4);
    uint32_t t4 = (uint32_t)fun2(a4, (int)a5);
    v4 = fun3(5u, (int)t4);
    v5 = (uint32_t)fun1(a1, a6);
    uint32_t t5 = fun3(3u, (int)a4);
    v6 = (uint32_t)fun2(t5, (int)t4);

    return  (v1 == 597141u) &&
            (v2 == 1644082u) &&
            (v3 == 1161537u) &&
            (v4 == 343890u) &&
            (v5 == 1136538u) &&
            (v6 == 1952901u);
}

int main(void)
{
    uint32_t a, b, c, d, e, f;
    char s[56];

    puts("请输入6个六位数: a, b, c, d, e, and f.");
    putchar('\n');

    printf("a = "); if (scanf("%u", &a) != 1) return 0;
    printf("b = "); if (scanf("%u", &b) != 1) return 0;
    printf("c = "); if (scanf("%u", &c) != 1) return 0;
    printf("d = "); if (scanf("%u", &d) != 1) return 0;
    printf("e = "); if (scanf("%u", &e) != 1) return 0;
    printf("f = "); if (scanf("%u", &f) != 1) return 0;

    if (judge(a, b, c, d, e, f)) {
        puts("Right.");
        sprintf(s, "BITs2CTF{%x%x%x%x%x%x}", a, b, c, d, e, f);
        puts(s);
    } else {
        puts("Wrong.");
    }
    return 0;
}

