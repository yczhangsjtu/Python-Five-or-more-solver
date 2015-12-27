#!/usr/bin/python

import os
from util import *

def __eval(bd):
    s = 0
    s += evalfives(bd)
    return s

def evalfives(bd):
    s = 0
    for i in range(9):
        for j in range(5):
            s += __evalfive(bd[i][j],bd[i][j+1],bd[i][j+2],\
                           bd[i][j+3],bd[i][j+4])
    for i in range(9):
        for j in range(5):
            s += __evalfive(bd[j][i],bd[j+1][i],bd[j+2][i],\
                           bd[j+3][i],bd[j+4][i])
    for i in range(5):
        for j in range(5):
            s += __evalfive(bd[i][j],bd[i+1][j+1],bd[i+2][j+2],\
                           bd[i+3][j+3],bd[i+4][j+4])
    for i in range(5):
        for j in range(5):
            s += __evalfive(bd[i][j+4],bd[i+1][j+3],bd[i+2][j+2],\
                           bd[i+3][j+1],bd[i+4][j])
    return s

def __evalfive(c1,c2,c3,c4,c5):
    l = [c1,c2,c3,c4,c5]
    s = [c for c in l if c != '.']
    st = set(s)
    if len(st) == 0: return 4
    if len(st) == 1:
        if len(s) == 1: return 5
        if len(s) == 2: return 50
        if len(s) == 3: return 300
        if len(s) == 4: return 1000
        if len(s) == 5: return 10000
    if len(st) == 2:
        ct = {}
        for c in s:
            if c in ct:
                ct[c] += 1
            else:
                ct[c] = 1
        for c in ct:
            if ct[c] == 4: return 300
            if ct[c] == 3 and len(s) == 4: return 250
            if ct[c] == 3 and len(s) == 5: return 100
            if ct[c] == 2 and len(s) == 3: return 20
            if ct[c] == 2 and len(s) == 4: return 20
            if ct[c] == 1 and len(s) == 2: return 1
    if len(st) == 3:
        ct = {}
        for c in s:
            if c in ct:
                ct[c] += 1
            else:
                ct[c] = 1
        for c in ct:
            if ct[c] == 3: return 10
        return 0
    if len(st) == 4:
        return -50
    if len(st) == 5:
        return -100

def bestmove(bd):
    m = None
    for p in range(9):
        for q in range(9):
            for r in range(9):
                for s in range(9):
                    if bd[p][q] == '.' or bd[r][s] != '.':
                        continue
                    if not connected(bd,p,q,r,s):
                        continue
                    c = bd[p][q]
                    bd[p] = strrep(bd[p],q,'.')
                    bd[r] = strrep(bd[r],s,c)
                    e = __eval(bd)
                    if m == None or e > m[0]: m = (e,(p,q),(r,s))
                    bd[p] = strrep(bd[p],q,c)
                    bd[r] = strrep(bd[r],s,'.')
    return m

state = raw_input()
score = input()
if state == "over":
    print "over"
    print score
else:
    data = [raw_input() for i in range(9)]
    # printboard(data)
    bm = bestmove(data)
    if bm == None:
        print "over"
        print score
    else:
        print "normal"
        print score
        printcolormove(data,bm[1][0],bm[1][1],bm[2][0],bm[2][1])
