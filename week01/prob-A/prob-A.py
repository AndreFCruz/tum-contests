"""
Week 1 - Problem A
"""

import sys

if __name__ == '__main__':

    lines = sys.stdin.readlines()[1:]
    for i, line in enumerate(lines):
        print('Case #{}: Hello {}!'.format(i+1, line.strip()))
