# simple program to convert addresses like 0x7fc49a404b80 to unique index from 0
# reads in stuff like:
#
#   malloc 56 -> 0x7fc49a500c00
#   free 0x7fc49a500c00
#
# and emits
#
#   malloc 56 -> 0
#   free 0
#

import sys

addr2index = {}
next = 0

for line in sys.stdin:
    words = line.strip().split(" ")
    if len(words)==0: break
    if words[0] == "malloc":
        addr = words[3]
        if addr not in addr2index:
            addr2index[addr] = str(next)
            next += 1
        words[3] = addr2index[addr]
    else: # must be free
        if addr not in addr2index:
            print "FREE INVALID ADDRESS:", line
        words[1] = addr2index[addr]
    print " ".join(words)