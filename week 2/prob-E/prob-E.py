import sys
from collections import defaultdict


def get_nums(): return map(int, input().strip().split())
def get_num(): return int(input().strip())


class TestCase:
    def __init__(self):
        self.n_countries, self.n_interactions = get_nums()
        self.relations = defaultdict(list)
        for _ in range(self.n_interactions):
            rel, c1, c2 = input().strip().split()
            self.relations[int(c1)].append()



if __name__ == '__main__':
    num_tests = get_num()
    for i in range(num_tests):
        print('Case #{}: {}'.format(i+1, TestCase().solve()))
