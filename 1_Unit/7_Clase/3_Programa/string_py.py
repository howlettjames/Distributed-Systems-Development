from random import randint, uniform,random
import os

N=300000
cadenota=""
list1=['I','P','N']
for i in range(N):
    for x in range(3):
        s=randint(65,90)
        cadenota=cadenota+chr(s)
    cadenota=cadenota+" "
#print(cadenota)
a=0
b=3
found=0
r=len(cadenota)
while(r!=0):
    sr=cadenota[a:b]
    if(sr=="IPN"):
        found+=1
    a=a+4
    b=b+4
    r=r-4

print(found)
#print(len(cadenota))
