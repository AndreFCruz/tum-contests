"""
Week 1 - Problem D
"""

import sys

if __name__ == '__main__':
    
    num_cases = int(sys.stdin.readline())
    for i in range(num_cases):
        if i > 0: sys.stdin.readline()
        n = int(sys.stdin.readline())
        members = []
        for _ in range(n):
            scores = map(int, sys.stdin.readline().rstrip().split(' '))
            members.append(tuple(sorted(scores, reverse=True)))

        members = sorted(members, reverse=True)
        
        ## Output
        print('Case #{}:'.format(i+1))
        for j in range(n):
            print(' '.join(map(str, members[j])))
