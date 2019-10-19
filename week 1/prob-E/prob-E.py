"""
Week 1 - Problem E
"""

import sys
from enum import Enum

operations = {
    'plus': lambda x, y: x + y,
    'minus': lambda x, y: x - y,
    'tothepowerof': lambda x, y: x ** y,
    'times': lambda x, y: x * y,
}


class State(Enum):
    NUMBER = 1
    OPERATION = 2


def eval_string(line):
    """Evaluates the mathematical expression written on the given string"""

    val = 0
    num, op = '', ''
    state = State.NUMBER
    for i, char in enumerate(line):

        # import ipdb; ipdb.set_trace()
        if state == State.NUMBER and not char.isdigit():
            state = State.OPERATION
            new_num = int(num)
            if len(op) == 0:
                val += new_num
            else:
                val = operations[op](val, new_num)
            num, op = '', ''

        elif state == State.OPERATION and char.isdigit():
            state = State.NUMBER


        if char.isdigit():
            num = num + char
        else:
            op = op + char

    ## Handle final digit/number
    val = operations[op](val, int(num)) if op else int(num)
    return val


if __name__ == '__main__':

    num_cases = int(sys.stdin.readline())

    for i in range(num_cases):
        line = sys.stdin.readline().rstrip()
        print('Case #{}: {}'.format(i+1, eval_string(line)))
