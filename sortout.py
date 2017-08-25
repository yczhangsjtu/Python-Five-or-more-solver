import sys
import string

lines = sys.stdin.readlines()
lines = map(string.strip,lines)

for i,line in enumerate(lines):
    print line,len(lines)-i-1
