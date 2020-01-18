"""
Week 9 - Prob E
"""

import sys
from fractions import Fraction as frac
from collections import defaultdict


class DiceSet:
    def __init__(self, n_dices: int, n_sides: int):
        self.n_dices = int(n_dices)
        self.n_sides = int(n_sides)


def solve_test_case():
    line = sys.stdin.readline().rstrip().split(' ')
    n_points = int(line[0])
    dice_sets = [DiceSet(*d_str.split('d')) for d_str in line[1].split('+')]
    dices = [ds.n_sides for ds in dice_sets for _ in range(ds.n_dices)]

    dices = sorted(dices)
    probs = None
    for idx, n_sides in enumerate(dices):
        # If first dice
        if idx == 0:
            # Probability of obtaining i points is 1 / n_sides
            probs = {p: frac(1, n_sides) for p in range(1, n_sides + 1)}
            continue

        # Remaining dices
        new_probs = defaultdict(lambda: frac(0, 1))
        for points in range(1, n_sides + 1):
            for old_points, old_prob in probs.items():
                new_probs[old_points + points] += old_prob * frac(1, n_sides)

        probs = new_probs


    # Calculate probability of getting more than n points
    goal_prob = frac(0, 1)
    for points, prob in probs.items():
        if points >= n_points:
            goal_prob += prob

    return \
        '0/1' if goal_prob.numerator == 0 else \
        '1/1' if goal_prob.denominator == 1 else \
        str(goal_prob)


if __name__ == '__main__':
    n_cases = int(input())
    for i in range(n_cases):
        print('Case #{}: {}'.format(i + 1, solve_test_case()))
