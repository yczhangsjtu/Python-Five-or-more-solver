#!/usr/bin/python

from random import randrange, choice

while True:
    try:
        n = raw_input()
        n = map(int,n.split(' '))
        i = randrange(len(n))
        k = n[i]
        if abs(k) < 10:
            n[i] = k + choice([-1,1])*randrange(5,10)
        else:
            n[i] = k + choice([-1,1])*randrange(abs(k)/5,abs(k)/3)
        print " ".join(map(str,n))
    except:
        break
