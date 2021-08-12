#!/usr/bin/python

"""Test runner for screen saver executables."""

__author__ = 'Simon Aertssen'

import re
import os
import sys
import time
import subprocess


BINARY = './screensaver'
GREEN = '\033[92;1m'
RED = '\033[91;1m'
END = '\033[0m'


def wait_for_test_process(proc, timeout):
    """Wait for a test process with a timeout, and reads stderr.

    Reading stderr while polling is important, or a deadlock can occur if the
    pipe's internal buffer fills up.
    """
    endtime = time.time() + timeout
    err_chunks = []
    while proc.returncode is None and time.time() < endtime:
        time.sleep(0.1)
        proc.poll()

    # Kill the child if it hasn't stopped yet, and wait for it.
    timed_out = False
    if proc.returncode is None:
        proc.kill()
        proc.wait()
        timed_out = True
        print(f'Test process timed out after {timeout}s...')

    # Read the rest of stderr.
    chunk = True
    while chunk:
        chunk = os.read(proc.stderr.fileno(), 4096)
        err_chunks.append(chunk)

    # Check if there are any errors registered
    if len(err_chunks) > 1 and err_chunks[0] != b'':
        lines = b''.join(err_chunks)

    # If not, then read from the stdout
    lines = b''.join(proc.stdout.readlines())
    return (timed_out, str(lines))


def run_binary(binary, frames, input_file):
    timeout = 60*60.0
    done_testing = False
    # Run the binary in a subprocess
    while not done_testing:
        proc = subprocess.Popen([binary, str(frames), input_file],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (timed_out, lines) = wait_for_test_process(proc, timeout)

        # If there was a timeout, try it again
        if timed_out:
            raise TimeoutError(f'Animation {input_file} timed out.')
        elif proc.returncode != 0:
            raise ValueError(f'{input_file} returned {proc.returncode} error code.')

        # Process the output lines
        numbers = re.findall(r"[-+]?\d*\.?\d+", lines)
        if len(numbers) > 4:
            print("Found more than 4 integers in this output.")
            numbers = numbers[0:4]
        done_testing = True
    _, exc_time, l_w_coll, l_l_coll = numbers  # Drop the frame number
    return float(exc_time), int(l_w_coll), int(l_l_coll)


def produce_test_table(frames=10, verbose=False):
    output_filename = f'../test_output/test_table_{frames}.txt'
    output_file = open(output_filename, "w")

    output = '| Animation Name | Execution Time | Line-Wall Collisions | Line-Line Collisions |\n'
    output += '| - | - | - | - |\n'

    input_directory = '../screensaver/input/'
    for filename in os.listdir(input_directory):
        try:
            exc_time, l_w_coll, l_l_coll = run_binary(BINARY, frames, input_directory + filename)
        except Exception:
            exc_time, l_w_coll, l_l_coll = '-', '_', '_'
        current_output = f'| {filename} | {exc_time} s | {l_w_coll} | {l_l_coll} |\n'
        output += current_output
        if verbose:
            print(current_output)
    output_file.write(output)
    output_file.close()


def print_result(name, result, verbose):
    """If result is True, print a green PASSED or red FAILED line otherwise."""
    flag = GREEN + ' PASSED' if result is True else RED + ' FAILED'
    if verbose:
        print(name + flag + END)
    # if not result:
        # raise ValueError(intro + flag + END)
    return result


def test_correctness(frames, verbose):
    input_directory = '../screensaver/input/'

    test_results = [0, 0]  # Failed and passed tests: 1*False = 0

    # First test the box.in animation manually as a first line of defence
    file_name = input_directory + 'box.in'
    _, l_w_coll, l_l_coll = run_binary(BINARY, 100, file_name)
    result = print_result(file_name, l_w_coll == 108 and l_l_coll == 3384, verbose)
    test_results[result*1] += 1

    # Now test the beaver.in animation manually as a first line of defence
    file_name = input_directory + 'beaver.in'
    _, l_w_coll, l_l_coll = run_binary(BINARY, 1000, file_name)
    result = print_result(file_name, l_w_coll == 7 and l_l_coll == 758, verbose)
    test_results[result*1] += 1

    # If we reached this point all tests should have passed:
    index = test_results.index(max(test_results))
    print_result(f'{test_results[index]} / {sum(test_results)} tests', index == 1, True)


def main(args):
    verbose = False if '--quiet' in args else True
    frames = 1000
    if '--table' in args:
        index = args.index('--table') + 1
        frames = args[index]
        produce_test_table(frames, verbose)
    elif '--test' in args:
        test_correctness(frames, verbose)


if __name__ == '__main__':
    main(sys.argv)
