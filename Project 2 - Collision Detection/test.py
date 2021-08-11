import os
import sys


def produce_test_table():
    for filename in os.listdir('screensaver/input'):
        print(filename)


def main(argv):
    produce_test_table()   


if __name__ == '__main__':
    main(sys.argv)
