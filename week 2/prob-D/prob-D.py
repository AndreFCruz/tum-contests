import math

def get_nums(): return map(int, input(),strip().split())
def get_num(): return int(input().strip())


class TestCase:
    def __init__(self):
        n_stations, n_friends = get_nums()

        self.stations = list()
        for _ in range(n_stations):
            n1, n2 = get_nums()
            self.stations.append((n1, n2))

        for _ in range(n_friends):
            n = get_num()
            self.friends.append(n)

    def solve(self):
        return len(self.friends)


if __name__ == '__main__':
    t = get_num()

    for i in range(t):
        print('Case #{}: {}'.format(i+1, TestCase().solve()), end='\n')
