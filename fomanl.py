#!/usr/bin/python

import os

def __eval(bd):
    s = 0
    s += nofive(bd) * 10000
    s += nofour(bd) * 1000
    s += nothree(bd) * 10
    s += notwo(bd)
    s -= nocfive(bd) * 100
    s -= __nofar(bd)/2
    return s

def notwo(bd):
    s = 0
    for i in range(9):
        for j in range(8):
            if bd[i][j] == '.': continue
            if bd[i][j] == bd[i][j+1]:
                s += 1
    for i in range(9):
        for j in range(8):
            if bd[j][i] == '.': continue
            if bd[j][i] == bd[j+1][i]:
                s += 1
    for i in range(8):
        for j in range(8):
            if abs(i-j)>4: continue
            if bd[i][j] == '.': continue
            if bd[i][j] == bd[i+1][j+1]:
                s += 1
    for i in range(8):
        for j in range(8):
            if j+i<3 or j+i>11: continue
            if bd[i][j+1] == '.': continue
            if bd[i][j+1] == bd[i+1][j]:
                s += 1
    return s

def nothree(bd):
    s = 0
    for i in range(9):
        for j in range(7):
            if bd[i][j] == '.': continue
            if bd[i][j] == bd[i][j+1] and bd[i][j] == bd[i][j+2]:
                s += 1
    for i in range(9):
        for j in range(7):
            if bd[j][i] == '.': continue
            if bd[j][i] == bd[j+1][i] and bd[j][i] == bd[j+2][i]:
                s += 1
    for i in range(7):
        for j in range(7):
            if abs(j-i)>4: continue
            if bd[i][j] == '.': continue
            if bd[i][j] == bd[i+1][j+1] and bd[i][j] == bd[i+2][j+2]:
                s += 1
    for i in range(7):
        for j in range(7):
            if j+i<2 or j+i>10: continue
            if bd[i][j+2] == '.': continue
            if bd[i][j+2] == bd[i+1][j+1] and bd[i][j+2] == bd[i+2][j]:
                s += 1
    return s

def nofour(bd):
    s = 0
    for i in range(9):
        for j in range(6):
            if bd[i][j] == '.': continue
            if bd[i][j] == bd[i][j+1] and bd[i][j] == bd[i][j+2] and\
               bd[i][j] == bd[i][j+3]:
                s += 1
    for i in range(9):
        for j in range(6):
            if bd[j][i] == '.': continue
            if bd[j][i] == bd[j+1][i] and bd[j][i] == bd[j+2][i] and\
               bd[j][i] == bd[j+3][i]:
                s += 1
    for i in range(6):
        for j in range(6):
            if abs(j-i)>4: continue
            if bd[i][j] == '.': continue
            if bd[i][j] == bd[i+1][j+1] and bd[i][j] == bd[i+2][j+2] and\
               bd[i][j] == bd[i+3][j+3]:
                s += 1
    for i in range(6):
        for j in range(6):
            if j+i<1 or j+i>9: continue
            if bd[i][j+3] == '.': continue
            if bd[i][j+3] == bd[i+1][j+2] and\
               bd[i][j+3] == bd[i+2][j+1] and bd[i][j+3] == bd[i+3][j]:
                s += 1
    return s

def nofive(bd):
    s = 0
    for i in range(9):
        for j in range(5):
            if bd[i][j] == '.': continue
            if bd[i][j] == bd[i][j+1] and bd[i][j] == bd[i][j+2] and\
               bd[i][j] == bd[i][j+3] and bd[i][j] == bd[i][j+4]:
                s += 1
    for i in range(9):
        for j in range(5):
            if bd[j][i] == '.': continue
            if bd[j][i] == bd[j+1][i] and bd[j][i] == bd[j+2][i] and\
               bd[j][i] == bd[j+3][i] and bd[j][i] == bd[j+4][i]:
                s += 1
    for i in range(5):
        for j in range(5):
            if bd[i][j] == '.': continue
            if bd[i][j] == bd[i+1][j+1] and bd[i][j] == bd[i+2][j+2] and\
               bd[i][j] == bd[i+3][j+3] and bd[i][j] == bd[i+4][j+4]:
                s += 1
    for i in range(5):
        for j in range(5):
            if bd[i][j+4] == '.': continue
            if bd[i][j+4] == bd[i+1][j+3] and bd[i][j+4] == bd[i+2][j+2] and\
               bd[i][j+4] == bd[i+3][j+1] and bd[i][j+4] == bd[i+4][j]:
                s += 1
    return s

def nocfive(bd):
    s = 0
    for i in range(9):
        for j in range(5):
            if __allbutone(bd[i][j],bd[i][j+1],bd[i][j+2],\
                           bd[i][j+3],bd[i][j+4]):
                s += 1
    for i in range(9):
        for j in range(5):
            if __allbutone(bd[j][i],bd[j+1][i],bd[j+2][i],\
                           bd[j+3][i],bd[j+4][i]):
                s += 1
    for i in range(5):
        for j in range(5):
            if bd[i][j] == '.': continue
            if __allbutone(bd[i][j],bd[i+1][j+1],bd[i+2][j+2],\
                           bd[i+3][j+3],bd[i+4][j+4]):
                s += 1
    for i in range(5):
        for j in range(5):
            if bd[i][j+4] == '.': continue
            if __allbutone(bd[i][j+4],bd[i+1][j+3],bd[i+2][j+2],\
                           bd[i+3][j+1],bd[i+4][j]):
                s += 1
    return s

def __nofar(bd):
    s = 0
    for p in range(9):
        for q in range(9):
            for r in range(9):
                for t in range(9):
                    if abs(r-p)<5 and abs(q-t)<5: continue
                    if bd[p][q] == bd[r][t] and bd[p][q] != '.':
                        s += 1
    return s

def __allbutone(c1,c2,c3,c4,c5):
    l = [c1,c2,c3,c4,c5]
    l.sort()
    if l[0]==l[1] and l[0]==l[2] and l[0]==l[3]\
        and l[0]!=l[4] and l[0]!='.' and l[4]!='.':
        return True
    if l[4]==l[1] and l[4]==l[2] and l[4]==l[3]\
        and l[4]!=l[0] and l[0]!='.' and l[4]!='.':
        return True
    return False

def __strrep(s,i,c):
    return "%s%s%s"%(s[0:i],c,s[i+1:])

def bestmove(bd):
    m = None
    for p in range(9):
        for q in range(9):
            for r in range(9):
                for s in range(9):
                    if bd[p][q] == '.' or bd[r][s] != '.':
                        continue
                    if not __connected(bd,p,q,r,s):
                        continue
                    c = bd[p][q]
                    bd[p] = __strrep(bd[p],q,'.')
                    bd[r] = __strrep(bd[r],s,c)
                    e = __eval(bd)
                    if m == None or e > m[0]: m = (e,(p,q),(r,s))
                    bd[p] = __strrep(bd[p],q,c)
                    bd[r] = __strrep(bd[r],s,'.')
    return m

def __connected(bd,p,q,r,s):
    qe = [(p,q)]
    v = {}
    while len(qe) > 0:
        c = qe[0]
        qe.remove(c)
        nb = [(c[0]+1,c[1]),(c[0]-1,c[1]),(c[0],c[1]+1),(c[0],c[1]-1)]
        for n in nb:
            if n == (r,s): return True
            if n[0] < 0 or n[0] >= 9 or n[1] < 0 or n[1] >= 9: continue
            if bd[n[0]][n[1]] != '.': continue
            if n in v: continue
            qe.append(n)
            v[n] = True
    return False

def printboard(bd):
    os.system('clear')
    for l in bd:
        print l
    print

def printmove(bd,p,q,r,s):
    c = bd[p][q]
    d = bd[r][s]
    bd[p] = __strrep(bd[p],q,'O')
    bd[r] = __strrep(bd[r],s,'*')
    os.system('clear')
    for l in bd:
        m = ''
        for k in l:
            if k == 'O':
                m += '\x1b[37;44m\033[1m.\033[0m'
            elif k == '*':
                m += '\033[0;31m%s\033[0m'%c
            else:
                m += k
        print m
    bd[p] = __strrep(bd[p],q,c)
    bd[r] = __strrep(bd[r],s,d)

data = [raw_input() for i in range(9)]
ct = {}
printboard(data)
bm = bestmove(data)
printmove(data,bm[1][0],bm[1][1],bm[2][0],bm[2][1])
