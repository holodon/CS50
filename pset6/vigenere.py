##
# Implements the vigenere cipher
##

import cs50
import sys


def main():

    # check if called with exactly one argument
    if len(sys.argv) != 2:
        show_usage()
        exit(1)

    # check if k consist only of alphabeticals
    k = sys.argv[1]
    k_len = len(k)
    if not k.isalpha():
        show_usage()
        exit(1)

    # ask for plaintext
    print("plaintext:  ", end="")
    p = cs50.get_string()

    # calculate and print encrypted text
    print("ciphertext: ", end="")
    i = 0
    for c in p:
        if c.isalpha:
            print(enc(ord(c), k[i]), end="")
            i += 1
            if i == k_len:
                i = 0
        else:
            print(c, end="")

    # print newline for better formatting
    print()
    exit(0)


# this function encrypts single character c with a key k
def enc(c, k):
    shift = 65
    if c > 96:
        shift = 97
    m = ord(k.upper()) - 65
    return chr(((c - shift + m) % 26) + shift)


# prints usage
def show_usage():
    print("Usage: ./vigenere k")

if __name__ == "__main__":
    main()
