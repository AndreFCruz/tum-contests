import sys


def get_nums(): return map(int, input().strip().split())
def get_num(): return int(input().strip())


class TestCase:
    def __init__(self):
        n_stations, n_friends = get_nums()

        self.stations = list()  # room range for each station
        for _ in range(n_stations):
            n1, n2 = get_nums()
            self.stations.append((n1, n2))

        self.friends = list()   # friend's room line in map
        for _ in range(n_friends):
            n = get_num()
            self.friends.append(n)

        self.friends_rooms = None

    def get_room_from_line(self, n):
        '''
        Returns the room number that corresponds to the given line on the map.
        :param n: a line number
        :return: the room number
        '''
        station_idx = 0
        while True:
            n1, n2 = self.stations[station_idx]
            if n < n2 - n1 + 1: # n is in this station's range
                return n1 + n - 1
            n -= n2 - n1 + 1    # subtract this station's lines

    def solve(self):
        self.friends_rooms = [
            self.get_room_from_line(n) for n in self.friends
        ]

    def __str__(self):
        if not self.friends_rooms:
            return 'not solved'
        return '\n'.join(self.friends_rooms)


if __name__ == '__main__':
    t = get_num()

    for i in range(t):
        print('Case #{}:'.format(i+1))
        TestCase().solve()
        print(TestCase)
        if sys.stdin: sys.stdin.read(1) # extract line-break if not EOF
