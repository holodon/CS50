##
# Implements the caesar cipher
##

import cs50
import sys


def main():

    # check if called with exactly one argument
    if len(sys.argv) != 2:
        show_usage()
        exit(1)

    # check if the argument is non-negative integer
    if not sys.argv[1].isdigit():
        show_usage()
        exit(1)
    k = int(sys.argv[1])

    # ask for plaintext
    print("plaintext:  ", end="")
    p = cs50.get_string()

    # calculate and print encrypted text
    print("ciphertext: ", end="")
    for c in p:

        # encrypt and print alphabetic characters
        if c.isalpha():
            print(enc(ord(c), k), end="")

        # just print everything else
        else:
            print(c)

    # print newline for better formatting
    print()
    exit(0)


# this function encrypts single character c with a key k
# and returns the encrypted char
def enc(c, k):

    # shift sets the distance to zero for the upper and the lower case letters
    shift = 65

    # check and set for lowcase
    if c > 96:
        shift = 97
    return chr(((c - shift + k) % 26) + shift)


def show_usage():
    print("Usage: ./caesar k")

if __name__ == "__main__":
    main()
