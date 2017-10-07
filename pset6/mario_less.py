##
# Calculates and draws a single "mario" pyramide
##

import cs50


def main():
    while True:
        print("Height: ", end="")
        i = cs50.get_int()
        if input_okay(i):
            break
    draw_pyramid(i)
    exit(0)


def input_okay(i):
    if i < 0 or i > 23:
        return False
    return True


def draw_pyramid(rows):
    cells = rows + 1
    hashes = 2
    for k in range(rows):
        draw_symbols(" ", cells - hashes)
        draw_symbols("#", hashes)
        print()
        hashes += 1


def draw_symbols(s, c):
    for i in range(c):
        print(s, end="")

if __name__ == "__main__":
    main()
