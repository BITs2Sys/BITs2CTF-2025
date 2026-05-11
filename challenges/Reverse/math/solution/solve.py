from z3 import *
s=Solver()
a=BitVec('a',22)
b=BitVec('b',22)
c=BitVec('c',22)
d=BitVec('d',22)
e=BitVec('e',22)
f=BitVec('f',22)

s.add(a<999999)
s.add(b<999999)
s.add(c<999999)
s.add(d<999999)
s.add(e<999999)
s.add(f<999999)

v1=(a+b)%0xe8329
v2=2*c+a-b
v3=(4*f)^d
v4=5*(d-e)
v5=a+f
v6=3*d-(d-e)

s.add(v1==597141)
s.add(v2==1644082)
s.add(v3==1161537)
s.add(v4==343890)
s.add(v5==1136538)
s.add(v6==1952901)

s.check()
solution=s.model()
print(s.model())
# [f = 457801,
#  b = 869485,
#  a = 678737,
#  e = 605115,
#  d = 673893,
#  c = 917415]
# BITs2CTF{a5b51d446ddffa7a486593bbb6fc49}