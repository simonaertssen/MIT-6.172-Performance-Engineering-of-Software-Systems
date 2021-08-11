import os
import sys


def produce_test_table(frames):
    output_filename = f'test_table_{frames}.txt'
    output_file = open(output_filename, "w")
    
    output = ''
    for filename in os.listdir('../screensaver/input/'):
        print(filename)
        output += f'{filename} | | | |'

    output_file.write(output)


def main(argv):
    produce_test_table(10)   


if __name__ == '__main__':
    main(sys.argv)
