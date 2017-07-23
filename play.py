#!/usr/bin/python

from Tkinter import *

if __name__ == "__main__":

    root = Tk()

    rframe = Frame(root)
    lframe = Frame(root)

    hintButton = Button(rframe,text="Hint")
    scoreLabel = Label(rframe,text="Score")
    canvas = Canvas(lframe)

    rframe.pack(side=RIGHT)
    lframe.pack(side=LEFT)

    hintButton.pack(side=TOP)
    scoreLabel.pack(side=BOTTOM)
    canvas.pack(side=LEFT)

    # canvas.create_rectangle(20,20,300,300,fill="red")
    # canvas.create_text(30,30,text="Canvas",anchor=NW,fill="blue")

    root.mainloop()
