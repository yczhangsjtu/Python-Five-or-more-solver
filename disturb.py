#!/usr/bin/python

from random import randrange, choice

while True:
    try:
        n = raw_input()
        n = map(int,n.split(' '))
        i = randrange(len(n))
        k = n[i]
        n[i] = k + choice([-1,1])*randrange(abs(k)/2,abs(k))
        print " ".join(map(str,n))
    except:
        break
