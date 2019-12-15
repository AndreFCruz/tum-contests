"""
Week 9 - Prob D
"""

import sys

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
    n_games, n_friends = map(int, sys.stdin.readline().rstrip().split(' '))

    primes = dict()
    sizes, remainders = list(), list()

    m = 1
    for _ in range(n_games):
        s, r = map(int, sys.stdin.readline().rstrip().split(' '))

        # Check consistent sizes and remainders
        if s in primes:
            if primes[s] != r:
                return "impossible"
        else:
            primes[s] = r
            m = m * s
            sizes.append(s)
            remainders.append(r)

    result = 0
    for i in range(len(sizes)):
        m_i = m // sizes[i]
        inv = modinv(m_i, sizes[i])
        result += remainders[i] * m_i * inv

    result = result % m
    scale = (n_friends - result) // m
    result += scale * m
    if result > n_friends or result < 0:
        return "impossible"
    else:
        return result


if __name__ == '__main__':
    n_cases = int(input())
    for i in range(n_cases):
        print('Case #{}: {}'.format(i + 1, solve_test_case()))
        sys.stdin.readline()
