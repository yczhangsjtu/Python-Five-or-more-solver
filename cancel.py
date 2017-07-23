#!/usr/bin/python

import os
from util import *

def __all(c1,c2,c3,c4,c5):
    return c1==c2 and c1==c3 and c1==c4 and c1==c5 and c1!='.'

def cancel(bd,score):
    bl = [[False for i in range(9)] for j in range(9)]

    for i in range(9):
        for j in range(5):
            if __all(bd[i][j],bd[i][j+1],bd[i][j+2],\
                    bd[i][j+3],bd[i][j+4]):
                bl[i][j] = True
                bl[i][j+1] = True
                bl[i][j+2] = True
                bl[i][j+3] = True
                bl[i][j+4] = True
            if __all(bd[j][i],bd[j+1][i],bd[j+2][i],\
                    bd[j+3][i],bd[j+4][i]):
                bl[j][i]= True
                bl[j+1][i]= True
                bl[j+2][i]= True
                bl[j+3][i]= True
                bl[j+4][i]= True
    for i in range(5):
        for j in range(5):
            if __all(bd[i][j],bd[i+1][j+1],bd[i+2][j+2],\
                    bd[i+3][j+3],bd[i+4][j+4]):
                bl[i][j] = True
                bl[i+1][j+1] = True
                bl[i+2][j+2] = True
                bl[i+3][j+3] = True
                bl[i+4][j+4] = True
            if __all(bd[i][j+4],bd[i+1][j+3],bd[i+2][j+2],\
                    bd[i+3][j+1],bd[i+4][j]):
                bl[i][j+4] = True
                bl[i+1][j+3] = True
                bl[i+2][j+2] = True
                bl[i+3][j+1] = True
                bl[i+4][j] = True

    cc = False
    for i in range(9):
        for j in range(9):
            if bl[i][j]:
                cc = True
                bd[i] = strrep(bd[i],j,'.')
                score += 1

    if cc:
        return "cancelled",score,bd
    else:
        return "normal",score,bd

state = raw_input()
score = input()
if state == "over":
    print "over"
    print score
else:
    bd = [raw_input() for i in range(9)]
    state,score,bd = cancel(bd,score)
    print state
    print score
    if state != "over":
        printboard(bd)
