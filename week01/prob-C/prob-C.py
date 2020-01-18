"""
Week 1 - Problem C
"""

import sys

SPEED_OF_LIGHT = 299792458

if __name__ == '__main__':

    num_cases = int(sys.stdin.readline())
    for i in range(num_cases):
        mass = int(sys.stdin.readline())
        print('Case #{}: {}'.format(i+1, mass * (SPEED_OF_LIGHT ** 2)))
