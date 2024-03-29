#!/usr/bin/env python

import random
from part1 import modexp

def primality2(N, k):
    test_values = random.sample( range(1, N), k)
    false_pos = 0
    
    for val in test_values:
        if (modexp(val, N - 1, N) == 1):
            false_pos += 1
    return false_pos

if __name__ == '__main__':
    import sys

    N, k = int(sys.argv[1]), int(sys.argv[2])

    if primality2(N, k):
        print "{} is prime".format(N)
    else:
        print "{} is not prime".format(N)
