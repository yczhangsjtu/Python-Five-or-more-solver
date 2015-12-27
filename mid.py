#!/usr/bin/python

n=map(int,raw_input().strip(' ').split(' '))
n.sort()
l=len(n)
if l == 0:
    print 0
else:
    if l%2 == 0:
        print (n[l/2-1]+n[l/2])/2
    else:
        print n[l/2]
