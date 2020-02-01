"""
Week 13 - Prob I - Cannibal Island
"""

import sys




def solve_test_case():
  n, k = map(int, sys.stdin.readline().strip().split(' '))

  pigs = list(range(1, n + 1))
  curr_ptr = 0
  while len(pigs) > 1:
    curr_ptr = (curr_ptr + k - 1) % len(pigs)
    pigs.pop(curr_ptr)

  return pigs[0]


if __name__ == '__main__':
    n_cases = int(input())
    for i in range(n_cases):
        print('Case #{}: {}'.format(i + 1, solve_test_case()))
