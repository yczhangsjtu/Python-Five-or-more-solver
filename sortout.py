import sys
import string

lines = sys.stdin.readlines()
lines = map(string.strip,lines)

cton = {'.':0, 'r':1, 'g':2, 'b':3,
        'y':4, 'o':5, 'p':6, 'd':7,
        }

ctol = {'.':'0000000',
        'r':'0000001',
        'g':'0000010',
        'b':'0000100',
        'y':'0001000',
        'o':'0010000',
        'p':'0100000',
        'd':'1000000',
    }

def replace(line):
    ret = ''
    for i in range(len(line)):
        ret += ctol[line[i]]
    return ret

class State(object):
    def __init__(self,event,line):
        self.event = event
        self.line = replace(line)
        self.qvalue = Empty(line)
        if event[:6] == "cancel":
            self.event = "cancel"
            self.score = int(event[7:])

def Empty(s):
    count = 0
    for c in s:
        if c == '.':
            count += 1
    return count

states = []
for i in range(0,len(lines),2):
    state = State(lines[i],lines[i+1])
    states.append(state)

gamma = 0.8
for i in range(len(states)-1):
    rate = 1
    for j in range(i+1,len(states)):
        if states[j].event == "add":
            rate *= gamma
        elif states[j].event == "cancel":
            states[i].qvalue += states[j].score * rate

for i in range(len(states)):
    print states[i].line,states[i].qvalue
