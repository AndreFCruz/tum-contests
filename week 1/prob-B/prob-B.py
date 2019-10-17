"""
Week 1 - Problem B
"""

import sys

if __name__ == '__main__':

    num_cases = int(sys.stdin.readline())

    for i in range(num_cases):
        print('Case #{}:'.format(i + 1))
        num_sents = int(sys.stdin.readline())
        for j in range(num_sents):
            line = sys.stdin.readline().rstrip()
            line = line.replace('enten', 'ierende')
            line = line.replace('entin', 'ierende')
            line = line.replace('ent', 'ierender')
            print(line)
