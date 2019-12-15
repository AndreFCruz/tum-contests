"""
Week 9 - Prob B - Finding the modular inverse.
"""

import sys
import math


def extended_gcd(aa, bb):
    """From: https://rosettacode.org/wiki/Modular_inverse#Iteration_and_error-handling"""
    lastremainder, remainder = abs(aa), abs(bb)
    x, lastx, y, lasty = 0, 1, 1, 0
    while remainder:
        lastremainder, (quotient, remainder) = remainder, divmod(lastremainder, remainder)
        x, lastx = lastx - quotient*x, x
        y, lasty = lasty - quotient*y, y
    return lastremainder, lastx * (-1 if aa < 0 else 1), lasty * (-1 if bb < 0 else 1)


def modinv(a, m):
    """From: https://rosettacode.org/wiki/Modular_inverse#Iteration_and_error-handling"""
    g, x, y = extended_gcd(a, m)
    if g != 1:
        raise ValueError
    return x % m


def solve_test_case():
    n, y = map(int, sys.stdin.readline().rstrip().split(' '))
    return modinv(y, 10 ** n)

if __name__ == '__main__':
    n_cases = int(input())
    for i in range(n_cases):
        print('Case #{}: {}'.format(i + 1, solve_test_case()))
