#!/usr/bin/python

from Tkinter import *
import subprocess
import string

from addrand import *
from cancel import *
from util import *

global x0,y0,w,h,nw,nh,gw,gh
global colorMap
global currboard
global score
global state
global pos1,pos2
global move

def drawBoard(bd,canvas):
    global x0,y0,x1,y1,currboard,move
    canvas.create_rectangle(x0,y0,x1,y1,fill="white")
    for i in range(nh):
        for j in range(nw):
            x = x0+w*j
            y = y0+h*i
            if len(move) == 4 and [i,j] == move[:2]:
                canvas.create_rectangle(x,y,x+w,y+h,fill="green")
            if len(move) == 4 and [i,j] == move[2:]:
                canvas.create_rectangle(x,y,x+w,y+h,fill="blue")
            if [i,j] == pos1:
                canvas.create_rectangle(x,y,x+w,y+h,fill="yellow")
            else:
                canvas.create_rectangle(x,y,x+w,y+h,fill="white")
            if bd[i][j] != ".":
                canvas.create_oval(x,y,x+w,y+h,fill=colorMap[bd[i][j]])
    if len(move) == 4:
        canvas.create_line(x0+move[1]*w+w/2,y0+move[0]*h+h/2,x0+move[3]*w+w/2,y0+move[2]*h+h/2)

def getIndex(x,y):
    offx,offy = x-x0,y-y0
    return offy/h,offx/w

def showHint():
    global currboard,score,state,move
    # with open("/tmp/board") as f:
    #     f.write("%s\n"%state)
    #     f.write("%d\n"%score)
    #     for l in currboard:
    #         f.write("%s\n"%l)
    p = subprocess.Popen(['./fomanl','-m'],stdout=subprocess.PIPE,stdin=subprocess.PIPE)
    p.stdin.write("%s\n"%state)
    p.stdin.write("%d\n"%score)
    for l in currboard:
        p.stdin.write("%s\n"%l)
    state = string.strip(p.stdout.readline())
    s = int(string.strip(p.stdout.readline()))
    # print state
    # print s
    if state == "normal":
        move = [int(string.strip(p.stdout.readline())) for i in range(4)]
        print move
    drawBoard(currboard,canvas)

def mouseClick(event):
    global pos1,pos2,currboard,state,score,move
    x,y = event.x,event.y
    i,j = getIndex(x,y)
    if i >= 0 and i < nw and j >= 0 and j < nh:
        if len(pos1) == 0:
            pos1 = [i,j]
        else:
            pos2 = [i,j]
            if pos1 != pos2 and connected(currboard,pos1[0],pos1[1],pos2[0],pos2[1]) and \
                currboard[pos1[0]][pos1[1]] != "." and \
                currboard[pos2[0]][pos2[1]] == ".":

                currboard[pos2[0]] = strrep(currboard[pos2[0]],pos2[1],currboard[pos1[0]][pos1[1]])
                currboard[pos1[0]] = strrep(currboard[pos1[0]],pos1[1],".")
                state,score,currboard = cancel(currboard,score)
                if state != "cancelled":
                    state,currboard = add_rand(currboard)
                    state,score,currboard = cancel(currboard,score)
                move = []
            pos1,pos2 = [],[]
    drawBoard(currboard,canvas)
    scoreLabel.config(text=str(score))

if __name__ == "__main__":

    x0,y0,w,h = 0,0,40,40
    nw,nh = 9,9
    gw,gh = nw*w,nh*h
    x1,y1 = x0+gw,y0+gh

    colorMap = {
        "r":"red",
        "g":"green",
        "b":"blue",
        "y":"yellow",
        "o":"orange",
        "d":"cyan",
        "p":"magenta",
        ".":"white",
    }

    root = Tk()

    rframe = Frame(root)
    lframe = Frame(root)

    hintButton = Button(rframe,text="Hint",command=showHint)
    scoreLabel = Label(rframe,text="Score")
    canvas = Canvas(lframe,width=gw,height=gh)
    canvas.bind("<Button-1>",mouseClick)

    rframe.pack(side=RIGHT)
    lframe.pack(side=LEFT)

    hintButton.pack(side=TOP)
    scoreLabel.pack(side=BOTTOM)
    canvas.pack(side=LEFT)

    currboard = ["."*nw]*nh
    state = "normal"
    score = 0
    pos1 = []
    pos2 = []
    move = []
    scoreLabel.config(text=str(score))

    state,currboard = add_rand(currboard)

    drawBoard(currboard,canvas)
    # canvas.create_rectangle(20,20,300,300,fill="red")
    # canvas.create_text(30,30,text="Canvas",anchor=NW,fill="blue")

    root.mainloop()
