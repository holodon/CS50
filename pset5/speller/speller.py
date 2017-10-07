import sys
import time
import re
from dictionary import Dictionary


def main():
    # check if called with exactly one argument
    l = len(sys.argv)
    if l != 2 and l != 3:
        print("Usage: speller [dictionary] text")
        exit(1)

    # time buffers
    time_load = 0.0
    time_check = 0.0
    time_size = 0.0
    time_unload = 0.0
    before = 0.0

    # dictionary file
    dictionary = sys.argv[1] if l == 3 else "dictionaries/large"

    # dictionary object
    dic = Dictionary()

    # load dictionary
    before = time.time()
    if not dic.load(dictionary):
        print("Could not load {}".format(dictionary))
        exit(1)
    time_load = time.time() - before

    # open text
    text = sys.argv[2] if l == 3 else sys.argv[1]
    file = open(text, "r")
    # TODO: check if succesfull

    # prepare to report misspellings
    print("\nMISSPELLED WORDS\n")

    # prepare to spell-check
    misspellings = 0
    words = 0

    # spell-check each word in text
    with open(text, "r") as f:
        for line in f:

            # TODO: still not 100% working - optimize
            for word in re.findall(r"[a-zA-Z'\'']+\b", line):
                if len(word) < 46:
                    words += 1

                    # check word's spelling
                    before = time.time()
                    misspelled = not dic.check(word)
                    time_check += time.time() - before

                    # print word if misspelled
                    if misspelled:
                        print(word)
                        misspellings += 1

    # close the file
    f.close

    # determine dictionary's size
    before = time.time()
    n = dic.size()
    time_size = time.time() - before

    # unload dictionary
    before = time.time()
    dic.unload()
    time_unload = time.time() - before

    # report
    print("\nWORDS MISSPELLED:     {0:0.0f}".format(misspellings))
    print("WORDS IN DICTIONARY:  {0:0.0f}".format(n))
    print("WORDS IN TEXT:        {0:0.0f}".format(words))
    print("TIME IN load:         {0:0.2f}".format(time_load))
    print("TIME IN check:        {0:0.2f}".format(time_check))
    print("TIME IN size:         {0:0.2f}".format(time_size))
    print("TIME IN unload:       {0:0.2f}".format(time_unload))
    print("TIME IN TOTAL:        {0:0.2f}\n".format(time_load
                                                    + time_check
                                                    + time_size
                                                    + time_unload))

    # that's all folks
    exit(0)

if __name__ == "__main__":
    main()
