##
# Cracks up to 4 letter alphabetical passwords by bruteforce
##

import sys
import crypt
import itertools


def main():

    # check if called with exactly one argument
    if len(sys.argv) != 2:
        show_usage()
        exit(1)

    # all possible characters
    aA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    h = sys.argv[1]

    # check the hash length
    if len(h) != 13:
        show_usage()
        exit(1)

    # crack with itertools
    for i in range(1, 5):
        for c in itertools.product(aA, repeat=i):
            p = ''.join(c)
            if crypt.crypt(''.join(p), h) == h:
                print(p)
                exit(0)

    # not cracked
    print("All variations tested - none matched!")
    exit(1)

# shows usage
def show_usage():
    print("Usage: ./crack hash")

if __name__ == "__main__":
    main()
