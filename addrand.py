#!/usr/bin/python

from util import *
from random import choice

state = raw_input()
score = input()
if state == "over":
    print "over"
    print score
else:
    bd = [raw_input() for i in range(9)]

    if state == "cancelled":
        print "normal"
        print score
        printboard(bd)
    else:
        l = []
        cs = ['r','g','b','y','o','d','p']
        for i in range(9):
            for j in range(9):
                if bd[i][j] == '.':
                    l.append((i,j))

        if len(l) < 3:
            print "over"
            print score
        else:
            for i in range(3):
                p = choice(l)
                c = choice(cs)
                bd[p[0]] = strrep(bd[p[0]],p[1],c)
                l.remove(p)
            print "normal"
            print score
            printboard(bd)
