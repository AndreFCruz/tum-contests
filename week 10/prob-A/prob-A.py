"""
Week 10 - Prob A - Meteorite
Solution: Point inside arbitrary Polygon w/ ray casting
"""

import sys
import math
from random import random


def equal_floats(f1: float, f2: float):
    # return abs(f1 - f2) < sys.float_info.epsilon
    return abs(f1 - f2) < 1e-10


def random_point_in_circumference(r = 1):
    theta = 2 * 3.14 * random()
    return math.cos(theta) * r, math.sin(theta) * r


class Segment:
    def __init__(self, x1, y1, x2, y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2

    def intersects_segment(self, other):
        xi, yi, valid, r, s = intersectLines(
            [self.x1, self.y1], [self.x2, self.y2],
            [other.x1, other.y1], [other.x2, other.y2]
        )
        return valid and 0 < r < 1 and 0 < s < 1


def intersectLines(pt1, pt2, ptA, ptB) -> tuple:
    """
    From: https://www.cs.hmc.edu/ACM/lectures/intersections.html

    - Returns the intersection of Line(pt1,pt2) and Line(ptA,ptB)
    - Returns a tuple: (xi, yi, valid, r, s), where
        (xi, yi) is the intersection
        r is the scalar multiple such that (xi,yi) = pt1 + r*(pt2-pt1)
        s is the scalar multiple such that (xi,yi) = pt1 + s*(ptB-ptA)
            valid == 0 if there are 0 or inf. intersections (invalid)
            valid == 1 if it has a unique intersection ON the segment
    """

    DET_TOLERANCE = 0.00000001

    # the first line is pt1 + r*(pt2-pt1)
    # in component form:
    x1, y1 = pt1;   x2, y2 = pt2
    dx1 = x2 - x1;  dy1 = y2 - y1

    # the second line is ptA + s*(ptB-ptA)
    x, y = ptA;   xB, yB = ptB;
    dx = xB - x;  dy = yB - y;

    # we need to find the (typically unique) values of r and s
    # that will satisfy
    #
    # (x1, y1) + r(dx1, dy1) = (x, y) + s(dx, dy)
    #
    # which is the same as
    #
    #    [ dx1  -dx ][ r ] = [ x-x1 ]
    #    [ dy1  -dy ][ s ] = [ y-y1 ]
    #
    # whose solution is
    #
    #    [ r ] = _1_  [  -dy   dx ] [ x-x1 ]
    #    [ s ] = DET  [ -dy1  dx1 ] [ y-y1 ]
    #
    # where DET = (-dx1 * dy + dy1 * dx)
    #
    # if DET is too small, they're parallel
    #
    DET = (-dx1 * dy + dy1 * dx)

    if math.fabs(DET) < DET_TOLERANCE:
        return 0, 0, 0, 0, 0

    # now, the determinant should be OK
    DETinv = 1.0/DET

    # find the scalar amount along the "self" segment
    r = DETinv * (-dy  * (x-x1) +  dx * (y-y1))

    # find the scalar amount along the input line
    s = DETinv * (-dy1 * (x-x1) + dx1 * (y-y1))

    # return the average of the two descriptions
    xi = (x1 + r*dx1 + x + s*dx)/2.0
    yi = (y1 + r*dy1 + y + s*dy)/2.0
    return xi, yi, 1, r, s


def solve_test_case():
    x_impact, y_impact, n = map(int, sys.stdin.readline().rstrip().split(' '))
    segments = [Segment(*[int(x) for x in sys.stdin.readline().rstrip().split(' ')]) for _ in range(int(n))]

    ray_end_x, ray_end_y = random_point_in_circumference(1500)
    ray_segment = Segment(x_impact, y_impact, ray_end_x, ray_end_y)

    n_intersects = sum(ray_segment.intersects_segment(s) for s in segments)
    return 'jackpot' if n_intersects % 2 == 1 else 'too bad'


if __name__ == '__main__':
    n_cases = int(input())
    for i in range(n_cases):
        print('Case #{}: {}'.format(i + 1, solve_test_case()))
        sys.stdin.readline()
