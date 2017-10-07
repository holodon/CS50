##
# Validates a credit card number
# according to algorithm invented by Hans Peter Luhn
##

import cs50


def main():
    print("Number: ", end="")
    card = cs50.get_int()
    sum = sum_flip(card, False, 2) + sum_flip(card, True, 1)
    if (sum % 10) == 0:
        i = card
        while i > 10:
            i //= 10
        m = get_sec(card)
        if i == 3:
            if m == 4 or m == 7:
                print("AMEX")
            else:
                print("INVALID")
        elif i == 4:
            print("VISA")
        elif i == 5:
            if m > 0 and m < 6:
                print("MASTERCARD")
            else:
                print("INVALID")
        else:
            print("INVALID")
    else:
        print("INVALID")
    exit(0)


def get_sec(card):
    while True:
        if card <= 100:
            break
        card //= 10
    return int(card % 10)


def sum_flip(i, flip, m):
    sum = 0
    while i != 0:
        if flip:
            ss = sum_sum(int(i % 10)*m)
            sum += ss
        i //= 10
        flip = not flip
    return sum


def sum_sum(i):
    sum = 0
    while i != 0:
        sum += i % 10
        i //= 10
    return sum

if __name__ == "__main__":
    main()
