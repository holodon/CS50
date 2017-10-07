##
# Calculates the smallest amount of coins for a given change
##

import cs50


def main():
    change = get_cents()
    coins = 0
    coin_type = [25, 10, 5, 1]
    for i in range(4):
        while change >= coin_type[i]:
            coins += 1
            change -= coin_type[i]
    print(coins)
    exit(0)


def get_cents():
    while True:
        print("O hai! How much change is owed?")
        f = cs50.get_float()
        if f >= 0:
            break
    return round(f*100)

if __name__ == "__main__":
    main()
